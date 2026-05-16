#ifndef LU_LOCKER_H
#define LU_LOCKER_H

#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/frame/disp/lu_disp.h"
#include "lvui/frame/theme/lu_theme.h"
#include "lvui/frame/input/lu_touch.h"

typedef enum
{
    LU_LOCKER_EVENT_ENTER = 0,
    LU_LOCKER_EVENT_EXIT,
    LU_LOCKER_EVENT_LONG_PRESS,
    LU_LOCKER_EVENT_DOUBLE_CLICK,
    LU_LOCKER_EVENT_SUMNUM,
}lu_locker_event_t;
typedef enum lu_locker_icon_type_t
{
    LU_LOCKER_ICON_TYPE_LEFT = 0,
    LU_LOCKER_ICON_TYPE_RIGHT,
}lu_locker_icon_type_t;
typedef struct lu_locker_info_icon_t
{
    const char* icon;
    lu_locker_icon_type_t type;
}lu_locker_info_icon_t;
typedef struct lu_locker_info_txt_t
{
    const char* txt;
    const lv_font_t* font;
}lu_locker_info_txt_t;
typedef struct lu_locker_t lu_locker_t;
typedef void (*lu_locker_cb_t)(lu_locker_t* locker);

lu_locker_t* lu_locker_create(lu_disp_t* lu_disp, lu_theme_t* theme, lu_touch_t* lu_touch, lv_obj_t* parent);
void lu_locker_delete(lu_locker_t** locker);
void lu_locker_set_time(lu_locker_t* locker, const char* hour, const char* minute, const char* second);
void lu_locker_set_date(lu_locker_t* locker, const char* day, const char* month, const char* year);
void lu_locker_enter(lu_locker_t* locker);
void lu_locker_exit(lu_locker_t* locker);
bool lu_locker_get_status(lu_locker_t* locker);
void lu_locker_set_cb(lu_locker_t* locker, lu_locker_event_t event, lu_locker_cb_t cb, void* user_data);
void* lu_locker_get_user_data(lu_locker_t* locker, lu_locker_event_t event);
lv_obj_t* lu_locker_add_info_txt(lu_locker_t* locker,lu_locker_info_txt_t* info);
void lu_locker_remove_info_txt(lu_locker_t* locker, lu_locker_info_txt_t* info);
lv_obj_t* lu_locker_add_info_icon(lu_locker_t* locker, lu_locker_info_icon_t* info);
void lu_locker_remove_info_icon(lu_locker_t* locker, lu_locker_info_icon_t* info);
void lu_locker_set_wallpaper(lu_locker_t* locker, const void* src);

#endif
