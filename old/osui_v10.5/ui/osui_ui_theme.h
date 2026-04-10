#ifndef __OSUI_UI_THEME_H__
#define __OSUI_UI_THEME_H__

//OSUI基础
#include"osui/core/osui_base.h"
//OSUI显示
#include"osui/disp/osui_disp.h"
//OSUI输入
#include"osui/input/osui_input.h"

//参数定义
// #define OSUI_THEME_SAMPLE_TIME (LV_INDEV_STATE_PR*2)//主题采样时间
// #define OSUI_THEME_ANIM_PDC_P (0.1)//主题动画PD控制器P参数
// #define OSUI_THEME_ANIM_PDC_D (0.1)//主题动画PD控制器D参数
#define OSUI_THEME_LIGHT (lv_color_hex(0xffffffff))//主题明亮
#define OSUI_THEME_DARK (lv_color_hex(0xff000000))//主题黑暗
// #define OSUI_THEME_DARK (lv_color_hex(0xff00ff00))//主题黑暗

//主题颜色取反
lv_color_t osui_theme_color_invert(lv_color_t color);
//主题颜色智能取反
lv_color_t osui_theme_smart_invert(lv_color_t color);
//UI主题初始化
void osui_ui_theme_init();
//UI设置主题色
void osui_ui_theme_set_color(lv_color_t color);
//UI获取主题色
lv_color_t osui_ui_theme_get_color();

#endif
