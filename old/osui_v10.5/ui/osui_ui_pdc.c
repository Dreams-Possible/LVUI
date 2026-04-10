#include"osui_ui_pdc.h"

#include<float.h>

//PDC初始化
osui_ui_pdc_t* osui_ui_pdc_init();
//PDC设置目标
void osui_ui_pdc_set_target(osui_ui_pdc_t* pdc, float target);
//PDC设置KP
void osui_ui_pdc_set_kp(osui_ui_pdc_t* pdc, float kp);
//PDC设置KD
void osui_ui_pdc_set_kd(osui_ui_pdc_t* pdc, float kd);
//PDC设置输出限幅
void osui_ui_pdc_set_limit(osui_ui_pdc_t* pdc, float min, float max);
//PDC反初始化
void osui_ui_pdc_deinit(osui_ui_pdc_t**pdc);
//PDC计算
float osui_ui_pdc_comput(osui_ui_pdc_t* pdc, float input, float time);

//PDC初始化
osui_ui_pdc_t* osui_ui_pdc_init()
{
    osui_ui_pdc_t* pdc = (osui_ui_pdc_t*)osui_malloc(sizeof(osui_ui_pdc_t));
    if(!pdc)
    {
        osui_log("mem err");
        return NULL;
    }
    pdc->kp = 0;
    pdc->kd = 0;
    pdc->target = 0;
    pdc->output_min = -FLT_MAX;
    pdc->output_max = FLT_MAX;
    pdc->last_error = 0;
    pdc->last_time = 0;
    return pdc; 
}

//PDC设置目标
void osui_ui_pdc_set_target(osui_ui_pdc_t* pdc, float target)
{
    pdc->target = target;
}

//单独设置PD控制器的比例系数
void osui_ui_pdc_set_kp(osui_ui_pdc_t* pdc, float kp)
{
    if(pdc)
    {
        pdc->kp = kp;
    }
}

//单独设置PD控制器的微分系数
void osui_ui_pdc_set_kd(osui_ui_pdc_t* pdc, float kd)
{
    if(pdc)
    {
        pdc->kd = kd;
    }
}

//PDC设置输出限幅
void osui_ui_pdc_set_limit(osui_ui_pdc_t* pdc, float min, float max)
{
    pdc->output_min = min;
    pdc->output_max = max;
}

//PDC重置
void osui_ui_pdc_reset(osui_ui_pdc_t* pdc)
{
    pdc->last_error = 0;
    pdc->last_time = 0;
}

//PDC反初始化
void osui_ui_pdc_deinit(osui_ui_pdc_t**pdc)
{
    if(*pdc)
    {
        osui_free(*pdc);
        *pdc = NULL;
    }
}

//PDC计算
float osui_ui_pdc_comput(osui_ui_pdc_t* pdc, float input, float time)
{
    if(time == 0)
    {
        return 0;
    }
    //计算误差
    float error = pdc->target - input;
    //计算时间差
    float dt = 0;
    if (pdc->last_time > 0)
    {
        dt = time - pdc->last_time;
    }
    //比例项
    float proportional = pdc->kp * error;
    // osui_log("proportional: kp=%.2f, err=%.2f, prop=%.2f", pdc->kp, error, proportional);
    //微分项
    float derivative = 0;
    if (dt > 0)
    {
        derivative = pdc->kd * (error - pdc->last_error) / dt;
    }
    // osui_log("derivative: kd=%.2f, d_err=%.2f, deriv=%.2f", pdc->kd, (error - pdc->last_error), derivative);
    //计算输出
    float output = proportional + derivative;
    //输出限幅
    if (output > pdc->output_max)
    {
        output = pdc->output_max;
    }
    if (output < pdc->output_min)
    {
        output = pdc->output_min;
    }
    //更新状态
    pdc->last_error = error;
    pdc->last_time = time;
    // osui_log("pdc comput: in=%.2f, err=%.2f, out=%.2f", input, error, output);
    return output;
}
