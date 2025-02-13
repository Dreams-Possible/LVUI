#if 1
#ifndef LVUI_STATUSBAR_H
#define LVUI_STATUSBAR_H

//include
#include"lvui_manager.h"

//define
#define LVUI_STATUSBAR_VER lvui_display_change(32)

//lvui statusbar init
void lvui_statusbar_init();
//lvui statusbar load
lv_obj_t*lvui_statusbar_load();
//lvui statusbar height
uint8_t lvui_statusbar_height();

#endif//#ifndef LVUI_STATUSBAR_H
#endif//#if 1
