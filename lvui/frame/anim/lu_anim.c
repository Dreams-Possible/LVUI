#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/misc/lu_log.h"
#include "lvui/plat/lu_mem.h"
#include "lvui/misc/lu_pdc.h"
#include "lu_anim.h"

typedef struct lu_anim_t
{
    lv_obj_t* obj;
    lu_pdc_t* pdc;
    lu_anim_get_cb_t get_cb;
    lu_anim_set_cb_t set_cb;
    lv_timer_t* timer;
}lu_anim_t;

static void anim_timer_cb(lv_timer_t*timer);
lu_anim_t* lu_anim_create(lv_obj_t* obj);
void lu_anim_delete(lu_anim_t** anim);
void lu_anim_set_obj(lu_anim_t* anim, lv_obj_t* obj);
lv_obj_t* lu_anim_get_obj(lu_anim_t* anim);
void lu_anim_set_value(lu_anim_t* anim, int32_t value);
int32_t lu_anim_get_value(lu_anim_t* anim);
void lu_anim_set_param(lu_anim_t* anim, const lu_anim_param_t* param);
lu_anim_param_t lu_anim_get_param(lu_anim_t* anim);
void lu_anim_set_get_cb(lu_anim_t* anim, lu_anim_get_cb_t get_cb);
lu_anim_get_cb_t lu_anim_get_get_cb(lu_anim_t* anim);
void lu_anim_set_set_cb(lu_anim_t* anim, lu_anim_set_cb_t set_cb);
lu_anim_set_cb_t lu_anim_get_set_cb(lu_anim_t* anim);
void lu_anim_start(lu_anim_t* anim);
void lu_anim_stop(lu_anim_t* anim);
bool lu_anim_is_finish(lu_anim_t* anim);
int32_t lu_anim_lv_anim_path_cb(const lv_anim_t *anim);
void lu_anim_lv_anim_auto_cb(lv_anim_t *anim);
void lu_anim_lv_anim_auto_wd_cb(lv_anim_t *anim);
int32_t lu_anim_lv_get_bg_opa(const lv_obj_t* obj);
void lu_anim_lv_set_bg_opa(lv_obj_t* obj, int32_t v);

static void anim_timer_cb(lv_timer_t*timer)
{
    lu_anim_t* anim = (lu_anim_t*)lv_timer_get_user_data(timer);
    int32_t curr = anim->get_cb(anim->obj);
    if(curr!=lu_anim_get_value(anim))
    {
        float move = lu_pdc_comput(anim->pdc, curr, lv_tick_get());
        if(move>=0&&move<1)
        {
            move=1;
        }
        if(move>-1&&move<0)
        {
            move=-1;
        }
        int32_t set = curr+(int32_t)move;
        anim->set_cb(anim->obj, set);
    }
}

lu_anim_t* lu_anim_create(lv_obj_t* obj)
{
    lu_anim_t* anim = (lu_anim_t*)lu_calloc(1, sizeof(lu_anim_t));
    if(!anim)
    {
        return NULL;
    }
    anim->obj = obj;
    anim->pdc = lu_pdc_create();
    if(!anim->pdc)
    {
        lu_free(anim);
        return NULL;
    }
    return anim;
}

void lu_anim_delete(lu_anim_t** anim)
{
    if(!anim)
    {
        return;
    }
    lu_pdc_delete(&(*anim)->pdc);
    lu_free(*anim);
    *anim = NULL;
}

void lu_anim_set_obj(lu_anim_t* anim, lv_obj_t* obj)
{
    if(!obj)
    {
        return;
    }
    anim->obj = obj;
}

lv_obj_t* lu_anim_get_obj(lu_anim_t* anim)
{
    if(!anim)
    {
        return NULL;
    }
    return anim->obj;
}

void lu_anim_set_value(lu_anim_t* anim, int32_t value)
{
    if(!anim)
    {
        return;
    }
    lu_pdc_set_target(anim->pdc, value);
}

int32_t lu_anim_get_value(lu_anim_t* anim)
{
    if(!anim)
    {
        return 0;
    }
    return lu_pdc_get_target(anim->pdc);
}

void lu_anim_set_param(lu_anim_t* anim, const lu_anim_param_t* param)
{
    if(!anim)
    {
        return;
    }
    lu_pdc_set_kp(anim->pdc, param->kp);
    lu_pdc_set_kd(anim->pdc, param->kd);
}

lu_anim_param_t lu_anim_get_param(lu_anim_t* anim)
{
    if(!anim)
    {
        return (lu_anim_param_t){0,0};
    }
    lu_anim_param_t param;
    param.kp = lu_pdc_get_kp(anim->pdc);
    param.kd = lu_pdc_get_kd(anim->pdc);
    return param;
}

void lu_anim_set_get_cb(lu_anim_t* anim, lu_anim_get_cb_t get_cb)
{
    if(!anim)
    {
        return;
    }
    anim->get_cb = get_cb;
}

lu_anim_get_cb_t lu_anim_get_get_cb(lu_anim_t* anim)
{
    if(!anim)
    {
        return NULL;
    }
    return anim->get_cb;
}

void lu_anim_set_set_cb(lu_anim_t* anim, lu_anim_set_cb_t set_cb)
{
    if(!anim)
    {
        return;
    }
    anim->set_cb = set_cb;
}

lu_anim_set_cb_t lu_anim_get_set_cb(lu_anim_t* anim)
{
    if(!anim)
    {
        return NULL;
    }
    return anim->set_cb;
}

void lu_anim_start(lu_anim_t* anim)
{
    if(!anim)
    {
        return;
    }
    if(!anim->timer)
    {
        anim->timer = lv_timer_create(anim_timer_cb, LV_DEF_REFR_PERIOD,anim);
    }
}

void lu_anim_stop(lu_anim_t* anim)
{
    if(!anim)
    {
        return;
    }
    if(!anim->timer)
    {
        return;
    }
    lv_timer_delete(anim->timer);
    anim->timer = NULL;
}

bool lu_anim_is_finish(lu_anim_t* anim)
{
    if(!anim)
    {
        return false;
    }
    int32_t curr = anim->get_cb(anim->obj);
    return curr == lu_anim_get_value(anim);
}

int32_t lu_anim_lv_anim_path_cb(const lv_anim_t *anim)
{
    //贝塞尔曲线
    int32_t t = lv_map(anim->act_time, 0, anim->duration, 0, 1024);
    // int32_t step = lv_anim_path_cb3(t, 0, 0, 1024, 1024);
    int32_t step = lv_bezier3(t, 0, 1024, 1024, 1024);
    int32_t new_value;
    new_value = step * (anim->end_value - anim->start_value);
    new_value = new_value >> 10;
    new_value += anim->start_value;
    return new_value;
}

void lu_anim_lv_anim_auto_cb(lv_anim_t *anim)
{
    lv_anim_delete(anim->var, anim->exec_cb);
}

void lu_anim_lv_anim_auto_wd_cb(lv_anim_t *anim)
{
    lv_anim_delete(anim->var, anim->exec_cb);
    lv_obj_delete(anim->var);
}

int32_t lu_anim_lv_get_bg_opa(const lv_obj_t* obj)
{
    return lv_obj_get_style_bg_opa(obj, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void lu_anim_lv_set_bg_opa(lv_obj_t* obj, int32_t v)
{
    lv_obj_set_style_bg_opa(obj, v, LV_PART_MAIN | LV_STATE_DEFAULT);
}
