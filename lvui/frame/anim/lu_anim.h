#ifndef LU_ANIM_H
#define LU_ANIM_H

#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"

typedef struct lu_anim_param_t
{
    float kp;
    float kd;
}lu_anim_param_t;
typedef int32_t (*lu_anim_get_cb_t)(const lv_obj_t *);
typedef void (*lu_anim_set_cb_t)(lv_obj_t *, const int32_t);
typedef struct lu_anim_t lu_anim_t;

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

#endif
