#ifndef __OSUI_UI_STATBAR_H__
#define __OSUI_UI_STATBAR_H__

//OSUI基础
#include"osui/core/osui_base.h"
//OSUI显示
#include"osui/disp/osui_disp.h"
//OSUI输入
#include"osui/input/osui_input.h"

//UI状态栏初始化
void osui_ui_statbar_init();
//获取状态栏高度
uint16_t osui_ui_statbar_get_height();
//设置状态栏图标
void osui_ui_statbar_set_icon(const char* icon);
//设置状态栏时间
void osui_ui_statbar_set_time(const char* time);
//设置状态栏电量
void osui_ui_statbar_set_batt(const char* batt);
//设置状态栏字体颜色
void osui_ui_statbar_set_font_color(lv_color_t color);

#endif
