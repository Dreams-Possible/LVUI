#ifndef __OSUI_DISP_H__
#define __OSUI_DISP_H__

//OSUI基础
#include"osui/core/osui_base.h"

//参数定义
#define OSUI_STAND_DPI 160//标准DPI

//显示参数类
typedef struct osui_dispinfo_t
{
    uint16_t hor;//水平像素
    uint16_t ver;//垂直像素
    float size;//大小（英寸）
    float dpi;//像素密度（像素每英寸）
}osui_dispinfo_t;

//显示初始化
void osui_disp_init(uint16_t hor,uint16_t ver,float size);
//像素转像素无关
float osui_disp_px2dp(float px);
//像素无关转像素
float osui_disp_dp2px(float dp);
//显示获取
osui_dispinfo_t*osui_disp_get();

#endif
