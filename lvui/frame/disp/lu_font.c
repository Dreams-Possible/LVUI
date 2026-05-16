#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/misc/lu_log.h"
#include "lu_font.h"

//字体大小映射表
static const float font_size_map[LU_FONT_SIZE_COUNT]=
{
    8.0f,//LU_FONT_SIZE_MIN
    10.0f,//LU_FONT_SIZE_TINY
    12.0f,//LU_FONT_SIZE_SMALL
    14.0f,//LU_FONT_SIZE_NORMAL
    16.0f,//LU_FONT_SIZE_MEDIUM
    18.0f,//LU_FONT_SIZE_LARGE
    20.0f,//LU_FONT_SIZE_XLARGE
    24.0f,//LU_FONT_SIZE_XXLARGE
    28.0f,//LU_FONT_SIZE_HUGE
    32.0f,//LU_FONT_SIZE_XHUGE
    48.0f//LU_FONT_SIZE_MAX
};

//字体映射表
static const lv_font_t* const font_map[LU_FONT_SIZE_COUNT]=
{
    #if LV_FONT_MONTSERRAT_8
        &lv_font_montserrat_8,//LU_FONT_SIZE_MIN
    #else
        NULL,
    #endif
    #if LV_FONT_MONTSERRAT_10
        &lv_font_montserrat_10,//LU_FONT_SIZE_TINY
    #else
        NULL,
    #endif
    #if LV_FONT_MONTSERRAT_12
        &lv_font_montserrat_12,//LU_FONT_SIZE_SMALL
    #else
        NULL,
    #endif
    #if LV_FONT_MONTSERRAT_14
        &lv_font_montserrat_14,//LU_FONT_SIZE_NORMAL
    #else
        NULL,
    #endif
    #if LV_FONT_MONTSERRAT_16
        &lv_font_montserrat_16,//LU_FONT_SIZE_MEDIUM
    #else
        NULL,
    #endif
    #if LV_FONT_MONTSERRAT_18
        &lv_font_montserrat_18,//LU_FONT_SIZE_LARGE
    #else
        NULL,
    #endif
    #if LV_FONT_MONTSERRAT_20
        &lv_font_montserrat_20,//LU_FONT_SIZE_XLARGE
    #else
        NULL,
    #endif
    #if LV_FONT_MONTSERRAT_24
        &lv_font_montserrat_24,//LU_FONT_SIZE_XXLARGE
    #else
        NULL,
    #endif
    #if LV_FONT_MONTSERRAT_28
        &lv_font_montserrat_28,//LU_FONT_SIZE_HUGE
    #else
        NULL,
    #endif
    #if LV_FONT_MONTSERRAT_32
        &lv_font_montserrat_32,//LU_FONT_SIZE_XHUGE
    #else
        NULL,
    #endif
    #if LV_FONT_MONTSERRAT_48
        &lv_font_montserrat_48//LU_FONT_SIZE_MAX
    #else
        NULL,
    #endif
};

const lv_font_t*lu_font_get(float dpi, lu_font_size_t size_type, float scale);
//获取自适应字体大小
const lv_font_t*lu_font_get_auto(lu_disp_t* disp, lu_font_auto_size_t size_type);

const lv_font_t*lu_font_get(float dpi, lu_font_size_t size_type, float scale)
{
    if(size_type>=LU_FONT_SIZE_COUNT||!dpi||!scale)
    {
        return NULL;
    }
    float px=lu_disp_dp2px(dpi, font_size_map[size_type]*scale);
    lu_font_size_t size=LU_FONT_SIZE_NORMAL;
    if(px<=font_size_map[LU_FONT_SIZE_MIN])
    {
        size=LU_FONT_SIZE_MIN;
    }else if(px<=font_size_map[LU_FONT_SIZE_TINY])
    {
        size=LU_FONT_SIZE_TINY;
    }else if(px<=font_size_map[LU_FONT_SIZE_SMALL])
    {
        size=LU_FONT_SIZE_SMALL;
    }else if(px<=font_size_map[LU_FONT_SIZE_NORMAL])
    {
        size=LU_FONT_SIZE_NORMAL;
    }else if(px<=font_size_map[LU_FONT_SIZE_MEDIUM])
    {
        size=LU_FONT_SIZE_MEDIUM;
    }else if(px<=font_size_map[LU_FONT_SIZE_LARGE])
    {
        size=LU_FONT_SIZE_LARGE;
    }else if(px<=font_size_map[LU_FONT_SIZE_XLARGE])
    {
        size=LU_FONT_SIZE_XLARGE;
    }else if(px<=font_size_map[LU_FONT_SIZE_XXLARGE])
    {
        size=LU_FONT_SIZE_XXLARGE;
    }else if(px<=font_size_map[LU_FONT_SIZE_HUGE])
    {
        size=LU_FONT_SIZE_HUGE;
    }else if(px<=font_size_map[LU_FONT_SIZE_XHUGE])
    {
        size=LU_FONT_SIZE_XHUGE;
    }else if(px<=font_size_map[LU_FONT_SIZE_MAX])
    {
        size=LU_FONT_SIZE_MAX;
    }else
    {
        size=LU_FONT_SIZE_MAX;
    }
    return font_map[size];
}

//获取自适应字体大小
const lv_font_t*lu_font_get_auto(lu_disp_t* disp, lu_font_auto_size_t size_type)
{
    uint32_t size=lv_obj_get_width(disp->contain)*lv_obj_get_height(disp->contain);
    if(size>320*240)
    {
        switch (size_type)
        {
            case LU_FONT_AUTO_SIZE_SMALL:
                return lu_font_get(disp->dpi,LU_FONT_SIZE_NORMAL,disp->scale);
            case LU_FONT_AUTO_SIZE_MEDIUM:
                return lu_font_get(disp->dpi,LU_FONT_SIZE_MEDIUM,disp->scale);
            case LU_FONT_AUTO_SIZE_BIG:
                return lu_font_get(disp->dpi,LU_FONT_SIZE_HUGE,disp->scale);
            case LU_FONT_AUTO_SIZE_LARGE:
                return lu_font_get(disp->dpi,LU_FONT_SIZE_MAX,disp->scale);
            // case LU_FONT_AUTO_SIZE_MAX:
            //     return lu_font_get(disp->dpi,LU_FONT_SIZE_MAX,disp->scale);
            default:
                return LV_FONT_DEFAULT;
        }
    }else
    {
        switch (size_type)
        {
            case LU_FONT_AUTO_SIZE_SMALL:
                return lu_font_get(disp->dpi,LU_FONT_SIZE_NORMAL-1,disp->scale);
            case LU_FONT_AUTO_SIZE_MEDIUM:
                return lu_font_get(disp->dpi,LU_FONT_SIZE_MEDIUM-2,disp->scale);
            case LU_FONT_AUTO_SIZE_BIG:
                return lu_font_get(disp->dpi,LU_FONT_SIZE_HUGE-2,disp->scale);
            case LU_FONT_AUTO_SIZE_LARGE:
                return lu_font_get(disp->dpi,LU_FONT_SIZE_MAX-1,disp->scale);
            default:
                return LV_FONT_DEFAULT;
        }
    }
}
