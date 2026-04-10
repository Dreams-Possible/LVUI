#ifndef LU_HOME_H
#define LU_HOME_H

#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/frame/disp/lu_disp.h"
#include "lvui/frame/theme/lu_theme.h"
#include "lvui/frame/input/lu_touch.h"

typedef struct lu_desktop_t lu_desktop_t;

lu_desktop_t* lu_desktop_create(lu_disp_t* lu_disp, lu_theme_t* lu_theme, lu_touch_t* lu_touch, lv_obj_t* parent);
void lu_desktop_delete(lu_desktop_t** desktop);
lv_obj_t* lu_desktop_get_contain(lu_desktop_t* desktop);

#endif
