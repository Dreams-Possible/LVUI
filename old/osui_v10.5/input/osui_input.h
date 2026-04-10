#ifndef __OSUI_INPUT_H__
#define __OSUI_INPUT_H__

//OSUI基础
#include"osui/core/osui_base.h"
//OSUI显示
#include"osui/disp/osui_disp.h"

//参数定义
#define OSUI_INPUT_SAMPLE_TIME 50//输入采样时间
#define OSUI_INPUT_DP_SPEED_FAST 0.4//像素无关输入速度快

//输入初始化
void osui_input_init();
//是否启用实时速度纪录
void osui_input_enable_rtspeed(bool enable);
//获取输入状态
bool osui_input_get_status();
//获取输入坐标
void osui_input_get_point(uint16_t* x,uint16_t* y);
//单独获取X坐标
uint16_t osui_input_get_x(void);
//单独获取Y坐标
uint16_t osui_input_get_y(void);
//获取输入移动距离
void osui_input_get_move(int16_t* x,int16_t* y);
//单独获取X移动距离
int16_t osui_input_get_move_x(void);
//单独获取Y移动距离
int16_t osui_input_get_move_y(void);
//获取实时输入速度
void osui_input_get_rtspeed(float* x,float* y);
//单独获取X速度
float osui_input_get_rtspeed_x(void);
//单独获取Y速度
float osui_input_get_rtspeed_y(void);
//获取像素无关实时输入速度
void osui_input_get_rtspeed_dp(float* x,float* y);
//单独获取像素无关X速度
float osui_input_get_rtspeed_dp_x(void);
//单独获取像素无关Y速度
float osui_input_get_rtspeed_dp_y(void);

#endif
