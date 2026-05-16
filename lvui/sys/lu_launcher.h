#ifndef LU_LAUNCHER_H
#define LU_LAUNCHER_H

#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/frame/disp/lu_disp.h"
#include "lvui/frame/theme/lu_theme.h"
#include "lvui/frame/input/lu_touch.h"

#include "lvui/view/lu_desktop.h"
// #include "lvui/view/lu_statbar.h"
#include "lvui/view/lu_gesture.h"
#include "lvui/view/lu_ctrbar.h"
#include "lvui/view/lu_locker.h"

typedef enum
{
    LU_LAUNCHER_SWIPE_DOWN =0,
    LU_LAUNCHER_LONG_PRESS,
    LU_LAUNCHER_DOUBLE_CLICK,
    LU_LAUNCHER_EVENT_SUMNUM,
}lu_launcher_event_t;
typedef struct lu_app_t lu_app_t;
typedef struct lu_launcher_t lu_launcher_t;
typedef void (*lu_app_cb_t)(lu_app_t* app);
typedef void (*lu_launcher_double_click_cb_t)(lu_launcher_t* launcher);
typedef enum lu_app_size_t
{
    LU_APP_SIZE_SMALL = 0,
    LU_APP_SIZE_MEDIUM,
    LU_APP_SIZE_LARGE,
}lu_app_size_t;
typedef struct lu_app_info_t
{
    lv_obj_t* app_contain;
    const char* app_name;
    const char* app_icon;
    lu_app_size_t app_size;
    lv_point_t app_pos;
}lu_app_info_t;
typedef struct lu_launcher_info_t
{
    lu_disp_t* lu_disp;
    lu_theme_t* lu_theme;
    lu_touch_t* lu_touch;
    lv_obj_t* parent;
    lu_desktop_t* desktop;
    lu_ctrbar_t* ctrbar;
    lu_gesture_t* gesture;
    lu_locker_t* locker;
}lu_launcher_info_t;

lu_launcher_t* lu_launcher_create(const lu_launcher_info_t* launcher_info);
void lu_launcher_delete(lu_launcher_t** home);
// lv_obj_t* lu_launcher_get_app_contain(lu_launcher_t* home);
lu_app_t* lu_launcher_add_app(lu_launcher_t* home, const lu_app_info_t* app_info);
void lu_launcher_remove_app(lu_launcher_t* home, lu_app_t** app);
void lu_launcher_set_app_cb(lu_app_t* app, lu_app_cb_t cb);
void* lu_launcher_get_app_user_data(lu_launcher_t* home, lu_app_t* app);
void lu_launcher_open_app(lu_launcher_t* launcher, lu_app_t* app);
void lu_launcher_close_app(lu_launcher_t* launcher, lu_app_t* app);
void lu_launcher_close_all(lu_launcher_t* launcher);
void lu_launcher_set_cb(lu_launcher_t* launcher, lu_launcher_event_t event, lu_launcher_double_click_cb_t cb, void* user_data);
void* lu_launcher_get_user_data(lu_launcher_t* launcher, lu_launcher_event_t event);
int32_t lu_launcher_get_active_app_num(lu_launcher_t* launcher);
void lu_launcher_set_wallpaper(lu_launcher_t* launcher, const void* src);

#endif
