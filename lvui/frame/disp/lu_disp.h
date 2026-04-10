#ifndef LU_DISP_H
#define LU_DISP_H

#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"

typedef struct lu_disp_t
{
    // int32_t (*get_hor)(void);
    // int32_t (*get_ver)(void);
    lv_obj_t* contain;
    float dpi;
}lu_disp_t;

// typedef struct lu_disp_t lu_disp_t;

float lu_disp_calc_dpi(int32_t hor, int32_t ver, float size);
float lu_disp_px2dp(float dpi, float px);
float lu_disp_dp2px(float dpi, float dp);
lu_disp_t* lu_disp_create(const lu_disp_t* info);
void lu_disp_delete(lu_disp_t** disp);
// lv_point_t lu_disp_get_res(lu_disp_t* disp);
int32_t lu_disp_get_hor(lu_disp_t* disp);
int32_t lu_disp_get_ver(lu_disp_t* disp);
float lu_disp_get_dpi(lu_disp_t* disp);
lv_obj_t* lu_disp_get_contain(lu_disp_t* disp);
float lu_disp_get_default_dpi();

#endif
