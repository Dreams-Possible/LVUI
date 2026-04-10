#ifndef __OSUI_UI_LOCKRE_H__
#define __OSUI_UI_LOCKRE_H__

//OSUI基础
#include"osui/core/osui_base.h"
//OSUI显示
#include"osui/disp/osui_disp.h"

//UI锁屏初始化
void osui_ui_locker_init();
//进入锁屏
void osui_ui_locker_enter();
//获取锁屏状态
bool osui_ui_locker_get_status();
//设置锁屏图标
void osui_ui_locker_set_icon(const char* icon);
//设置锁屏时间
void osui_ui_locker_set_time(const char* time);
//设置锁屏日期
void osui_ui_locker_set_date(const char* date);
//设置锁屏年份
void osui_ui_locker_set_year(const char* year);
//设置锁屏字体颜色
void osui_ui_locker_set_font_color(lv_color_t color);
//设置锁屏背景颜色
void osui_ui_locker_set_back_color(lv_color_t color);
//设置锁屏壁纸
void osui_ui_locker_set_wallpaper(char* path);

#endif
