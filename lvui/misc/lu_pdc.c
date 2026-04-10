#include "lvui/plat/lu_type.h"
#include "lvui/misc/lu_log.h"
#include "lvui/plat/lu_mem.h"
#include <float.h>

typedef struct lu_pdc_t
{
    float kp;
    float kd;
    float target;
    float output_min;
    float output_max;
    float last_error;
    float last_time;
} lu_pdc_t;

lu_pdc_t* lu_pdc_create();
void lu_pdc_delete(lu_pdc_t**pdc);
void lu_pdc_set_target(lu_pdc_t* pdc, float target);
float lu_pdc_get_target(lu_pdc_t* pdc);
void lu_pdc_set_kp(lu_pdc_t* pdc, float kp);
float lu_pdc_get_kp(lu_pdc_t* pdc);
void lu_pdc_set_kd(lu_pdc_t* pdc, float kd);
float lu_pdc_get_kd(lu_pdc_t* pdc);
void lu_pdc_set_limit(lu_pdc_t* pdc, float min, float max);
void lu_pdc_get_limit(lu_pdc_t* pdc, float* min, float* max);
void lu_pdc_reset(lu_pdc_t* pdc);
float lu_pdc_comput(lu_pdc_t* pdc, float input, float time);

lu_pdc_t* lu_pdc_create()
{
    lu_pdc_t* pdc = (lu_pdc_t*)lu_calloc(1, sizeof(lu_pdc_t));
    if(!pdc)
    {
        return NULL;
    }
    pdc->output_max = FLT_MAX;
    pdc->output_min = -FLT_MAX;
    return pdc; 
}

void lu_pdc_delete(lu_pdc_t**pdc)
{
    if(*pdc)
    {
        lu_free(*pdc);
        *pdc = NULL;
    }
}

void lu_pdc_set_target(lu_pdc_t* pdc, float target)
{
    if(pdc)
    {
        pdc->target = target;
    }
}

float lu_pdc_get_target(lu_pdc_t* pdc)
{
    if(pdc)
    {
        return pdc->target;
    }
    return 0;
}

void lu_pdc_set_kp(lu_pdc_t* pdc, float kp)
{
    if(pdc)
    {
        pdc->kp = kp;
    }
}

float lu_pdc_get_kp(lu_pdc_t* pdc)
{
    if(pdc)
    {
        return pdc->kp;
    }
    return 0;
}

void lu_pdc_set_kd(lu_pdc_t* pdc, float kd)
{
    if(pdc)
    {
        pdc->kd = kd;
    }
}

float lu_pdc_get_kd(lu_pdc_t* pdc)
{
    if(pdc)
    {
        return pdc->kd;
    }
    return 0;
}

void lu_pdc_set_limit(lu_pdc_t* pdc, float min, float max)
{
    pdc->output_min = min;
    pdc->output_max = max;
}

void lu_pdc_get_limit(lu_pdc_t* pdc, float* min, float* max)
{
    if(pdc)
    {
        *min = pdc->output_min;
        *max = pdc->output_max;
    }
}

void lu_pdc_reset(lu_pdc_t* pdc)
{
    pdc->last_error = 0;
    pdc->last_time = 0;
}

float lu_pdc_comput(lu_pdc_t* pdc, float input, float time) 
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
    //微分项
    float derivative = 0;
    if (dt > 0)
    {
        derivative = pdc->kd * (error - pdc->last_error) / dt;
    }
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
    return output;
}
