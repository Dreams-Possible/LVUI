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

#include "lvui/view/lu_desktop.h"
// #include "lvui/view/lu_statbar.h"
#include "lvui/view/lu_gesture.h"
#include "lvui/view/lu_ctrbar.h"
#include "lvui/view/lu_locker.h"
#include "src/core/lv_obj_style_gen.h"
#include "src/misc/lv_color.h"

#include "lu_launcher.h"

#include <string.h>

typedef struct lu_app_t
{
    lu_launcher_t* launcher;
    lu_app_info_t app_info;
    lv_obj_t* app_contain;
    lv_obj_t* app_item;
    lu_theme_obj_t* app_item_theme_obj;
    lu_app_cb_t app_cb;
    void* app_user_data;
    bool is_opened;
}lu_app_t;

typedef struct lu_launcher_t
{
    lu_disp_t* lu_disp;
    lu_theme_t* lu_theme;
    lu_touch_t* lu_touch;
    lv_obj_t* parent;
    lu_desktop_t* desktop;
    lu_ctrbar_t* ctrbar;
    lu_gesture_t* gesture;
    lu_locker_t* locker;
    lv_obj_t* contain;
    lv_obj_t* bg;
    lv_obj_t* app_desktop_contain;
    lv_obj_t* app_desktop;
    lu_layout_t* app_layout;
    int32_t app_gap;
    int32_t app_square;
    lv_obj_t* app_contain;
    lu_sll_t* app_list;
    lu_anim_t* app_desktop_contain_anim;
    lv_timer_t* timer;

    bool is_pressed;
    int32_t active_app_num;

    lu_launcher_double_click_cb_t cb[LU_LAUNCHER_EVENT_SUMNUM];
    void* user_data[LU_LAUNCHER_EVENT_SUMNUM];
}lu_launcher_t;

static void launcher_event_cb(lu_launcher_t* launcher, lu_launcher_event_t event);
static void app_desktop_contain_event_cb(lv_event_t *event);
static void launcher_timer_cb(lv_timer_t* timer);
lu_launcher_t* lu_launcher_create(const lu_launcher_info_t* launcher_info);
void lu_launcher_delete(lu_launcher_t** launcher);
lv_obj_t* lu_launcher_get_app_contain(lu_launcher_t* launcher);
static void app_event_cb(lv_event_t* event);
lu_app_t* lu_launcher_add_app(lu_launcher_t* launcher, const lu_app_info_t* app_info);
void lu_launcher_remove_app(lu_launcher_t* launcher, lu_app_t** app);
// void lu_launcher_set_app_user_data(lu_launcher_t* launcher, lu_app_t* app, void* user_data);
void lu_launcher_set_app_cb(lu_app_t* app, lu_app_cb_t cb);
void* lu_launcher_get_app_user_data(lu_launcher_t* launcher, lu_app_t* app);
static void app_open_anim(lu_launcher_t* launcher, lu_app_t* app);
static void app_close_anim_finish(lv_anim_t* anim);
static void app_close_anim(lu_launcher_t* launcher, lu_app_t* app);
void lu_launcher_open_app(lu_launcher_t* launcher, lu_app_t* app);
void lu_launcher_close_app(lu_launcher_t* launcher, lu_app_t* app);
void lu_launcher_close_all(lu_launcher_t* launcher);
void lu_launcher_set_cb(lu_launcher_t* launcher, lu_launcher_event_t event, lu_launcher_double_click_cb_t cb, void* user_data);
void* lu_launcher_get_user_data(lu_launcher_t* launcher, lu_launcher_event_t event);
int32_t lu_launcher_get_active_app_num(lu_launcher_t* launcher);
void lu_launcher_set_wallpaper(lu_launcher_t* launcher, const char* path);

static void launcher_event_cb(lu_launcher_t* launcher, lu_launcher_event_t event)
{
    if(launcher->cb[event])
    {
        launcher->cb[event](launcher);
    }
    // LU_LOG("double_click_cb: success");
}

static void app_desktop_contain_event_cb(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lu_launcher_t* launcher = (lu_launcher_t*)lv_event_get_user_data(event);
    switch(code)
    {
        case LV_EVENT_PRESSED:
            launcher->is_pressed = true;
            // lu_anim_set_value(gesture->bar_mask_anim, lu_gesture_get_height(gesture)-lu_touch_get_moved_y(gesture->lu_touch));
        break;
        case LV_EVENT_RELEASED:
            launcher->is_pressed = false;
            if(lu_touch_get_moved_y(launcher->lu_touch)>lu_disp_get_ver(launcher->lu_disp)*0.4)
            {
                launcher_event_cb(launcher, LU_LAUNCHER_SWIPE_DOWN);
            }else
            if(lu_touch_get_double_click(launcher->lu_touch))
            {
                // LU_LOG("double_click_cb: success");
                launcher_event_cb(launcher, LU_LAUNCHER_DOUBLE_CLICK);
            }
            // LU_LOG("released_time = %d, last_released_time = %d", released_time, last_released_time);
            // lu_anim_set_value(gesture->bar_mask_anim, lu_gesture_get_height(gesture));
        break;
        default:
        break;
    }
}

static void launcher_timer_cb(lv_timer_t* timer)
{
    lu_launcher_t* launcher = (lu_launcher_t*)lv_timer_get_user_data(timer);
    if(lu_locker_get_status(launcher->locker))
    {
        return;
    }
    if(lv_obj_get_height(launcher->app_desktop)>lv_obj_get_height(launcher->app_desktop_contain))
    {
        return;
    }
    // if(lu_touch_get_state(launcher->lu_touch))
    // {
    //     int32_t y = lu_touch_get_press_y(launcher->lu_touch);
    //     if(y>lu_ctrbar_get_height(launcher->ctrbar)&&y<lu_disp_get_ver(launcher->lu_disp)-lu_gesture_get_height(launcher->gesture)+launcher->app_gap)
    //     {
    //         lu_anim_set_value(launcher->app_desktop_contain_anim, lu_touch_get_moved_y(launcher->lu_touch));
    //     }
    //     // LU_LOG("y = %d, ctrbar=%d", y, lu_ctrbar_get_height(launcher->ctrbar));
    // }else
    // {
    //     lu_anim_set_value(launcher->app_desktop_contain_anim, 0);
    // }
    if(launcher->is_pressed)
    {
        if(lu_touch_get_long_press(launcher->lu_touch))
        {
            launcher_event_cb(launcher, LU_LAUNCHER_LONG_PRESS);
        }
        lu_anim_set_value(launcher->app_desktop_contain_anim, lu_touch_get_moved_y(launcher->lu_touch));
    }else
    {
        lu_anim_set_value(launcher->app_desktop_contain_anim, 0);
    }
}

lu_launcher_t* lu_launcher_create(const lu_launcher_info_t* launcher_info)
{
    if(!launcher_info)
    {
        return NULL;
    }
    lu_launcher_t* launcher = (lu_launcher_t*)lu_calloc(1, sizeof(lu_launcher_t));
    if(!launcher)
    {
        return NULL;
    }
    launcher->lu_disp = launcher_info->lu_disp;
    launcher->lu_theme = launcher_info->lu_theme;
    launcher->lu_touch = launcher_info->lu_touch;
    launcher->parent = launcher_info->parent;
    launcher->desktop = launcher_info->desktop;
    launcher->ctrbar = launcher_info->ctrbar;
    launcher->gesture = launcher_info->gesture;
    launcher->locker = launcher_info->locker;
    lv_obj_t* obj = lu_widget_obj_init(launcher->parent);
    if(!obj)
    {
        lu_launcher_delete(&launcher);
        return NULL;
    }
    launcher->contain = obj;
    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_opa(obj, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {
        {
            // lv_obj_t* obj = lu_widget_obj_init(launcher->contain);
            lv_obj_t* obj = lu_widget_image_init(launcher->contain);
            if(!obj)
            {
                lu_launcher_delete(&launcher);
                return NULL;
            }
            launcher->bg = obj;
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            lv_obj_set_style_bg_opa(obj, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lu_theme_info_t theme_info =
            {
                .self_type = LU_THEME_TYPE_MODE,
                .flag = LU_THEME_FLAG_SELF,
            };
            lu_theme_add_obj(launcher->lu_theme, obj, &theme_info);
        }
        {
            lv_obj_t* obj = lu_widget_obj_init(launcher->contain);
            if(!obj)
            {
                lu_launcher_delete(&launcher);
                return NULL;
            }
            launcher->app_desktop_contain = obj;
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_add_event_cb(obj, app_desktop_contain_event_cb, LV_EVENT_PRESSED, launcher);
            lv_obj_add_event_cb(obj, app_desktop_contain_event_cb, LV_EVENT_RELEASED, launcher);
            {
                lv_obj_t* obj = lu_widget_obj_init(launcher->app_desktop_contain);
                if(!obj)
                {
                    lu_launcher_delete(&launcher);
                    return NULL;
                }
                launcher->app_desktop = obj;
                {
                    const int32_t ratio = 8;
                    const int32_t num = 6;
                    launcher->app_gap = lu_disp_get_hor(launcher->lu_disp) / (ratio * num + num + 1);
                    launcher->app_square = (lu_disp_get_hor(launcher->lu_disp) - launcher->app_gap * (num + 1)) / num;
                }

                // lv_obj_set_style_bg_opa(obj, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
                // lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN | LV_STATE_DEFAULT);

                lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                lv_obj_set_style_pad_hor(obj, launcher->app_gap, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_pad_top(obj, launcher->app_gap+lu_ctrbar_get_height(launcher->ctrbar), LV_PART_MAIN | LV_STATE_DEFAULT);
                lu_layout_info_t app_layout_info =
                {
                    .grid_size = {6, 64},
                    .obj_size = {launcher->app_square, launcher->app_square},
                    .obj_gap = {launcher->app_gap, launcher->app_gap},
                };
                launcher->app_layout = lu_layout_create(obj, &app_layout_info);
                if(!launcher->app_layout)
                {
                    lu_launcher_delete(&launcher);
                    return NULL;
                }
                // lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_grid_column_align(obj, LV_GRID_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                lv_obj_add_event_cb(obj, app_desktop_contain_event_cb, LV_EVENT_PRESSED, launcher);
                lv_obj_add_event_cb(obj, app_desktop_contain_event_cb, LV_EVENT_RELEASED, launcher);
            }
        }
        {
            lv_obj_t* obj = lu_widget_obj_init(launcher->contain);
            if(!obj)
            {
                lu_launcher_delete(&launcher);
                return NULL;
            }
            launcher->app_contain = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            // lv_obj_set_style_bg_opa(obj, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    launcher->app_list = lu_sll_create();
    if(!launcher->app_list)
    {
        lu_launcher_delete(&launcher);
        return NULL;
    }
    launcher->app_desktop_contain_anim = lu_anim_create(launcher->app_desktop_contain);
    if(!launcher->app_desktop_contain_anim)
    {
        lu_launcher_delete(&launcher);
        return NULL;
    }
    lu_anim_param_t param =
    {
        .kp = 0.2,
        .kd = 0,
    };
    lu_anim_set_param(launcher->app_desktop_contain_anim, &param);
    lu_anim_set_get_cb(launcher->app_desktop_contain_anim, lv_obj_get_y);
    lu_anim_set_set_cb(launcher->app_desktop_contain_anim, lv_obj_set_y);
    lu_anim_start(launcher->app_desktop_contain_anim);
    launcher->timer = lv_timer_create(launcher_timer_cb, LV_DEF_REFR_PERIOD, launcher);
    if(!launcher->timer)
    {
        lu_launcher_delete(&launcher);
        return NULL;
    }
    return launcher;
}

void lu_launcher_delete(lu_launcher_t** launcher)
{
    if(!launcher||!*launcher)
    {
        return;
    }
    if((*launcher)->timer)
    {
        lv_timer_delete((*launcher)->timer);
    }
    if((*launcher)->app_desktop_contain_anim)
    {
        lu_anim_delete(&(*launcher)->app_desktop_contain_anim);
    }
    if((*launcher)->app_list)
    {
        lu_sll_node_t* app_list = lu_sll_get_head((*launcher)->app_list);
        while(app_list)
        {
            lu_app_t* app = lu_sll_get_data(app_list);
            lu_launcher_remove_app((*launcher), &app);
            app_list = lu_sll_get_next(app_list);
        }
        lu_sll_delete(&(*launcher)->app_list);
    }
    if((*launcher)->app_layout)
    {
        lu_layout_delete(&(*launcher)->app_layout);
    }
    if((*launcher)->contain)
    {
        lu_theme_remove_obj((*launcher)->lu_theme, (*launcher)->bg);
        lv_obj_delete((*launcher)->contain);
    }
    lu_free(*launcher);
    *launcher = NULL;
}

lv_obj_t* lu_launcher_get_app_contain(lu_launcher_t* launcher)
{
    return launcher->app_contain;
}

static void app_event_cb(lv_event_t* event)
{
    // LU_LOG("app_event_cb!");
    lu_app_t* app = lv_event_get_user_data(event);
    lu_launcher_open_app(app->launcher, app);
    if(app->app_cb)
    {
        app->app_cb(app);
        // lu_launcher_open_app(launcher, &app);
    }
}

lu_app_t* lu_launcher_add_app(lu_launcher_t* launcher, const lu_app_info_t* app_info)
{
    if(!launcher||!app_info)
    {
        return NULL;
    }
    if(!app_info->app_name&&!app_info->app_icon)
    {
        return NULL;
    }
    lu_app_t* app = (lu_app_t*)lu_calloc(1, sizeof(lu_app_t));
    if(!app)
    {
        return NULL;
    }
    app->launcher = launcher;
    app->app_contain = app_info->app_contain;
    memcpy(&app->app_info, app_info, sizeof(lu_app_info_t));
    {
        lv_obj_t* obj = lu_widget_btn_init(launcher->app_desktop);
        if(!obj)
        {
            lu_free(app);
            return NULL;
        }
        app->app_item = obj;
        lv_obj_set_style_bg_opa(obj, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_all(obj, launcher->app_gap, LV_PART_MAIN | LV_STATE_DEFAULT);
        switch (app_info->app_size)
        {
            case LU_APP_SIZE_SMALL:
            {
                lv_point_t size = {1,1};
                lu_layout_set(launcher->app_layout, obj, app_info->app_pos, size);
                break;
            }
            case LU_APP_SIZE_MEDIUM:
            {
                lv_point_t size = {2,2};
                lu_layout_set(launcher->app_layout, obj, app_info->app_pos, size);
                break;
            }
            case LU_APP_SIZE_LARGE:
            {
                lv_point_t size = {4,2};
                lu_layout_set(launcher->app_layout, obj, app_info->app_pos, size);
                break;
            }
            default:
            break;
        }
        // lu_theme_add_obj(launcher->lu_theme, obj, LU_THEME_TYPE_THEME);
        lv_obj_add_event_cb(obj, app_event_cb, LV_EVENT_CLICKED, app);
        lu_theme_info_t theme_info =
        {
            .self_type = LU_THEME_TYPE_THEME,
            .child_type = LU_THEME_TYPE_INVMODE,
            .flag = LU_THEME_FLAG_ALL,
        };
        app->app_item_theme_obj = lu_theme_add_obj(launcher->lu_theme, obj, &theme_info);
        if(!app->app_item_theme_obj)
        {
            lv_obj_delete(obj);
            lu_free(app);
            return NULL;
        }
        if(app_info->app_icon)
        {
            lv_obj_t * label = lu_widget_txt_init(app->app_item,lu_font_get(lu_disp_get_dpi(launcher->lu_disp),LU_FONT_SIZE_HUGE,1));
            if(!label)
            {
                lu_theme_remove_obj(launcher->lu_theme, app->app_item);
                lv_obj_delete(obj);
                lu_free(app);
                return NULL;
            }
            lv_label_set_text(label, app_info->app_icon);
            // lu_theme_add_obj(launcher->lu_theme, label, LU_THEME_TYPE_INVMODE);
            lv_obj_set_align(label, LV_ALIGN_CENTER);
        }
        if(app_info->app_name)
        {
            if(app_info->app_size != LU_APP_SIZE_SMALL)
            {
                lv_obj_t* label = lu_widget_txt_init(app->app_item,lu_font_get(lu_disp_get_dpi(launcher->lu_disp),LU_FONT_SIZE_NORMAL,1));
                if(!label)
                {
                    lu_theme_remove_obj(launcher->lu_theme, app->app_item);
                    lv_obj_delete(obj);
                    lu_free(app);
                    return NULL;
                }
                lv_label_set_text(label, app_info->app_name);
                // lu_theme_add_obj(launcher->lu_theme, label, LU_THEME_TYPE_INVMODE);
                lv_obj_set_align(label, LV_ALIGN_BOTTOM_LEFT);
            }
        }
        lu_theme_update_obj(launcher->lu_theme, app->app_item_theme_obj);
    }
    {
        lv_obj_t* obj = app_info->app_contain;
        if(obj)
        {
            lv_obj_set_pos(obj, 0, LV_PCT(100));
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            lv_obj_set_parent(obj, launcher->app_contain);
            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            // lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
    }
    lu_sll_append(launcher->app_list, app);
    return app;
}

void lu_launcher_remove_app(lu_launcher_t* launcher, lu_app_t** app)
{
    if(!launcher||!*app)
    {
        return;
    }
    if(!(*app)->app_item)
    {
        return;
    }
    if((*app)->app_contain)
    {
        lv_obj_delete((*app)->app_contain);
    }
    lu_theme_remove_obj(launcher->lu_theme, (*app)->app_item);
    lv_obj_delete((*app)->app_item);
    // if((*app)->app_item)
    // {
    // }
    // while(lv_obj_get_child(obj, 0))
    // {
    //     lv_obj_t* child = lv_obj_get_child(obj, 0);
    //     lu_theme_remove_obj(launcher->lu_theme, child);
    //     lv_obj_delete(child);
    // }
    lu_free(*app);
    *app = NULL;
    return;
}

void lu_launcher_set_app_cb(lu_app_t* app, lu_app_cb_t cb)
{
    if(!app)
    {
        return;
    }
    app->app_cb = cb;
}

void* lu_launcher_get_app_user_data(lu_launcher_t* launcher, lu_app_t* app)
{
    if(!launcher||!app)
    {
        return NULL;
    }
    return app->app_user_data;
}

static void app_open_anim(lu_launcher_t* launcher, lu_app_t* app)
{
    if(launcher->active_app_num)
    {
        lv_obj_remove_flag(launcher->app_contain, LV_OBJ_FLAG_HIDDEN);
    }
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, app->app_contain);
    lv_anim_set_values(&anim, lu_disp_get_ver(launcher->lu_disp), 0);
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_time(&anim, 300);
    lv_anim_set_path_cb(&anim, lu_anim_lv_anim_path_cb);
    lv_anim_set_completed_cb(&anim, lu_anim_lv_anim_auto_cb);
    lv_anim_start(&anim);
}

static void app_close_anim_finish(lv_anim_t* anim)
{
    // lv_anim_delete(anim->var, anim->exec_cb);
    lu_anim_lv_anim_auto_cb(anim);
    lu_launcher_t* launcher = (lu_launcher_t*)lv_anim_get_user_data(anim);
    if(!launcher->active_app_num)
    {
        lv_obj_add_flag(launcher->app_contain, LV_OBJ_FLAG_HIDDEN);
    }
}

static void app_close_anim(lu_launcher_t* launcher, lu_app_t* app)
{
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, app->app_contain);
    lv_anim_set_values(&anim, 0, lu_disp_get_ver(launcher->lu_disp));
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_time(&anim, 300);
    lv_anim_set_path_cb(&anim, lu_anim_lv_anim_path_cb);
    lv_anim_set_completed_cb(&anim, app_close_anim_finish);
    lv_anim_set_user_data(&anim, launcher);
    lv_anim_start(&anim);
}

void lu_launcher_open_app(lu_launcher_t* launcher, lu_app_t* app)
{
    if(!launcher||!app)
    {
        return;
    }
    if(!app->app_contain)
    {
        return;
    }
    ++launcher->active_app_num;
    app->is_opened = true;
    app_open_anim(launcher, app);
}

void lu_launcher_close_app(lu_launcher_t* launcher, lu_app_t* app)
{
    if(!launcher||!app)
    {
        return;
    }
    if(!app->app_contain)
    {
        return;
    }
    --launcher->active_app_num;
    app->is_opened = false;
    app_close_anim(launcher, app);
}

void lu_launcher_close_all(lu_launcher_t* launcher)
{
    if(!launcher)
    {
        return;
    }
    if(!launcher->active_app_num)
    {
        return;
    }
    lu_sll_node_t* app_node = lu_sll_get_head(launcher->app_list);
    while(app_node)
    {
        lu_app_t* app = (lu_app_t*)lu_sll_get_data(app_node);
        if(app->is_opened)
        {
            lu_launcher_close_app(launcher, app);
        }
        app_node = lu_sll_get_next(app_node);
    }
}

void lu_launcher_set_cb(lu_launcher_t* launcher, lu_launcher_event_t event, lu_launcher_double_click_cb_t cb, void* user_data)
{
    launcher->cb[event] = cb;
    launcher->user_data[event] = user_data;
}

void* lu_launcher_get_user_data(lu_launcher_t* launcher, lu_launcher_event_t event)
{
    return launcher->user_data[event];
}

int32_t lu_launcher_get_active_app_num(lu_launcher_t* launcher)
{
    return launcher->active_app_num;
}

void lu_launcher_set_wallpaper(lu_launcher_t* launcher, const char* path)
{
    if(!launcher||!path)
    {
        return;
    }
    lv_image_set_src(launcher->bg, path);
    // lu_widget_image_set_src(launcher->bg, path);
}
