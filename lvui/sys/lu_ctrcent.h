#ifndef LU_CTRCENTER_H
#define LU_CTRCENTER_H

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

typedef struct lu_ctrbtn_t lu_ctrbtn_t;
typedef struct lu_ctrcent_t lu_ctrcent_t;
typedef void (*lu_ctrbtn_cb_t)(lu_ctrbtn_t* ctrbtn);
typedef enum lu_ctrbtn_size_t
{
    LU_CTRBTN_SIZE_SMALL = 0,
    LU_CTRBTN_SIZE_MEDIUM,
    LU_CTRBTN_SIZE_LARGE,
}lu_ctrbtn_size_t;
typedef struct lu_ctrbtn_info_t
{
    lv_obj_t* ctrbtn_contain;
    const char* ctrbtn_name;
    const char* ctrbtn_icon;
    lu_ctrbtn_size_t ctrbtn_size;
    lv_point_t ctrbtn_pos;
}lu_ctrbtn_info_t;
typedef struct lu_ctrcent_info_t
{
    lu_disp_t* lu_disp;
    lu_theme_t* lu_theme;
    lu_touch_t* lu_touch;
    lv_obj_t* parent;
    lu_desktop_t* desktop;
    lu_ctrbar_t* ctrbar;
    lu_gesture_t* gesture;
    lu_locker_t* locker;
}lu_ctrcent_info_t;

lu_ctrcent_t* lu_ctrcent_create(const lu_ctrcent_info_t* ctrcent_info);
void lu_ctrcent_delete(lu_ctrcent_t** ctrcent);
void lu_ctrcent_add_ctrbtn_cb(lu_ctrbtn_t* ctrbtn, lu_ctrbtn_cb_t cb, void* user_data);
void* lu_ctrcent_get_ctrbtn_user_data(lu_ctrbtn_t* ctrbtn);
lu_ctrbtn_t* lu_ctrcent_add_ctrbtn(lu_ctrcent_t* ctrcent, const lu_ctrbtn_info_t* ctrbtn_info);
void lu_ctrcent_remove_ctrbtn(lu_ctrcent_t* ctrcent, lu_ctrbtn_t** ctrbtn);
float lu_ctrcent_get_ctrbtn_status(lu_ctrbtn_t* ctrbtn);
void lu_ctrcent_set_ctrbtn_status(lu_ctrbtn_t* ctrbtn, float status);

#endif
