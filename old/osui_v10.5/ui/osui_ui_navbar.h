#ifndef __OSUI_UI_NAVBAR_H__
#define __OSUI_UI_NAVBAR_H__

//OSUI基础
#include"osui/core/osui_base.h"
//OSUI显示
#include"osui/disp/osui_disp.h"
//OSUI输入
#include"osui/input/osui_input.h"

//UI导航条初始化
void osui_ui_navbar_init();
//获取导航条高度
uint16_t osui_ui_navbar_get_height();
//设置导航条颜色
void osui_ui_navbar_set_color(lv_color_t color);

#endif
