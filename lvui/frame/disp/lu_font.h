#ifndef LU_FONT_H
#define LU_FONT_H

#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lu_disp.h"

//字体大小类型
typedef enum lu_font_size_t
{
    LU_FONT_SIZE_MIN=0,//最小字体（约8sp）
    LU_FONT_SIZE_TINY,//极小字体（约10sp）
    LU_FONT_SIZE_SMALL,//小字体（约12sp）
    LU_FONT_SIZE_NORMAL,//正常字体（约14sp）
    LU_FONT_SIZE_MEDIUM,//中等字体（约16sp）
    LU_FONT_SIZE_LARGE,//大字体（约18sp）
    LU_FONT_SIZE_XLARGE,//超大字体（约20sp）
    LU_FONT_SIZE_XXLARGE,//特大字体（约24sp）
    LU_FONT_SIZE_HUGE,//巨大字体（约28sp）
    LU_FONT_SIZE_XHUGE,//超巨大字体（约32sp）
    LU_FONT_SIZE_MAX,//最大字体（约48sp）
    LU_FONT_SIZE_COUNT
}lu_font_size_t;
typedef enum lu_font_auto_size_t
{
    LU_FONT_AUTO_SIZE_SMALL=0,
    LU_FONT_AUTO_SIZE_MEDIUM,
    LU_FONT_AUTO_SIZE_BIG,
    LU_FONT_AUTO_SIZE_LARGE,
    // LU_FONT_AUTO_SIZE_MAX,
    LU_FONT_AUTO_SIZE_COUNT
}lu_font_auto_size_t;

//获取字体（按大小和样式）
const lv_font_t*lu_font_get(float dpi, lu_font_size_t size_type, float scale);
//获取自适应字体大小
const lv_font_t*lu_font_get_auto(lu_disp_t* disp, lu_font_auto_size_t size_type);

#endif
