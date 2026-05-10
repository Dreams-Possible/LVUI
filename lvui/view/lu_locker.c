#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/plat/lu_mem.h"

#include "lvui/misc/lu_log.h"
#include "lvui/misc/lu_pdc.h"
#include "lvui/misc/lu_sll.h"

#include "lvui/frame/anim/lu_anim.h"
#include "lvui/frame/disp/lu_disp.h"
#include "lvui/frame/disp/lu_font.h"
#include "lvui/frame/input/lu_touch.h"
#include "lvui/frame/theme/lu_color.h"
#include "lvui/frame/theme/lu_theme.h"
#include "lvui/frame/widget/lu_widget.h"
#include "lvui/frame/widget/lu_layout.h"
#include "src/layouts/flex/lv_flex.h"

#include "lu_locker.h"

#include <stdio.h>
#include <string.h>

typedef struct lu_locker_t
{
    lu_disp_t* lu_disp;
    lu_theme_t* lu_theme;
    lu_touch_t* lu_touch;
    lv_obj_t* contain;
    lv_obj_t* contain_mask;
    lv_obj_t* bg;
    lv_obj_t* info_mask;
    lv_obj_t* info_contain;
    lv_obj_t* info_txt_contain;
    lu_theme_obj_t* info_txt_contain_theme_obj;
    lv_obj_t* info_icon_contain;
    lv_obj_t* info_icon_left;
    lu_theme_obj_t* info_icon_left_theme_obj;
    lv_obj_t* info_icon_right;
    lu_theme_obj_t* info_icon_right_theme_obj;
    // lv_obj_t* time;
    // char time_str[8];
    // lv_obj_t* date;
    // char date_str[8];
    lv_obj_t* locker_mask;
    lu_anim_t* info_mask_anim;
    lv_timer_t* timer;
    bool if_exit;

    bool is_pressed;

    lu_locker_cb_t cb[LU_LOCKER_EVENT_SUMNUM];
    void* user_data[LU_LOCKER_EVENT_SUMNUM];
}lu_locker_t;

static void locker_event_cb(lu_locker_t* locker, lu_locker_event_t event);
static void locker_mask_event_cb(lv_event_t *event);
static void locker_timer_cb(lv_timer_t* timer);
lu_locker_t* lu_locker_create(lu_disp_t* lu_disp, lu_theme_t* lu_theme, lu_touch_t* lu_touch, lv_obj_t* parent);
void lu_locker_delete(lu_locker_t** locker);
// void lu_locker_set_time(lu_locker_t* locker, const char* hour, const char* minute, const char* second);
// void lu_locker_set_date(lu_locker_t* locker, const char* day, const char* month, const char* year);
void lu_locker_enter(lu_locker_t* locker);
void lu_locker_exit(lu_locker_t* locker);
bool lu_locker_get_status(lu_locker_t* locker);
void lu_locker_set_cb(lu_locker_t* locker, lu_locker_event_t event, lu_locker_cb_t cb, void* user_data);
void* lu_locker_get_user_data(lu_locker_t* locker, lu_locker_event_t event);
lv_obj_t* lu_locker_add_info_txt(lu_locker_t* locker,lu_locker_info_txt_t* info);
void lu_locker_remove_info_txt(lu_locker_t* locker, lu_locker_info_txt_t* info);
lv_obj_t* lu_locker_add_info_icon(lu_locker_t* locker, lu_locker_info_icon_t* info);
void lu_locker_remove_info_icon(lu_locker_t* locker, lu_locker_info_icon_t* info);
void lu_locker_set_wallpaper(lu_locker_t* locker, const char* path);

static void locker_event_cb(lu_locker_t* locker, lu_locker_event_t event)
{
    if(locker->cb[event])
    {
        locker->cb[event](locker);
    }
    // LU_LOG("locker_event_cb: %d", event);
}

static void locker_mask_event_cb(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lu_locker_t* locker = (lu_locker_t*)lv_event_get_user_data(event);
    switch(code)
    {
        case LV_EVENT_PRESSED:
            locker->is_pressed = true;
        break;
        case LV_EVENT_RELEASED:
            locker->is_pressed = false;
            if(lu_touch_get_moved_y(locker->lu_touch)<-lu_disp_get_ver(locker->lu_disp)*0.4)
            {
                lu_locker_exit(locker);
            }else
            if(lu_touch_get_double_click(locker->lu_touch))
            {
                locker_event_cb(locker, LU_LOCKER_EVENT_DOUBLE_CLICK);
            }
        break;
        default:
        break;
    }
}

static void locker_timer_cb(lv_timer_t* timer)
{
    lu_locker_t* locker = (lu_locker_t*)lv_timer_get_user_data(timer);
    if(locker->if_exit)
    {
        return;
    }
    if(locker->is_pressed)
    {
        if(lu_touch_get_long_press(locker->lu_touch))
        {
            locker_event_cb(locker, LU_LOCKER_EVENT_LONG_PRESS);
        }
        lu_anim_set_value(locker->info_mask_anim, lu_touch_get_moved_y(locker->lu_touch));
    }else
    {
        lu_anim_set_value(locker->info_mask_anim, lu_touch_get_moved_y(locker->lu_touch));
    }
}

lu_locker_t* lu_locker_create(lu_disp_t* lu_disp, lu_theme_t* lu_theme, lu_touch_t* lu_touch, lv_obj_t* parent)
{
    if(!lu_disp||!lu_theme||!lu_touch||!parent)
    {
        return NULL;
    }
    lu_locker_t* locker = (lu_locker_t*)lu_calloc(1, sizeof(lu_locker_t));
    if(!locker)
    {
        return NULL;
    }
    locker->lu_disp = lu_disp;
    locker->lu_theme = lu_theme;
    locker->lu_touch = lu_touch;
    lv_obj_t* obj = lu_widget_obj_init(parent);
    if(!obj)
    {
        lu_locker_delete(&locker);
        return NULL;
    }
    locker->contain = obj;
    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_opa(obj, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {
        {
            lv_obj_t* obj = lu_widget_obj_init(locker->contain);
            if(!obj)
            {
                lu_locker_delete(&locker);
                return NULL;
            }
            locker->contain_mask = obj;
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            // lv_obj_set_style_bg_opa(obj, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            // lu_theme_add_obj(locker->lu_theme, obj, LU_THEME_TYPE_MODE);
            {
                // lv_obj_t* obj = lu_widget_obj_init(locker->contain_mask);
                lv_obj_t* obj = lu_widget_image_init(locker->contain_mask);
                if(!obj)
                {
                    lu_locker_delete(&locker);
                    return NULL;
                }
                locker->bg = obj;
                lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                lv_obj_set_style_bg_opa(obj, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
                // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                lu_theme_info_t info = 
                {
                    .self_type = LU_THEME_TYPE_MODE,
                    .flag = LU_THEME_FLAG_SELF,
                };
                lu_theme_add_obj(locker->lu_theme, obj, &info);
            }
            {
                lv_obj_t* obj = lu_widget_obj_init(locker->contain_mask);
                if(!obj)
                {
                    lu_locker_delete(&locker);
                    return NULL;
                }
                locker->info_mask = obj;
                lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                lv_obj_set_style_pad_hor(obj, lu_disp_get_hor(locker->lu_disp)*0.1, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_pad_ver(obj, lu_disp_get_ver(locker->lu_disp)*0.15, LV_PART_MAIN | LV_STATE_DEFAULT);
                // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                {
                    lv_obj_t* obj = lu_widget_obj_init(locker->info_mask);
                    if(!obj)
                    {
                        lu_locker_delete(&locker);
                        return NULL;
                    }
                    locker->info_contain = obj;
                    // lv_obj_set_size(obj, LV_SIZE_CONTENT,  LV_SIZE_CONTENT);
                    lv_obj_set_size(obj, LV_PCT(100),  LV_SIZE_CONTENT);
                    lv_obj_align(obj, LV_ALIGN_BOTTOM_LEFT, 0, 0);
                    lv_obj_set_style_pad_ver(obj, lu_disp_get_ver(locker->lu_disp)*0.05, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
                    // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    {
                        lv_obj_t* obj = lu_widget_obj_init(locker->info_contain);
                        if(!obj)
                        {
                            lu_locker_delete(&locker);
                            return NULL;
                        }
                        locker->info_txt_contain = obj;
                        // lu_theme_add_obj(locker->lu_theme, obj, LU_THEME_TYPE_INVMODE);
                        lv_obj_set_size(obj, LV_PCT(100),  LV_SIZE_CONTENT);
                        lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
                        // lv_obj_align(obj, LV_ALIGN_BOTTOM_LEFT, 0, 0);
                        // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                        lu_theme_info_t info = 
                        {
                            .child_type = LU_THEME_TYPE_INVMODE,
                            .flag = LU_THEME_FLAG_CHILD,
                        };
                        locker->info_txt_contain_theme_obj = lu_theme_add_obj(locker->lu_theme, obj, &info);
                        if(!locker->info_txt_contain_theme_obj)
                        {
                            lu_locker_delete(&locker);
                            return NULL;
                        }
                    }
                    {
                        lv_obj_t* obj = lu_widget_obj_init(locker->info_contain);
                        if(!obj)
                        {
                            lu_locker_delete(&locker);
                            return NULL;
                        }
                        locker->info_icon_contain = obj;
                        // lu_theme_add_obj(locker->lu_theme, obj, LU_THEME_TYPE_INVMODE);
                        lv_obj_set_size(obj, LV_PCT(100),  LV_SIZE_CONTENT);
                        // lv_obj_align(obj, LV_ALIGN_BOTTOM_LEFT, 0, 0);
                        // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                        {
                            lv_obj_t* obj = lu_widget_obj_init(locker->info_icon_contain);
                            if(!obj)
                            {
                                lu_locker_delete(&locker);
                                return NULL;
                            }
                            locker->info_icon_left = obj;
                            // lu_theme_add_obj(locker->lu_theme, obj, LU_THEME_TYPE_INVMODE);
                            lv_obj_set_size(obj, LV_PCT(50),  LV_SIZE_CONTENT);
                            lv_obj_align(obj, LV_ALIGN_LEFT_MID, 0, 0);
                            lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
                            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lu_theme_info_t info = 
                            {
                                .child_type = LU_THEME_TYPE_INVMODE,
                                .flag = LU_THEME_FLAG_CHILD,
                            };
                            locker->info_icon_left_theme_obj = lu_theme_add_obj(locker->lu_theme, obj, &info);
                            if(!locker->info_icon_left_theme_obj)
                            {
                                lu_locker_delete(&locker);
                                return NULL;
                            }
                        }
                        {
                            lv_obj_t* obj = lu_widget_obj_init(locker->info_icon_contain);
                            if(!obj)
                            {
                                lu_locker_delete(&locker);
                                return NULL;
                            }
                            locker->info_icon_right = obj;
                            // lu_theme_add_obj(locker->lu_theme, obj, LU_THEME_TYPE_INVMODE);
                            lv_obj_set_size(obj, LV_PCT(50),  LV_SIZE_CONTENT);
                            lv_obj_align(obj, LV_ALIGN_RIGHT_MID, 0, 0);
                            lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW_REVERSE);
                            lv_obj_set_flex_align(obj,
                                    LV_FLEX_ALIGN_END,
                                    LV_FLEX_ALIGN_START,
                                    LV_FLEX_ALIGN_START);
                            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lu_theme_info_t info = 
                            {
                                .child_type = LU_THEME_TYPE_INVMODE,
                                .flag = LU_THEME_FLAG_CHILD,
                            };
                            locker->info_icon_right_theme_obj = lu_theme_add_obj(locker->lu_theme, obj, &info);
                            if(!locker->info_icon_right_theme_obj)
                            {
                                lu_locker_delete(&locker);
                                return NULL;
                            }
                        }
                    }
                    // {
                    //     lv_obj_t* obj = lu_widget_txt_init(locker->info_contain, lu_font_get(lu_disp_get_dpi(locker->lu_disp), LU_FONT_SIZE_MAX, 1));
                    //     if(!obj)
                    //     {
                    //         lu_locker_delete(&locker);
                    //         return NULL;
                    //     }
                    //     locker->time = obj;
                    //     lu_theme_add_obj(locker->lu_theme, obj, LU_THEME_TYPE_INVMODE);
                    // }
                    // {
                    //     lv_obj_t* obj = lu_widget_txt_init(locker->info_contain, lu_font_get(lu_disp_get_dpi(locker->lu_disp), LU_FONT_SIZE_HUGE, 1));
                    //     if(!obj)
                    //     {
                    //         lu_locker_delete(&locker);
                    //         return NULL;
                    //     }
                    //     locker->date = obj;
                    //     lu_theme_add_obj(locker->lu_theme, obj, LU_THEME_TYPE_INVMODE);
                    // }
                }
            }
        }
        {
            lv_obj_t* obj = lu_widget_obj_init(locker->contain);
            if(!obj)
            {
                lu_locker_delete(&locker);
                return NULL;
            }
            locker->locker_mask = obj;
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            // lv_obj_set_style_bg_opa(obj, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            // lu_theme_add_obj(locker->lu_theme, obj, LU_THEME_TYPE_MODE);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_add_event_cb(obj, locker_mask_event_cb, LV_EVENT_PRESSED, locker);
            lv_obj_add_event_cb(obj, locker_mask_event_cb, LV_EVENT_RELEASED, locker);
        }
    }
    locker->info_mask_anim = lu_anim_create(locker->info_mask);
    if(!locker->info_mask_anim)
    {
        lu_locker_delete(&locker);
        return NULL;
    }
    lu_anim_param_t param =
    {
        .kp = 0.1,
        .kd = 0.02,
    };
    lu_anim_set_param(locker->info_mask_anim, &param);
    lu_anim_set_get_cb(locker->info_mask_anim, lv_obj_get_y);
    lu_anim_set_set_cb(locker->info_mask_anim, lv_obj_set_y);
    lu_anim_start(locker->info_mask_anim);
    locker->timer = lv_timer_create(locker_timer_cb, LV_DEF_REFR_PERIOD, locker);
    if(!locker->timer)
    {
        lu_locker_delete(&locker);
        return NULL;
    }
    return locker;
}

void lu_locker_delete(lu_locker_t** locker)
{
    if(!locker||!*locker)
    {
        return;
    }
    if((*locker)->timer)
    {
        lv_timer_delete((*locker)->timer);
    }
    if((*locker)->info_mask_anim)
    {
        lu_anim_delete(&(*locker)->info_mask_anim);
    }
    if((*locker)->info_icon_left)
    {
        lu_theme_remove_obj((*locker)->lu_theme, (*locker)->info_icon_left);
        // lv_obj_delete((*locker)->info_icon_left);
    }
    if((*locker)->info_icon_right)
    {
        lu_theme_remove_obj((*locker)->lu_theme, (*locker)->info_icon_right);
        // lv_obj_delete((*locker)->info_icon_right);
    }
    if((*locker)->contain)
    {
        lu_theme_remove_obj((*locker)->lu_theme, (*locker)->bg);
        // lu_theme_remove_obj((*locker)->lu_theme, (*locker)->time);
        // lu_theme_remove_obj((*locker)->lu_theme, (*locker)->date);
        lv_obj_delete((*locker)->contain);
    }
    lu_free(*locker);
    *locker = NULL;
}

// void lu_locker_set_time(lu_locker_t* locker, const char* hour, const char* minute, const char* second)
// {
//     if(!locker)
//     {
//         return;
//     }
//     memset(locker->time_str, 0, sizeof(locker->time_str));
//     snprintf(locker->time_str, sizeof(locker->time_str), "%s:%s:%s", hour, minute, second);
//     lv_label_set_text(locker->time, locker->time_str);
// }

// void lu_locker_set_date(lu_locker_t* locker, const char* day, const char* month, const char* year)
// {
//     if(!locker)
//     {
//         return;
//     }
//     memset(locker->date_str, 0, sizeof(locker->date_str));
//     snprintf(locker->date_str, sizeof(locker->date_str), "%s-%s-%s", year, month, day);
//     lv_label_set_text(locker->date, locker->date_str);
// }

void lu_locker_enter(lu_locker_t* locker)
{
    if(!locker)
    {
        return;
    }
    locker->if_exit = false;
    lv_obj_remove_flag(locker->contain, LV_OBJ_FLAG_HIDDEN);
    lu_anim_set_value(locker->info_mask_anim, 0);
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, locker->contain_mask);
    // lv_anim_set_values(&anim, -lu_disp_get_ver(locker->lu_disp), 0);
    lv_anim_set_values(&anim, -lu_disp_get_ver(locker->lu_disp), 0);
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_time(&anim, 400);
    lv_anim_set_path_cb(&anim, lu_anim_lv_anim_path_cb);
    lv_anim_set_completed_cb(&anim, lu_anim_lv_anim_auto_cb);
    lv_anim_start(&anim);
    locker_event_cb(locker, LU_LOCKER_EVENT_ENTER);
}

static void locker_exit_anim_finish(lv_anim_t* anim)
{
    // lv_anim_delete(anim->var, anim->exec_cb);
    lu_anim_lv_anim_auto_cb(anim);
    lu_locker_t* locker = (lu_locker_t*)lv_anim_get_user_data(anim);
    if(!locker->if_exit)
    {
        locker->if_exit = true;
        lv_obj_add_flag(locker->contain, LV_OBJ_FLAG_HIDDEN);
    }
}

void lu_locker_exit(lu_locker_t* locker)
{
    if(!locker)
    {
        return;
    }
    // lu_anim_set_value(locker->info_mask_anim, -lu_disp_get_ver(locker->lu_disp));
    lu_anim_set_value(locker->info_mask_anim, -lu_disp_get_ver(locker->lu_disp));
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, locker->contain_mask);
    // lv_anim_set_values(&anim, 0, -lu_disp_get_ver(locker->lu_disp));
    lv_anim_set_values(&anim, 0, -lu_disp_get_ver(locker->lu_disp));
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_time(&anim, 400);
    lv_anim_set_path_cb(&anim, lu_anim_lv_anim_path_cb);
    lv_anim_set_completed_cb(&anim, locker_exit_anim_finish);
    lv_anim_set_user_data(&anim, locker);
    lv_anim_start(&anim);
    locker_event_cb(locker, LU_LOCKER_EVENT_EXIT);
}

bool lu_locker_get_status(lu_locker_t* locker)
{
    if(!locker)
    {
        return false;
    }
    return !locker->if_exit;
}

void lu_locker_set_cb(lu_locker_t* locker, lu_locker_event_t event, lu_locker_cb_t cb, void* user_data)
{
    if(!locker)
    {
        return;
    }
    locker->cb[event] = cb;
    locker->user_data[event] = user_data;
}

void* lu_locker_get_user_data(lu_locker_t* locker, lu_locker_event_t event)
{
    if(!locker)
    {
        return NULL;
    }
    return locker->user_data[event];
}

// lv_obj_t* lu_locker_get_info_txt(lu_locker_t* locker)
// {
//     if(!locker)
//     {
//         return NULL;
//     }
//     return locker->info_contain;
// }

lv_obj_t* lu_locker_add_info_txt(lu_locker_t* locker,lu_locker_info_txt_t* info)
{
    if(!locker)
    {
        return NULL;
    }
    lv_obj_t* obj = lu_widget_txt_init(locker->info_txt_contain, info->font);
    if(!obj)
    {
        return NULL;
    }
    lv_label_set_text(obj, info->txt);
    lv_obj_set_width(obj, lv_pct(100));
    lv_label_set_long_mode(obj, LV_LABEL_LONG_SCROLL_CIRCULAR);
    // lu_theme_add_obj(locker->lu_theme, obj, LU_THEME_TYPE_INVMODE);
    return obj;
}

void lu_locker_remove_info_txt(lu_locker_t* locker, lu_locker_info_txt_t* info)
{
    if(!locker||!info)
    {
        return;
    }
    // lv_obj_t* obj=lv_obj_get_child(locker->info_txt_contain, 0);
    lv_obj_t* obj = locker->info_txt_contain;
    for(int32_t i=0;i<lv_obj_get_child_count(obj);++i)
    {
        lv_obj_t* child = lv_obj_get_child(obj, i);
        if(strcmp(lv_label_get_text(child), info->txt) == 0)
        {
            // lu_theme_remove_obj(locker->lu_theme, child);
            lv_obj_delete(child);
            break;
        }
    }
}

lv_obj_t* lu_locker_add_info_icon(lu_locker_t* locker, lu_locker_info_icon_t* info)
{
    if(!locker||!info)
    {
        return NULL;
    }
    switch(info->type)
    {
        case LU_LOCKER_ICON_TYPE_LEFT:
        {
            lv_obj_t* obj = lu_widget_txt_init(locker->info_icon_left, lu_font_get(lu_disp_get_dpi(locker->lu_disp), LU_FONT_SIZE_NORMAL, 1));
            if(!obj)
            {
                return NULL;
            }
            lv_label_set_text(obj, info->icon);
            lu_theme_update_obj(locker->lu_theme, locker->info_icon_left_theme_obj);
            return obj;
        }
        break;
        case LU_LOCKER_ICON_TYPE_RIGHT:
        {
            lv_obj_t* obj = lu_widget_txt_init(locker->info_icon_right, lu_font_get(lu_disp_get_dpi(locker->lu_disp), LU_FONT_SIZE_NORMAL, 1));
            if(!obj)
            {
                return NULL;
            }
            lv_label_set_text(obj, info->icon);
            lu_theme_update_obj(locker->lu_theme, locker->info_icon_right_theme_obj);
            return obj;
        }
        break;
        default:
        break;
    }
    return NULL;
}

void lu_locker_remove_icon(lu_locker_t* locker, lu_locker_info_icon_t* info)
{
    if(!locker||!info)
    {
        return;
    }
    switch(info->type)
    {
        case LU_LOCKER_ICON_TYPE_LEFT:
        {
            lv_obj_t* obj = locker->info_icon_left;
            for(int32_t i=0;i<lv_obj_get_child_count(obj);++i)
            {
                lv_obj_t* child = lv_obj_get_child(obj, i);
                if(strcmp(lv_label_get_text(child), info->icon) == 0)
                {
                    // lu_theme_remove_obj(locker->lu_theme, child);
                    lv_obj_delete(child);
                    break;
                }
            }
        }
        break;
        case LU_LOCKER_ICON_TYPE_RIGHT:
        {
            lv_obj_t* obj = locker->info_icon_right;
            for(int32_t i=0;i<lv_obj_get_child_count(obj);++i)
            {
                lv_obj_t* child = lv_obj_get_child(obj, i);
                if(strcmp(lv_label_get_text(child), info->icon) == 0)
                {
                    // lu_theme_remove_obj(locker->lu_theme, child);
                    lv_obj_delete(child);
                    break;
                }
            }
        }
        break;
        default:
        break;
    }
}

void lu_locker_set_wallpaper(lu_locker_t* locker, const char* path)
{
    if(!locker||!path)
    {
        return;
    }
    lv_image_set_src(locker->bg, path);
    // lu_widget_image_set_src(locker->bg, path);
}
