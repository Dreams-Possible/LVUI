#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/misc/lu_log.h"
#include "lvui/plat/lu_mem.h"
#include "lu_layout.h"

typedef struct lu_layout_t
{
    lv_obj_t* parent;
    int32_t* col_dsc;
    int32_t* row_dsc;
}lu_layout_t;

lu_layout_t* lu_layout_create(lv_obj_t* obj, const lu_layout_info_t* layout_info);
void lu_layout_delete(lu_layout_t** layout);
void lu_layout_set(lu_layout_t* layout, lv_obj_t* obj, lv_point_t pos, lv_point_t size);

lu_layout_t* lu_layout_create(lv_obj_t* obj, const lu_layout_info_t* layout_info)
{
    if(!obj || !layout_info)
    {
        return NULL;
    }
    lu_layout_t* layout=(lu_layout_t*)lu_calloc(1,sizeof(lu_layout_t));
    if(!layout)
    {
        return NULL;
    }
    layout->parent=obj;
    layout->col_dsc=(int32_t*)lu_calloc(layout_info->grid_size.x+1,sizeof(int32_t));
    if(!layout->col_dsc)
    {
        lu_layout_delete(&layout);
        return NULL;
    }
    for(int32_t i=0; i<layout_info->grid_size.x; ++i)
    {
        layout->col_dsc[i]=layout_info->obj_size.x;
    }
    layout->col_dsc[layout_info->grid_size.x]=LV_GRID_TEMPLATE_LAST;
    layout->row_dsc=(int32_t*)lu_calloc(layout_info->grid_size.y+1,sizeof(int32_t));
    if(!layout->row_dsc)
    {
        lu_layout_delete(&layout);
        return NULL;
    }
    for(int32_t i=0; i<layout_info->grid_size.y; ++i)
    {
        layout->row_dsc[i]=layout_info->obj_size.y;
    }
    layout->row_dsc[layout_info->grid_size.y]=LV_GRID_TEMPLATE_LAST;
    lv_obj_set_style_pad_row(obj, layout_info->obj_gap.x, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(obj, layout_info->obj_gap.y, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_grid_dsc_array(obj, layout->col_dsc, layout->row_dsc);
    return layout;
}

void lu_layout_delete(lu_layout_t** layout)
{
    if(!layout || !*layout)
    {
        return;
    }
    if((*layout)->col_dsc)
    {
        lu_free((*layout)->col_dsc);
    }
    if((*layout)->row_dsc)
    {
        lu_free((*layout)->row_dsc);
    }
    lu_free(*layout);
    *layout=NULL;
}

void lu_layout_set(lu_layout_t* layout, lv_obj_t* obj, lv_point_t pos, lv_point_t size)
{
    if(!layout || !obj)
    {
        return;
    }
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, pos.x, size.x, LV_GRID_ALIGN_STRETCH, pos.y, size.y);
}
