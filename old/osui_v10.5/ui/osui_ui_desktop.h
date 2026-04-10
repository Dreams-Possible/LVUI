#ifndef __OSUI_UI_DESKTOP_H__
#define __OSUI_UI_DESKTOP_H__

//OSUI基础
#include"osui/core/osui_base.h"
//OSUI显示
#include"osui/disp/osui_disp.h"
//OSUI输入
#include"osui/input/osui_input.h"

//进入锁屏
void osui_ui_locker_enter();
//UI桌面初始化
void osui_ui_desktop_init();
//桌面添加应用
void osui_ui_desktop_add_app(const char *name, lv_obj_t*app);
// //桌面启动应用
// void osui_ui_desktop_launch(lv_obj_t*app);
//桌面退出应用
void osui_ui_desktop_delaunch();
//获取桌面宽度
int16_t osui_ui_desktop_get_width();
//获取桌面高度
int16_t osui_ui_desktop_get_height();
//设置桌面时间
void osui_ui_desktop_set_time(const char* time);
//设置桌面日期
void osui_ui_desktop_set_date(const char* date);
//设置桌面壁纸
void osui_ui_desktop_set_wallpaper(char* path);
//设置桌面字体颜色
void osui_ui_desktop_set_font_color(lv_color_t color);
//设置桌面背景颜色
void osui_ui_desktop_set_back_color(lv_color_t color);

#endif
