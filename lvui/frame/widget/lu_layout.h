#ifndef LU_LAYOUT_H
#define LU_LAYOUT_H

#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "src/misc/lv_area.h"

typedef struct lu_layout_info_t
{
    lv_point_t grid_size;
    lv_point_t obj_size;
    lv_point_t obj_gap;
}lu_layout_info_t;
typedef struct lu_layout_t lu_layout_t;

lu_layout_t* lu_layout_create(lv_obj_t* obj, const lu_layout_info_t* layout_info);
void lu_layout_delete(lu_layout_t** layout);
void lu_layout_set(lu_layout_t* layout, lv_obj_t* obj, lv_point_t pos, lv_point_t size);

#endif
