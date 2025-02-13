#if 1
#ifndef LVUI_MANAGER_H
#define LVUI_MANAGER_H

//include
#include<stdio.h>
#include<math.h>
#include"lvgl/lvgl.h"
#include"lvui_display.h"
#include"lvui_gesture.h"
#include"lvui_statusbar.h"
#include"lvui_controlcenter.h"
#include"lvui_launcher.h"
#include"lvui_appexample.h"
#include"lvui_appexample_setting.h"
#include"lvui_ui.h"

//define
#define lvui_printf printf

//lvui manager screen
void lvui_manager_screen(uint16_t*hor,uint16_t*ver);
//lvui manager init
void lvui_manager_init(uint16_t hor,uint16_t ver,float size);

#endif//#ifndef LVUI_MANAGER_H
#endif//#if 1
