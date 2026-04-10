#ifndef LU_CTRBAR_H
#define LU_CTRBAR_H

#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/frame/disp/lu_disp.h"
#include "lvui/frame/theme/lu_theme.h"
#include "lvui/frame/input/lu_touch.h"

#include "lu_statbar.h"

typedef struct lu_ctrbar_t lu_ctrbar_t;

int32_t lu_ctrbar_get_height(lu_ctrbar_t* ctrbar);
lu_ctrbar_t* lu_ctrbar_create(lu_disp_t* lu_disp, lu_theme_t* lu_theme, lu_touch_t* lu_touch, lv_obj_t* parent);
void lu_ctrbar_delete(lu_ctrbar_t** ctrbar);
lv_obj_t* lu_ctrbar_get_contain(lu_ctrbar_t* ctrbar);
lv_obj_t* lu_ctrbar_get_statbar_contain(lu_ctrbar_t* ctrbar);
lu_statbar_t* lu_ctrbar_get_statbar(lu_ctrbar_t* ctrbar);

#endif
