#ifndef __OSUI_UI_PDC_H__
#define __OSUI_UI_PDC_H__

//OSUI基础
#include"osui/core/osui_base.h"

/**
 * @brief PD控制器结构体
 * @param kp 比例系数
 * @param kd 微分系数
 * @param target 目标值
 * @param output_min 输出最小值限制
 * @param output_max 输出最大值限制
 * @param last_error 上一次误差值
 * @param last_time 上一次计算时间
 */
typedef struct osui_ui_pdc_t
{
    float kp;
    float kd;
    float target;
    float output_min;
    float output_max;
    float last_error;
    float last_time;
} osui_ui_pdc_t;

/**
 * @brief 初始化PD控制器
 * @return 返回新创建的PD控制器指针，失败返回NULL
 */
osui_ui_pdc_t* osui_ui_pdc_init();

/**
 * @brief 设置PD控制器目标值
 * @param pdc PD控制器指针
 * @param target 目标值
 */
void osui_ui_pdc_set_target(osui_ui_pdc_t* pdc, float target);

/**
 * @brief 单独设置PD控制器的比例系数
 * @param pdc PD控制器指针
 * @param kp 新的比例系数
 */
void osui_ui_pdc_set_kp(osui_ui_pdc_t* pdc, float kp);

/**
 * @brief 单独设置PD控制器的微分系数
 * @param pdc PD控制器指针
 * @param kd 新的微分系数
 */
void osui_ui_pdc_set_kd(osui_ui_pdc_t* pdc, float kd);

/**
 * @brief 设置PD控制器输出限幅
 * @param pdc PD控制器指针
 * @param min 输出最小值
 * @param max 输出最大值
 */
void osui_ui_pdc_set_limit(osui_ui_pdc_t* pdc, float min, float max);

/**
 * @brief 反初始化PD控制器
 * @param pdc 指向PD控制器指针的指针
 * @note 该函数会释放PD控制器内存
 */
void osui_ui_pdc_deinit(osui_ui_pdc_t**pdc);

/**
 * @brief PD控制器计算
 * @param pdc PD控制器指针
 * @param input 当前输入值
 * @param time 当前时间
 * @return 返回PDC计算输出值
 */
float osui_ui_pdc_comput(osui_ui_pdc_t* pdc, float input, float time);

#endif
