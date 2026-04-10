#ifndef LU_THEME_H
#define LU_THEME_H

#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"

typedef enum lu_theme_mode_t
{
    LU_THEME_MODE_BRIGHT = 0,
    LU_THEME_MODE_DARK,
}lu_theme_mode_t;
typedef enum lu_theme_type_t
{
    LU_THEME_TYPE_MODE = 0,
    LU_THEME_TYPE_INVMODE,
    LU_THEME_TYPE_THEME,
}lu_theme_type_t;
typedef enum lu_theme_flag_t
{
    LU_THEME_FLAG_SELF = 0,
    LU_THEME_FLAG_CHILD,
    LU_THEME_FLAG_ALL,
}lu_theme_flag_t;
typedef struct lu_theme_info_t
{
    lu_theme_type_t self_type;
    lu_theme_type_t child_type;
    lu_theme_flag_t flag;
}lu_theme_info_t;
typedef struct lu_theme_t lu_theme_t;
typedef struct lu_theme_obj_t lu_theme_obj_t;

lu_theme_t* lu_theme_create();
void lu_theme_delete(lu_theme_t* theme);
lu_theme_obj_t* lu_theme_add_obj(lu_theme_t* theme, lv_obj_t* obj, const lu_theme_info_t* info);
void lu_theme_remove_obj(lu_theme_t* theme, lv_obj_t* obj);
void lu_theme_set_color(lu_theme_t* theme, lv_color_t color);
lv_color_t lu_theme_get_color(lu_theme_t* theme);
void lu_theme_set_mode(lu_theme_t* theme, lu_theme_mode_t mode);
lu_theme_mode_t lu_theme_get_mode(lu_theme_t* theme);
lv_color_t lu_theme_get_bright_color(lu_theme_t* theme);
lv_color_t lu_theme_get_dark_color(lu_theme_t* theme);
void lu_theme_update_obj(lu_theme_t* theme, lu_theme_obj_t* theme_obj);

#endif
