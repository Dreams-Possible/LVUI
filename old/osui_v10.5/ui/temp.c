// #include"osui_ui_anim.h"

// //UI动画LVGL类
// typedef struct osui_ui_anim_lv_t
// {
//     lv_anim_t anim;
//     lv_obj_t*obj;
//     int32_t p1;
//     int32_t p2;
// }osui_ui_anim_lv_t;

// //LVGL贝塞尔曲线
// static int32_t lv_bezier(const lv_anim_t * a);
// //动画LVGI初始化
// osui_ui_anim_lv_t*osui_ui_anim_lv_init();
// //动画LVGL反初始化
// void osui_ui_anim_lv_deinit(osui_ui_anim_lv_t* anim);
// //动画LVGL设置对象
// void osui_ui_anim_lv_set_obj(osui_ui_anim_lv_t* anim, lv_obj_t* obj);
// //动画LVGL设置时间
// void osui_ui_anim_lv_set_time(osui_ui_anim_lv_t* anim, lv_anim_exec_xcb_t type);
// //动画LVGL设置类型
// void osui_ui_anim_lv_set_type(osui_ui_anim_lv_t* anim, uint32_t time);
// //动画LVGL设置完成回调
// void osui_ui_anim_lv_set_complete_cb(osui_ui_anim_lv_t* anim, lv_anim_completed_cb_t cb);
// //动画LVGL设置贝塞尔参数
// void osui_ui_anim_lv_set_bezier_param(osui_ui_anim_lv_t* anim, int32_t p1, int32_t p2);
// //动画LVGL开始
// void osui_ui_anim_lv_start(osui_ui_anim_lv_t* anim);

// //LVGL贝塞尔曲线
// static int32_t lv_bezier(const lv_anim_t * a)
// {
//     uint32_t t = lv_map(a->act_time, 0, a->duration, 0, 1024);
//     int32_t step = lv_bezier3(t, 0, ((osui_ui_anim_lv_t*)(a->user_data))->p2, ((osui_ui_anim_lv_t*)(a->user_data))->p3, 1024);
//     int32_t new_value;
//     new_value = step * (a->end_value - a->start_value);
//     new_value = new_value >> 10;
//     new_value += a->start_value;
//     return new_value;
// }

// //动画LVGI初始化
// osui_ui_anim_lv_t*osui_ui_anim_lv_init()
// {
//     //申请内存
//     osui_ui_anim_lv_t*anim=(osui_ui_anim_lv_t*)osui_malloc(sizeof(osui_ui_anim_lv_t));
//     if(!anim)
//     {
//         osui_log("mem err");
//         return NULL;
//     }
//     memset(anim,0,sizeof(osui_ui_anim_lv_t));
//     //初始化
//     lv_anim_init(&anim->anim);
//     lv_anim_set_path_cb(&anim->anim, lv_bezier);
//     return anim;
// }

// //动画LVGL反初始化
// void osui_ui_anim_lv_deinit(osui_ui_anim_lv_t* anim)
// {
//     lv_anim_delete(&anim->anim,anim->obj);
//     osui_free(anim);
// }

// //动画LVGL设置对象
// void osui_ui_anim_lv_set_obj(osui_ui_anim_lv_t* anim, lv_obj_t* obj)
// {
//     anim->obj = obj;
//     lv_anim_set_var(&anim->anim, anim->obj);
// }

// //动画LVGL设置时间
// void osui_ui_anim_lv_set_time(osui_ui_anim_lv_t* anim, lv_anim_exec_xcb_t type)
// {
//     lv_anim_set_exec_cb(&anim->anim, type);
// }

// //动画LVGL设置类型
// void osui_ui_anim_lv_set_type(osui_ui_anim_lv_t* anim, uint32_t time)
// {
//     lv_anim_set_duration(&anim->anim, time);
// }

// //动画LVGL设置完成回调
// void osui_ui_anim_lv_set_complete_cb(osui_ui_anim_lv_t* anim, lv_anim_completed_cb_t cb)
// {
//     lv_anim_set_completed_cb(&anim->anim, cb);
// }

// //动画LVGL设置贝塞尔参数
// void osui_ui_anim_lv_set_bezier_param(osui_ui_anim_lv_t* anim, int32_t p1, int32_t p2)
// {
//     anim->p1 = p1;
//     anim->p2 = p2;
// }

// //动画LVGL开始
// void osui_ui_anim_lv_start(osui_ui_anim_lv_t* anim)
// {
//     lv_anim_start(&anim->anim);
// }
