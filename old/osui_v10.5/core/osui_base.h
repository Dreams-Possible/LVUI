#ifndef __OSUI_BASE_H__
#define __OSUI_BASE_H__

//类型类
#include<stdint.h>
#include<stdbool.h>

//结果类
typedef enum os_ret_t
{
    OS_RET_NONE=0,//没有错误
    OS_RET_PARAM,//参数错误
    OS_RET_MEMORY,//内存错误
    OS_RET_UNKNOWN//未知错误
}os_ret_t;


//内存分配
#include<stdlib.h>
#define osui_malloc malloc
#define osui_free free

//LVGL支持
#include"lvgl/lvgl.h"

//日志记录
#include<stdio.h>
#define osui_log(fmt,...) LV_LOG_USER("[UI_LOG][%s:%d:%s]: "fmt" \n",__FILE__,__LINE__,__func__,##__VA_ARGS__)


#endif
