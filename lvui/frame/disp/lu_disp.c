#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/misc/lu_log.h"
#include "lvui/plat/lu_mem.h"
#include "lu_disp.h"
#include <math.h>
#include <string.h>

#define STAND_DPI 160

// typedef struct lu_disp_t
// {
//     lu_disp_t info;
//     // float dpi;
//     // lv_obj_t* contain;
// }lu_disp_t;

float lu_disp_calc_dpi(int32_t hor, int32_t ver, float size);
float lu_disp_px2dp(float dpi, float px);
float lu_disp_dp2px(float dpi, float dp);
lu_disp_t* lu_disp_create(const lu_disp_t* info);
void lu_disp_delete(lu_disp_t** disp);
lv_point_t lu_disp_get_res(lu_disp_t* disp);
int32_t lu_disp_get_hor(lu_disp_t* disp);
int32_t lu_disp_get_ver(lu_disp_t* disp);
float lu_disp_get_dpi(lu_disp_t* disp);
lv_obj_t* lu_disp_get_contain(lu_disp_t* disp);
float lu_disp_get_default_dpi();

float lu_disp_calc_dpi(int32_t hor, int32_t ver, float size)
{
    if(size&&hor&&ver)
    {
        return sqrt(pow(hor,2)+pow(ver,2))/size;
    }else
    {
        return 0;
    }
}

float lu_disp_px2dp(float dpi, float px)
{
    return px*dpi/STAND_DPI;
}

float lu_disp_dp2px(float dpi, float dp)
{
    return dp*STAND_DPI/dpi;
}

lu_disp_t* lu_disp_create(const lu_disp_t* info)
{
    if(!info)
    {
        return NULL;
    }
    if(!info->contain || !info->dpi)
    {
        return NULL;
    }
    lu_disp_t* disp = (lu_disp_t*)lu_calloc(1, sizeof(lu_disp_t));
    if(!disp)
    {
        return NULL;
    }
    memcpy(disp, info, sizeof(lu_disp_t));
    // disp->dpi = get_dpi( disp->res.x, disp->res.y, disp->size);
    return disp;
}

void lu_disp_delete(lu_disp_t** disp)
{
    if(!disp || !*disp)
    {
        return;
    }
    lv_obj_delete(lu_disp_get_contain(*disp));
    lu_free((*disp));
    *disp = NULL;
}

lv_point_t lu_disp_get_res(lu_disp_t* disp)
{
    if(!disp)
    {
        return (lv_point_t){0,0};
    }
    return (lv_point_t){lv_obj_get_width(disp->contain), lv_obj_get_height(disp->contain)};
}

int32_t lu_disp_get_hor(lu_disp_t* disp)
{
    if(!disp)
    {
        return 0;
    }
    return lv_obj_get_width(disp->contain);
}

int32_t lu_disp_get_ver(lu_disp_t* disp)
{
    if(!disp)
    {
        return 0;
    }
    return lv_obj_get_height(disp->contain);
}

float lu_disp_get_dpi(lu_disp_t* disp)
{
    if(!disp)
    {
        return 0;
    }
    return disp->dpi;
}

lv_obj_t* lu_disp_get_contain(lu_disp_t* disp)
{
    if(!disp)
    {
        return NULL;
    }
    return disp->contain;
}

float lu_disp_get_default_dpi()
{
    return STAND_DPI;
}
