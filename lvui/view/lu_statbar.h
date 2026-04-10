#ifndef LU_STATBAR_H
#define LU_STATBAR_H

#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/frame/disp/lu_disp.h"
#include "lvui/frame/theme/lu_theme.h"
#include "lvui/frame/input/lu_touch.h"

typedef enum lu_statbar_type_t
{
    LU_STATBAR_TYPE_LEFT = 0,
    LU_STATBAR_TYPE_RIGHT,
}lu_statbar_type_t;
typedef struct lu_statbar_info_t
{
    const char* icon;
    lu_statbar_type_t type;
}lu_statbar_info_t;
typedef struct lu_statbar_t lu_statbar_t;

// int32_t lu_statbar_get_height(lu_statbar_t* statbar);
lu_statbar_t* lu_statbar_create(lu_disp_t* lu_disp, lu_theme_t* lu_theme, lu_touch_t* lu_touch, lv_obj_t* parent);
void lu_statbar_delete(lu_statbar_t** statbar);
// void lu_statbar_set_time(lu_statbar_t* statbar, const char* hour, const char* minute, const char* second);
lv_obj_t* lu_statbar_add_icon(lu_statbar_t* statbar, lu_statbar_info_t* info);
void lu_statbar_remove_icon(lu_statbar_t* statbar, lu_statbar_info_t* info);
// lv_obj_t* lu_statbar_get_contain(lu_statbar_t* statbar);

#endif
