#if 1
#ifndef LVUI_CONTROLCENTER_H
#define LVUI_CONTROLCENTER_H

//include
#include"lvui_manager.h"

//define
#define LVUI_CONTROLCENTER_BUTTON_LEFT 1
#define LVUI_CONTROLCENTER_BUTTON_RIGHT 2
#define LVUI_CONTROLCENTER_SLIDER_LEFT 3
#define LVUI_CONTROLCENTER_SLIDER_RIGHT 4

//lvui controlcenter init
void lvui_controlcenter_init();
//lvui controlcenter load
lv_obj_t*lvui_controlcenter_load();
//shortcutlist add
void shortcutlist_add(char*text,void(*shortcut_cb)());
//noticelist add
void noticelist_add(char*text,void(*notice_cb)());
//lvui controlcenter get
uint8_t lvui_controlcenter_get(uint8_t widget);

#endif//#ifndef LVUI_CONTROLCENTER_H
#endif//#if 1
