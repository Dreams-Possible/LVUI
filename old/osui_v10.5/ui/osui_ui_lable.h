#ifndef __OSUI_UI_LABLE_H__
#define __OSUI_UI_LABLE_H__

//OSUI基础
#include"osui/core/osui_base.h"

//参数定义
#define OSUI_UI_LABLE_FONT_MAX lv_font_montserrat_48//大字体
#define OSUI_UI_LABLE_FONT_MID lv_font_montserrat_24//中字体
#define OSUI_UI_LABLE_FONT_MIN lv_font_montserrat_12//小字体
#define OSUI_UI_LABLE_FONT_CONTENT lv_font_montserrat_14//内容字体

//获取标签高度
uint16_t osui_ui_lable_get_ver(const lv_font_t *font);
//获取标签宽度
uint16_t osui_ui_lable_get_hor(const char *text, const lv_font_t *font);
//获取标签尺寸
void osui_ui_lable_get_size(const char *text, const lv_font_t *font, uint16_t *hor, uint16_t *ver);

#endif
