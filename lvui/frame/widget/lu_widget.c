#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/misc/lu_log.h"
#include "src/core/lv_obj.h"
#include "src/core/lv_obj_style_gen.h"
#include "src/core/lv_obj_tree.h"
#include "src/widgets/image/lv_image.h"

lv_obj_t* lu_widget_obj_init(lv_obj_t* parent);
lv_obj_t* lu_widget_txt_init(lv_obj_t* parent, const lv_font_t* font);
lv_obj_t* lu_widget_btn_init(lv_obj_t* parent);
lv_obj_t* lu_widget_switch_init(lv_obj_t* parent);
lv_obj_t* lu_widget_slider_init(lv_obj_t* parent);
lv_obj_t* lu_widget_image_init(lv_obj_t* parent);
// bool lu_widget_image_set_src(lv_obj_t* container, const char* src);

lv_obj_t* lu_widget_obj_init(lv_obj_t* parent)
{
    lv_obj_t* obj = lv_obj_create(parent);
    if(!obj)
    {
        return NULL;
    }
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 0, 0);
    lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    return obj;
}

lv_obj_t* lu_widget_txt_init(lv_obj_t* parent, const lv_font_t* font)
{
    lv_obj_t* obj = lv_label_create(parent);
    if(!obj)
    {
        return NULL;
    }
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_text_font(obj, font, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(obj, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
    return obj;
}

lv_obj_t* lu_widget_btn_init(lv_obj_t* parent)
{
    lv_obj_t* obj = lv_button_create(parent);
    if(!obj)
    {
        return NULL;
    }
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 0, 0);
    lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_transform_height(obj, 0, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_transform_width(obj, 0, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    return obj;
}

lv_obj_t* lu_widget_switch_init(lv_obj_t* parent)
{
    lv_obj_t* obj = lv_switch_create(parent);
    if(!obj)
    {
        return NULL;
    }
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 0, 0);
    lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(obj, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, lv_color_black(), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_state(obj, LV_STATE_CHECKED);
    lv_obj_set_style_opa(obj,0,LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj,0,LV_PART_INDICATOR|LV_STATE_DEFAULT);
    return obj;
}

lv_obj_t* lu_widget_slider_init(lv_obj_t* parent)
{
    lv_obj_t* obj = lv_slider_create(parent);
    if(!obj)
    {
        return NULL;
    }
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 0, 0);
    lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(obj, LV_OPA_100, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, lv_color_black(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_slider_set_value(obj,0,LV_ANIM_OFF);
    lv_obj_set_style_opa(obj,0,LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj,0,LV_PART_INDICATOR|LV_STATE_DEFAULT);
    return obj;
}

lv_obj_t* lu_widget_image_init(lv_obj_t* parent)
{
    // lv_obj_t* container = lu_widget_obj_init(parent);
    // if(!container)
    // {
    //     return NULL;
    // }
    lv_obj_t* obj = lv_image_create(parent);
    if(!obj)
    {
        return NULL;
    }
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 0, 0);
    lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    // // lv_image_set_scale(obj, 64);
    // lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_STRETCH);
    return obj;
}

// bool lu_widget_image_set_src(lv_obj_t* container, const char* src)
// {
//     lv_obj_t* obj = lv_obj_get_child(container, 0);
//     if(obj&&lv_obj_check_type(obj, &lv_image_class))
//     {
//         lv_image_set_src(obj, src);
//         // lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_CENTER);
//         // LU_LOG("lu_widget_image_set_src: %s", src);
//         return true;
//     }
//     // LU_LOG("lu_widget_image_set_src: obj is not image");
//     return false;
// }
