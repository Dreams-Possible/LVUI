#ifndef __OSUI_UI_ANIM_H__
#define __OSUI_UI_ANIM_H__

//OSUI基础
#include"osui/core/osui_base.h"
//OSUI输入
#include"osui/input/osui_input.h"
//UIPD控制器
#include"osui/ui/osui_ui_pdc.h"

//UI动画类型
typedef enum osui_ui_anim_type_t
{
    OSUI_UI_ANIM_TYPE_NONE = 0,
    OSUI_UI_ANIM_TYPE_X = 0,
    OSUI_UI_ANIM_TYPE_Y = 1,
    OSUI_UI_ANIM_TYPE_OPA = 2,
    OSUI_UI_ANIM_TYPE_CB = 3
}osui_ui_anim_type_t;
//UI动画类
typedef struct osui_ui_anim_t
{
    lv_obj_t* obj;
    osui_ui_pdc_t* pdc;
    osui_ui_anim_type_t type;
    int16_t tg;
    void (*cb_set)(int16_t data);
    int16_t (*cb_get)();
}osui_ui_anim_t;

//动画初始化
osui_ui_anim_t*osui_ui_anim_init();
//动画反初始化
void osui_ui_anim_deinit(osui_ui_anim_t* anim);
//动画设置对象
void osui_ui_anim_set_obj(osui_ui_anim_t* anim, lv_obj_t* obj);
//动画设置类型
void osui_ui_anim_set_type(osui_ui_anim_t* anim, osui_ui_anim_type_t type);
//动画设置PD参数
void osui_ui_anim_set_pd(osui_ui_anim_t* anim, float kp, float kd);
//动画设置目标
void osui_ui_anim_set_tg(osui_ui_anim_t* anim, int16_t tg);
//动画设置回调
void osui_ui_anim_set_cb(osui_ui_anim_t* anim, int16_t (*cb_get)(), void (*cb_set)(int16_t data));
//动画获取目标
int16_t osui_ui_anim_get_tg(osui_ui_anim_t* anim);
//动画检查完成
bool osui_ui_anim_finish(osui_ui_anim_t* anim);
//动画执行
void osui_ui_anim_exec(osui_ui_anim_t* anim);
//动画复位
void osui_ui_anim_reset(osui_ui_anim_t* anim);

//UI动画LVGL类型
typedef lv_anim_custom_exec_cb_t osui_ui_anim_lv_exec_cb_t;
// typedef enum osui_ui_anim_lv_type_t
// {
//     OSUI_UI_ANIM_LV_TYPE_NONE = 0,
//     OSUI_UI_ANIM_LV_TYPE_X = 0,
//     OSUI_UI_ANIM_LV_TYPE_Y = 1,
//     OSUI_UI_ANIM_LV_TYPE_OPA = 2,
//     OSUI_UI_ANIM_LV_TYPE_CB = 3
// }osui_ui_anim_lv_type_t;
//UI动画LVGL类
typedef struct osui_ui_anim_lv_t
{
    lv_anim_t anim;
    lv_obj_t*obj;
    osui_ui_anim_lv_exec_cb_t exec_cb;
    // osui_ui_anim_lv_type_t type;
    int32_t p1;
    int32_t p2;
    // void (*exec_cb)(int32_t value);
    void (*finish_cb)();
}osui_ui_anim_lv_t;

//动画LVGI初始化
osui_ui_anim_lv_t*osui_ui_anim_lv_init();
//动画LVGL反初始化
void osui_ui_anim_lv_deinit(osui_ui_anim_lv_t* anim);
//动画LVGL设置对象
void osui_ui_anim_lv_set_obj(osui_ui_anim_lv_t* anim, lv_obj_t* obj);
//动画LVGL设置参数
void osui_ui_anim_lv_set_param(osui_ui_anim_lv_t* anim, int32_t start, int32_t end);
//动画LVGL设置时间
void osui_ui_anim_lv_set_time(osui_ui_anim_lv_t* anim, uint32_t time);
//动画LVGL设置执行回调
void osui_ui_anim_lv_set_exec_cb(osui_ui_anim_lv_t* anim, osui_ui_anim_lv_exec_cb_t cb);
//动画LVGL设置完成回调
void osui_ui_anim_lv_set_finish_cb(osui_ui_anim_lv_t* anim, void (*cb)());
//动画LVGL设置贝塞尔参数
void osui_ui_anim_lv_set_bezier_param(osui_ui_anim_lv_t* anim, int32_t p1, int32_t p2);
//动画LVGL开始
void osui_ui_anim_lv_start(osui_ui_anim_lv_t* anim);
//动画LVGL动画执行回调（X轴位置）
void osui_ui_anim_lv_exec_cb_x(lv_anim_t * anim, int32_t value);
//动画LVGL动画执行回调（Y轴位置）
void osui_ui_anim_lv_exec_cb_y(lv_anim_t * anim, int32_t value);
//动画LVGL动画执行回调（背景透明度）
void osui_ui_anim_lv_exec_cb_bg_opa(lv_anim_t * anim, int32_t value);

#endif
