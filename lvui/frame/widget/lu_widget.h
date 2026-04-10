#ifndef LU_WIDGET_H
#define LU_WIDGET_H

#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"

lv_obj_t* lu_widget_obj_init(lv_obj_t* parent);
lv_obj_t* lu_widget_txt_init(lv_obj_t* parent, const lv_font_t* font);
lv_obj_t* lu_widget_btn_init(lv_obj_t* parent);
lv_obj_t* lu_widget_switch_init(lv_obj_t* parent);
lv_obj_t* lu_widget_slider_init(lv_obj_t* parent);

#endif
