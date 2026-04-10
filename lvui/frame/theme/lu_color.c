#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/misc/lu_log.h"

lv_color_t lu_color_invert(lv_color_t color);
lv_color_t lu_color_smart_invert(lv_color_t color);

lv_color_t lu_color_invert(lv_color_t color)
{
    return lv_color_make(
        255 - color.red,
        255 - color.green,
        255 - color.blue
    );
}

lv_color_t lu_color_smart_invert(lv_color_t color)
{
    uint8_t brightness = lv_color_brightness(color);
    if (brightness > 128)
    {
        return lv_color_darken(lu_color_invert(color), 32);
    }else
    {
        return lv_color_lighten(lu_color_invert(color), 32);
    }
}
