#ifndef __OSUI_SYS_H__
#define __OSUI_SYS_H__

//OSUI基础
#include"osui/core/osui_base.h"

//系统设置时间
void osui_sys_set_time(uint8_t hour, uint8_t minute);
//系统设置日期
void osui_sys_set_date(uint8_t month, uint8_t day);
//系统设置年份
void osui_sys_set_year(uint16_t year);
//系统设置电量
void osui_sys_set_batt(uint8_t batt);
//系统设置WIFI状态
void osui_sys_set_wifi_stat(bool stat);
//系统设置蓝牙状态
void osui_sys_set_bt_stat(bool stat);
//系统刷新
void osui_sys_refresh();
//系统初始化
void osui_sys_init();

#endif
