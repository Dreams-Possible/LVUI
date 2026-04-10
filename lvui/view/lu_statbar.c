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
#include "src/misc/lv_area.h"

#include "lu_statbar.h"

#include <stdio.h>


typedef struct lu_statbar_t
{
    lu_disp_t* lu_disp;
    lu_theme_t* lu_theme;
    lu_touch_t* lu_touch;
    lv_obj_t* contain;
    // lv_obj_t* bg;
    // lv_obj_t* time;
    char time_str[8];
    lv_obj_t* icon_contain_left;
    lu_theme_obj_t* icon_contain_left_theme_obj;
    lv_obj_t* icon_contain_right;
    lu_theme_obj_t* icon_contain_right_theme_obj;
    // lu_anim_t* info_mask_anim;
    bool if_exit;
}lu_statbar_t;

// int32_t lu_statbar_get_height(lu_statbar_t* statbar);
lu_statbar_t* lu_statbar_create(lu_disp_t* lu_disp, lu_theme_t* lu_theme, lu_touch_t* lu_touch, lv_obj_t* parent);
void lu_statbar_delete(lu_statbar_t** statbar);
// void lu_statbar_set_time(lu_statbar_t* statbar, const char* hour, const char* minute, const char* second);
lv_obj_t* lu_statbar_add_icon(lu_statbar_t* statbar, lu_statbar_info_t* info);
void lu_statbar_remove_icon(lu_statbar_t* statbar, lu_statbar_info_t* info);
// lv_obj_t* lu_statbar_get_contain(lu_statbar_t* statbar);

// int32_t lu_statbar_get_height(lu_statbar_t* statbar)
// {
//     if(!statbar)
//     {
//         return 0;
//     }
//     return lu_disp_dp2px(lu_disp_get_dpi(statbar->lu_disp), 30);
// }

lu_statbar_t* lu_statbar_create(lu_disp_t* lu_disp, lu_theme_t* lu_theme, lu_touch_t* lu_touch, lv_obj_t* parent)
{
    if(!lu_disp||!lu_theme||!lu_touch||!parent)
    {
        return NULL;
    }
    lu_statbar_t* statbar = (lu_statbar_t*)lu_calloc(1, sizeof(lu_statbar_t));
    if(!statbar)
    {
        return NULL;
    }
    statbar->lu_disp = lu_disp;
    statbar->lu_theme = lu_theme;
    statbar->lu_touch = lu_touch;
    lv_obj_t* obj = lu_widget_obj_init(parent);
    if(!obj)
    {
        lu_statbar_delete(&statbar);
        return NULL;
    }
    statbar->contain = obj;
    // lv_obj_set_size(obj, lu_disp_get_hor(statbar->lu_disp), lu_statbar_get_height(statbar));
    lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
    
    // lv_obj_set_style_bg_opa(obj, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_pad_hor(obj, lu_disp_get_hor(statbar->lu_disp)*0.05, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_align(obj, LV_ALIGN_CENTER);
    // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {
        // lv_obj_t* obj = lu_widget_obj_init(statbar->contain);
        // if(!obj)
        // {
        //     lu_statbar_delete(&statbar);
        //     return NULL;
        // }
        // statbar->bg = obj;
        // lv_obj_set_size(obj, lu_disp_get_hor(statbar->lu_disp), lu_statbar_get_height(statbar));
        // // lv_obj_set_style_bg_opa(obj, LV_OPA_30, LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        // lu_theme_add_obj(statbar->lu_theme, obj, LU_THEME_TYPE_MODE);
        // {
        //     lv_obj_t* obj = lu_widget_txt_init(statbar->contain, lu_font_get(lu_disp_get_dpi(statbar->lu_disp), LU_FONT_SIZE_NORMAL, 1));
        //     if(!obj)
        //     {
        //         lu_statbar_delete(&statbar);
        //         return NULL;
        //     }
        //     statbar->time = obj;
        //     lv_obj_set_align(obj, LV_ALIGN_CENTER);
        //     lu_theme_add_obj(statbar->lu_theme, obj, LU_THEME_TYPE_INVMODE);
        // }
        {
            lv_obj_t* obj = lu_widget_obj_init(statbar->contain);
            if(!obj)
            {
                lu_statbar_delete(&statbar);
                return NULL;
            }
            statbar->icon_contain_left = obj;
            lv_obj_set_size(obj, LV_PCT(50), LV_SIZE_CONTENT);
            lv_obj_set_align(obj, LV_ALIGN_LEFT_MID);
            lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            // lu_theme_add_obj(statbar->lu_theme, obj, LU_THEME_TYPE_DARK);
            lu_theme_info_t info = 
            {
                .child_type = LU_THEME_TYPE_INVMODE,
                .flag = LU_THEME_FLAG_CHILD,
            };
            statbar->icon_contain_left_theme_obj = lu_theme_add_obj(statbar->lu_theme, obj, &info);
            if(!statbar->icon_contain_left_theme_obj)
            {
                lu_statbar_delete(&statbar);
                return NULL;
            }
        }
        {
            lv_obj_t* obj = lu_widget_obj_init(statbar->contain);
            if(!obj)
            {
                lu_statbar_delete(&statbar);
                return NULL;
            }
            statbar->icon_contain_right = obj;

            // lv_obj_set_style_bg_opa(obj, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_obj_set_size(obj, LV_PCT(50), LV_SIZE_CONTENT);
            lv_obj_set_align(obj, LV_ALIGN_RIGHT_MID);
            lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW_REVERSE);
            lv_obj_set_flex_align(obj,
                      LV_FLEX_ALIGN_END,
                      LV_FLEX_ALIGN_START,
                      LV_FLEX_ALIGN_START);
            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            // lu_theme_add_obj(statbar->lu_theme, obj, LU_THEME_TYPE_DARK);
            lu_theme_info_t info = 
            {
                .child_type = LU_THEME_TYPE_INVMODE,
                .flag = LU_THEME_FLAG_CHILD,
            };
            statbar->icon_contain_right_theme_obj = lu_theme_add_obj(statbar->lu_theme, obj, &info);
            if(!statbar->icon_contain_right_theme_obj)
            {
                lu_statbar_delete(&statbar);
                return NULL;
            }
        }
    }
    return statbar;
}

void lu_statbar_delete(lu_statbar_t** statbar)
{
    if(!statbar||!*statbar)
    {
        return;
    }
    if((*statbar)->icon_contain_left)
    {
        lu_theme_remove_obj((*statbar)->lu_theme, (*statbar)->icon_contain_left);
        // lv_obj_delete((*statbar)->icon_contain_left);
    }
    if((*statbar)->icon_contain_right)
    {
        lu_theme_remove_obj((*statbar)->lu_theme, (*statbar)->icon_contain_right);
        // lv_obj_delete((*statbar)->info_icon_right);
    }
    if((*statbar)->contain)
    {
        // lu_theme_remove_obj((*statbar)->lu_theme, (*statbar)->bg);
        // lu_theme_remove_obj((*statbar)->lu_theme, (*statbar)->time);
        // lu_theme_remove_obj((*statbar)->lu_theme, (*statbar)->icon_contain_left);
        // lu_theme_remove_obj((*statbar)->lu_theme, (*statbar)->icon_contain_right);
        lv_obj_delete((*statbar)->contain);
    }
    lu_free(*statbar);
    *statbar = NULL;
}

// void lu_statbar_set_time(lu_statbar_t* statbar, const char* hour, const char* minute, const char* second)
// {
//     if(!statbar)
//     {
//         return;
//     }
//     memset(statbar->time_str, 0, sizeof(statbar->time_str));
//     snprintf(statbar->time_str, sizeof(statbar->time_str), "%s:%s:%s", hour, minute, second);
//     lv_label_set_text(statbar->time, statbar->time_str);
// }

lv_obj_t* lu_statbar_add_icon(lu_statbar_t* statbar, lu_statbar_info_t* info)
{
    if(!statbar||!info)
    {
        return NULL;
    }
    switch(info->type)
    {
        case LU_STATBAR_TYPE_LEFT:
        {
            lv_obj_t* obj = lu_widget_txt_init(statbar->icon_contain_left, lu_font_get(lu_disp_get_dpi(statbar->lu_disp), LU_FONT_SIZE_NORMAL, 1));
            if(!obj)
            {
                return NULL;
            }
            lv_label_set_text(obj, info->icon);
            lu_theme_update_obj(statbar->lu_theme, statbar->icon_contain_left_theme_obj);
            return obj;
        }
        break;
        case LU_STATBAR_TYPE_RIGHT:
        {
            lv_obj_t* obj = lu_widget_txt_init(statbar->icon_contain_right, lu_font_get(lu_disp_get_dpi(statbar->lu_disp), LU_FONT_SIZE_NORMAL, 1));
            if(!obj)
            {
                return NULL;
            }
            lv_label_set_text(obj, info->icon);
            lu_theme_update_obj(statbar->lu_theme, statbar->icon_contain_right_theme_obj);
            return obj;
        }
        break;
        default:
        break;
    }
    return NULL;
}

void lu_statbar_remove_icon(lu_statbar_t* statbar, lu_statbar_info_t* info)
{
    if(!statbar||!info)
    {
        return;
    }
    switch(info->type)
    {
        case LU_STATBAR_TYPE_LEFT:
        {
            lv_obj_t* obj = statbar->icon_contain_left;
            for(int32_t i=0;i<lv_obj_get_child_count(obj);++i)
            {
                lv_obj_t* child = lv_obj_get_child(obj, i);
                if(strcmp(lv_label_get_text(child), info->icon) == 0)
                {
                    // lu_theme_remove_obj(statbar->lu_theme, child);
                    lv_obj_delete(child);
                    break;
                }
            }
        }
        break;
        case LU_STATBAR_TYPE_RIGHT:
        {
            lv_obj_t* obj = statbar->icon_contain_right;
            for(int32_t i=0;i<lv_obj_get_child_count(obj);++i)
            {
                lv_obj_t* child = lv_obj_get_child(obj, i);
                if(strcmp(lv_label_get_text(child), info->icon) == 0)
                {
                    // lu_theme_remove_obj(statbar->lu_theme, child);
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

// lv_obj_t* lu_statbar_get_contain(lu_statbar_t* statbar)
// {
//     if(!statbar)
//     {
//         return NULL;
//     }
//     return statbar->contain;
// }
