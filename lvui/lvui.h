#ifndef LVUI_H
#define LVUI_H

#include "lu_init.h"

#include "lvui/sys/lu_sysui.h"

typedef struct lvui_t
{
    lv_disp_t* lv_disp;
    lv_indev_t* lv_indev;
    // lv_theme_t* lv_theme;
    lu_disp_t* lu_disp;
    lu_touch_t* lu_touch;
    lu_sysui_t* sysui;
}lvui_t;

lvui_t* lvui_create(int16_t width, int16_t height, float size);
void lvui_delete(lvui_t** lvui);

#endif
