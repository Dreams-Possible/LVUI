#include"osui_ui_anim.h"

//依赖
#include<math.h>

//参数定义
#define ANIM_MOVE_MIN (1)//最小移动单位

//动画控件获取
static int16_t anim_obj_get(osui_ui_anim_t* anim);
//动画控件设置
static void anim_obj_set(osui_ui_anim_t* anim, int16_t set);
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
// //动画等待准备
// bool osui_ui_anim_wait(osui_ui_anim_t* anim);

//动画控件获取
static int16_t anim_obj_get(osui_ui_anim_t* anim)
{
    int16_t ret=0;
    //判断类型
    switch (anim->type)
    {
        //X轴位置
        case OSUI_UI_ANIM_TYPE_X:
            ret=lv_obj_get_x(anim->obj);
        break;
        //Y轴位置
        case OSUI_UI_ANIM_TYPE_Y:
            ret=lv_obj_get_y(anim->obj);
        break;
        //透明度
        case OSUI_UI_ANIM_TYPE_OPA:
            ret=lv_obj_get_style_bg_opa(anim->obj, LV_PART_MAIN | LV_STATE_DEFAULT);
        break;
        //自定义回调
        case OSUI_UI_ANIM_TYPE_CB:
            ret=anim->cb_get();
        break;
        //无
        default:
        break;
    }
    return ret;
}

//动画控件设置
static void anim_obj_set(osui_ui_anim_t* anim, int16_t set)
{
    //判断类型
    switch (anim->type)
    {
        //X轴位置
        case OSUI_UI_ANIM_TYPE_X:
            lv_obj_set_x(anim->obj,set);
        break;
        //Y轴位置
        case OSUI_UI_ANIM_TYPE_Y:
            lv_obj_set_y(anim->obj,set);
        break;
        //透明度
        case OSUI_UI_ANIM_TYPE_OPA:
            lv_obj_set_style_bg_opa(anim->obj, set, LV_PART_MAIN | LV_STATE_DEFAULT);
        break;
        //自定义回调
        case OSUI_UI_ANIM_TYPE_CB:
            anim->cb_set(set);
        break;
        //无
        default:
        break;
    }
}

//动画初始化
osui_ui_anim_t*osui_ui_anim_init()
{
    osui_ui_anim_t* anim = (osui_ui_anim_t*)osui_malloc(sizeof(osui_ui_anim_t));
    if(!anim)
    {
        return NULL;
    }
    anim->pdc = osui_ui_pdc_init();
    if(!anim->pdc)
    {
        return NULL;
    }
    return anim;
}

//动画反初始化
void osui_ui_anim_deinit(osui_ui_anim_t* anim)
{
    osui_free(anim->pdc);
    osui_free(anim);
}

//动画设置对象
void osui_ui_anim_set_obj(osui_ui_anim_t* anim, lv_obj_t* obj)
{
    anim->obj = obj;
}

//动画设置类型
void osui_ui_anim_set_type(osui_ui_anim_t* anim, osui_ui_anim_type_t type)
{
    anim->type=type;
}

//动画设置PD参数
void osui_ui_anim_set_pd(osui_ui_anim_t* anim, float kp, float kd)
{
    osui_ui_pdc_set_kp(anim->pdc, kp);
    osui_ui_pdc_set_kd(anim->pdc, kd);
}

//动画设置目标
void osui_ui_anim_set_tg(osui_ui_anim_t* anim, int16_t tg)
{
    anim->tg=tg;
    osui_ui_pdc_set_target(anim->pdc, tg);
}

//动画设置回调
void osui_ui_anim_set_cb(osui_ui_anim_t* anim, int16_t (*cb_get)(), void (*cb_set)(int16_t data))
{
    anim->cb_get=cb_get;
    anim->cb_set=cb_set;
}

//动画获取目标
int16_t osui_ui_anim_get_tg(osui_ui_anim_t* anim)
{
    return anim->tg;
}

//动画检查完成
bool osui_ui_anim_finish(osui_ui_anim_t* anim)
{
    bool ret=true;
    int16_t curr = anim_obj_get(anim);
    if(curr!=anim->tg)
    {
        ret=false;
    }
    // osui_log("curr=%d,tg=%d",curr,anim->tg);
    return ret;
}

//动画执行
void osui_ui_anim_exec(osui_ui_anim_t* anim)
{
    //调整
    int16_t curr = anim_obj_get(anim);
    if(curr!=anim->tg)
    {
        if(abs(curr-anim->tg)==1)
        {
            anim_obj_set(anim, anim->tg);
            return;
        }
        float move = osui_ui_pdc_comput(anim->pdc, curr, lv_tick_get());
        if(move>=0&&move<ANIM_MOVE_MIN)
        {
            move=ANIM_MOVE_MIN;
        }
        if(move>-ANIM_MOVE_MIN&&move<0)
        {
            move=-ANIM_MOVE_MIN;
        }
        int16_t set = curr+(int16_t)move;
        anim_obj_set(anim, set);
        // osui_log("curr=%d,set=%d,move=%f",curr,set,move);
    }
}

//动画复位
void osui_ui_anim_reset(osui_ui_anim_t* anim)
{
    anim->pdc->last_time=0;
    anim->pdc->last_error=0;
}

//LVGL动画路径回调
static int32_t lv_anim_path_cb(const lv_anim_t *anim);
//LVGL动画完成回调
static void lv_anim_finish_cb(lv_anim_t * anim);
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

//LVGL动画路径回调
static int32_t lv_anim_path_cb(const lv_anim_t *anim)
{
    //贝塞尔曲线
    uint32_t t = lv_map(anim->act_time, 0, anim->duration, 0, 1024);
    // int32_t step = lv_anim_path_cb3(t, 0, 0, 1024, 1024);
    int32_t step = lv_bezier3(t, 0, ((osui_ui_anim_lv_t*)(anim->user_data))->p1, ((osui_ui_anim_lv_t*)(anim->user_data))->p2, 1024);
    int32_t new_value;
    new_value = step * (anim->end_value - anim->start_value);
    new_value = new_value >> 10;
    new_value += anim->start_value;
    return new_value;
}

//LVGL动画完成回调
static void lv_anim_finish_cb(lv_anim_t * anim)
{
    osui_ui_anim_lv_t* ui_anim = (osui_ui_anim_lv_t*)anim->user_data;
    if(ui_anim->finish_cb)
    {
        ui_anim->finish_cb(anim);
    }
}

//动画LVGI初始化
osui_ui_anim_lv_t*osui_ui_anim_lv_init()
{
    //申请内存
    osui_ui_anim_lv_t*anim=(osui_ui_anim_lv_t*)osui_malloc(sizeof(osui_ui_anim_lv_t));
    if(!anim)
    {
        osui_log("mem err");
        return NULL;
    }
    memset(anim,0,sizeof(osui_ui_anim_lv_t));
    //初始化
    lv_anim_init(&anim->anim);
    lv_anim_set_path_cb(&anim->anim, lv_anim_path_cb);
    lv_anim_set_completed_cb(&anim->anim, lv_anim_finish_cb);
    lv_anim_set_user_data(&anim->anim, anim);
    return anim;
}

//动画LVGL反初始化
void osui_ui_anim_lv_deinit(osui_ui_anim_lv_t* anim)
{
    lv_anim_custom_delete(&anim->anim, anim->exec_cb);
    osui_free(anim);
}

//动画LVGL设置对象
void osui_ui_anim_lv_set_obj(osui_ui_anim_lv_t* anim, lv_obj_t* obj)
{
    anim->obj = obj;
    lv_anim_set_var(&anim->anim, anim->obj);
}

//动画LVGL设置参数
void osui_ui_anim_lv_set_param(osui_ui_anim_lv_t* anim, int32_t start, int32_t end)
{
    lv_anim_set_values(&anim->anim, start, end);
}

//动画LVGL设置时间
void osui_ui_anim_lv_set_time(osui_ui_anim_lv_t* anim, uint32_t time)
{
    lv_anim_set_duration(&anim->anim, time);
}

//动画LVGL设置执行回调
void osui_ui_anim_lv_set_exec_cb(osui_ui_anim_lv_t* anim, osui_ui_anim_lv_exec_cb_t cb)
{
    anim->exec_cb = (lv_anim_custom_exec_cb_t)cb;
    lv_anim_set_custom_exec_cb(&anim->anim, anim->exec_cb);
}

//动画LVGL设置完成回调
void osui_ui_anim_lv_set_finish_cb(osui_ui_anim_lv_t* anim, void (*cb)())
{
    anim->finish_cb = cb;
}

//动画LVGL设置贝塞尔参数
void osui_ui_anim_lv_set_bezier_param(osui_ui_anim_lv_t* anim, int32_t p1, int32_t p2)
{
    anim->p1 = p1;
    anim->p2 = p2;
}

//动画LVGL开始
void osui_ui_anim_lv_start(osui_ui_anim_lv_t* anim)
{
    lv_anim_start(&anim->anim);
}

//动画LVGL动画执行回调（X轴位置）
void osui_ui_anim_lv_exec_cb_x(lv_anim_t * anim, int32_t value)
{
    lv_obj_set_x(anim->var, value);
}

//动画LVGL动画执行回调（Y轴位置）
void osui_ui_anim_lv_exec_cb_y(lv_anim_t * anim, int32_t value)
{
    lv_obj_set_y(anim->var, value);
}

//动画LVGL动画执行回调（背景透明度）
void osui_ui_anim_lv_exec_cb_bg_opa(lv_anim_t * anim, int32_t value)
{
    lv_obj_set_style_bg_opa(anim->var, value, LV_PART_MAIN | LV_STATE_DEFAULT);
}
