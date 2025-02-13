#if 1
#ifndef LVUI_DISPLAY_H
#define LVUI_DISPLAY_H

//include
#include"lvui_manager.h"

//define
#define LVUI_DISPLAY_LEVEL_SMALL 1
#define LVUI_DISPLAY_LEVEL_NOMALL 2
#define LVUI_DISPLAY_LEVEL_BIG 3

//lvui display change
uint16_t lvui_display_change(uint16_t dp);
//lvui display init
void lvui_display_init(uint16_t hor,uint16_t ver,float size);
//lvui display level
uint8_t lvui_display_level();

#endif//#ifndef LVUI_DISPLAY_H
#endif//#if 1
