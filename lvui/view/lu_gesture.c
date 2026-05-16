#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/plat/lu_mem.h"

#include "lvui/misc/lu_log.h"
#include "lvui/misc/lu_pdc.h"
#include "lvui/misc/lu_sll.h"

#include "lvui/frame/anim/lu_anim.h"
#include "lvui/frame/disp/lu_disp.h"
#include "lvui/frame/disp/lu_font.h"
#include "lvui/frame/input/lu_touch.h"
#include "lvui/frame/theme/lu_color.h"
#include "lvui/frame/theme/lu_theme.h"
#include "lvui/frame/widget/lu_widget.h"
#include "lvui/frame/widget/lu_layout.h"

#include "lu_gesture.h"

#include <stdio.h>

typedef struct lu_gesture_t
{
    lu_disp_t* lu_disp;
    lu_theme_t* lu_theme;
    lu_touch_t* lu_touch;

    lv_obj_t* contain;
    lv_obj_t* bg;

    lv_obj_t* bar_contain;
    lv_obj_t* bar;
    lv_obj_t* bar_mask;

    lu_anim_t* bar_mask_anim;
    lv_timer_t* timer;

    bool is_pressed;

    lu_gesture_cb_t cb[LU_GESTURE_EVENT_SUMNUM];
    void* user_data[LU_GESTURE_EVENT_SUMNUM];
}lu_gesture_t;

static void gesture_event_cb(lu_gesture_t* gesture, lu_gesture_event_t event);
static void bar_mask_event_cb(lv_event_t *event);
static void gesture_timer_cb(lv_timer_t* timer);
int32_t lu_gesture_get_height(lu_gesture_t* gesture);
lu_gesture_t* lu_gesture_create(lu_disp_t* lu_disp, lu_theme_t* lu_theme, lu_touch_t* lu_touch, lv_obj_t* parent);
void lu_gesture_delete(lu_gesture_t** gesture);
void lu_gesture_set_cb(lu_gesture_t* gesture, lu_gesture_event_t event, lu_gesture_cb_t cb, void* user_data);
void* lu_gesture_get_user_data(lu_gesture_t* gesture, lu_gesture_event_t event);

static void gesture_event_cb(lu_gesture_t* gesture, lu_gesture_event_t event)
{
    if(!gesture)
    {
        return;
    }
    if(gesture->cb[event])
    {
        gesture->cb[event](gesture);
    }
    // LU_LOG("gesture_event_cb: %p", gesture);
}

static void bar_mask_event_cb(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lu_gesture_t* gesture = (lu_gesture_t*)lv_event_get_user_data(event);
    switch(code)
    {
        case LV_EVENT_PRESSED:
            gesture->is_pressed = true;
            // lu_anim_set_value(gesture->bar_mask_anim, lu_gesture_get_height(gesture)-lu_touch_get_moved_y(gesture->lu_touch));
        break;
        case LV_EVENT_RELEASED:
            gesture->is_pressed = false;
            // LU_LOG("bar_mask_event_cb: %d", lu_touch_get_moved_y(gesture->lu_touch));
            // LU_LOG("bar_mask_event_cb: %f", lu_disp_get_ver(gesture->lu_disp)*0.1);
            if(lu_touch_get_moved_y(gesture->lu_touch)<-lu_disp_get_ver(gesture->lu_disp)*0.15)
            {
                gesture_event_cb(gesture, LU_GESTURE_SWIPE_UP);
            }else
            if(lu_touch_get_double_click(gesture->lu_touch))
            {
                gesture_event_cb(gesture, LU_GESTURE_DOUBLE_CLICK);
            }
            // lu_anim_set_value(gesture->bar_mask_anim, lu_gesture_get_height(gesture));
        break;
        default:
        break;
    }
    // LU_LOG("bar_mask_event_cb: %d", lu_anim_get_value(gesture->bar_mask_anim));
}

static void gesture_timer_cb(lv_timer_t* timer)
{
    lu_gesture_t* gesture = (lu_gesture_t*)lv_timer_get_user_data(timer);
    if(gesture->is_pressed)
    {
        if(lu_touch_get_long_press(gesture->lu_touch))
        {
            gesture_event_cb(gesture, LU_GESTURE_LONG_PRESS);
        }
        lu_anim_set_value(gesture->bar_mask_anim, lu_gesture_get_height(gesture)-lu_touch_get_moved_y(gesture->lu_touch));
    }else
    {
        lu_anim_set_value(gesture->bar_mask_anim, lu_gesture_get_height(gesture));
    }
}

int32_t lu_gesture_get_height(lu_gesture_t* gesture)
{
    if(!gesture)
    {
        return 0;
    }
    return lu_disp_get_ver(gesture->lu_disp)*0.04;
}

lu_gesture_t* lu_gesture_create(lu_disp_t* lu_disp, lu_theme_t* lu_theme, lu_touch_t* lu_touch, lv_obj_t* parent)
{
    if(!lu_disp||!lu_theme||!lu_touch||!parent)
    {
        return NULL;
    }
    lu_gesture_t* gesture = (lu_gesture_t*)lu_calloc(1, sizeof(lu_gesture_t));
    if(!gesture)
    {
        return NULL;
    }
    gesture->lu_disp = lu_disp;
    gesture->lu_theme = lu_theme;
    gesture->lu_touch = lu_touch;
    lv_obj_t* obj = lu_widget_obj_init(parent);
    if(!obj)
    {
        lu_gesture_delete(&gesture);
        return NULL;
    }
    gesture->contain = obj;
    lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(obj, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_align(obj, LV_ALIGN_BOTTOM_MID);
    // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {
        {
            lv_obj_t* obj = lu_widget_obj_init(gesture->contain);
            if(!obj)
            {
                lu_gesture_delete(&gesture);
                return NULL;
            }
            gesture->bg = obj;
            lv_obj_set_size(obj, LV_PCT(100), lu_gesture_get_height(gesture));
            // lv_obj_set_size(obj, lu_disp_get_res(gesture->lu_disp).x, 100);
            lv_obj_set_style_bg_opa(obj, LV_OPA_30, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_align(obj, LV_ALIGN_BOTTOM_MID);
            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lu_theme_info_t theme_info =
            {
                .self_type = LU_THEME_TYPE_MODE,
                .flag = LU_THEME_FLAG_SELF,
            };
            lu_theme_add_obj(gesture->lu_theme, obj, &theme_info);
        }
        {
            lv_obj_t* obj = lu_widget_obj_init(gesture->contain);
            if(!obj)
            {
                lu_gesture_delete(&gesture);
                return NULL;
            }
            gesture->bar_contain = obj;
            lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
            // lv_obj_set_size(obj, lu_disp_get_res(gesture->lu_disp).x, 20);
            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_align(obj, LV_ALIGN_BOTTOM_MID);
            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            {
                lv_obj_t* obj = lu_widget_btn_init(gesture->bar_contain);
                if(!obj)
                {
                    lu_gesture_delete(&gesture);
                    return NULL;
                }
                gesture->bar = obj;
                lv_obj_set_size(obj, LV_PCT(40), lu_gesture_get_height(gesture)*0.4);
                lv_obj_set_style_bg_opa(obj, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_align(obj, LV_ALIGN_CENTER);
                // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                lu_theme_info_t info =
                {
                    .self_type = LU_THEME_TYPE_INVMODE,
                    .flag = LU_THEME_FLAG_SELF,
                };
                lu_theme_add_obj(gesture->lu_theme, obj, &info);
            }
            {
                lv_obj_t* obj = lu_widget_btn_init(gesture->bar_contain);
                if(!obj)
                {
                    lu_gesture_delete(&gesture);
                    return NULL;
                }
                gesture->bar_mask = obj;
                lv_obj_set_size(obj, LV_PCT(100), lu_gesture_get_height(gesture));
                lv_obj_set_style_bg_opa(obj, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_align(obj, LV_ALIGN_BOTTOM_MID);
                // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                lv_obj_add_event_cb(obj, bar_mask_event_cb, LV_EVENT_PRESSED, gesture);
                lv_obj_add_event_cb(obj, bar_mask_event_cb, LV_EVENT_RELEASED, gesture);
            
                // lv_obj_set_style_bg_opa(obj, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
                // lu_theme_add_obj(gesture->lu_theme, obj, LU_THEME_TYPE_OBJ);
            }
        }
    }
    gesture->bar_mask_anim = lu_anim_create(gesture->bar_mask);
    if(!gesture->bar_mask_anim)
    {
        lu_gesture_delete(&gesture);
        return NULL;
    }
    lu_anim_param_t param =
    {
        .kp = 0.2,
        .kd = 0,
    };
    lu_anim_set_param(gesture->bar_mask_anim, &param);
    lu_anim_set_get_cb(gesture->bar_mask_anim, lv_obj_get_height);
    lu_anim_set_set_cb(gesture->bar_mask_anim, lv_obj_set_height);
    lu_anim_set_value(gesture->bar_mask_anim, lu_gesture_get_height(gesture));
    lu_anim_start(gesture->bar_mask_anim);
    gesture->timer = lv_timer_create(gesture_timer_cb, LV_DEF_REFR_PERIOD, gesture);
    if(!gesture->timer)
    {
        lu_gesture_delete(&gesture);
        return NULL;
    }
    return gesture;
}

void lu_gesture_delete(lu_gesture_t** gesture)
{
    if(!gesture||!*gesture)
    {
        return;
    }
    if((*gesture)->timer)
    {
        lv_timer_delete((*gesture)->timer);
    }
    if((*gesture)->bar_mask_anim)
    {
        lu_anim_delete(&(*gesture)->bar_mask_anim);
    }
    if((*gesture)->contain)
    {
        lu_theme_remove_obj((*gesture)->lu_theme, (*gesture)->bg);
        lv_obj_delete((*gesture)->contain);
    }
    lu_free(*gesture);
    *gesture = NULL;
}

void lu_gesture_set_cb(lu_gesture_t* gesture, lu_gesture_event_t event, lu_gesture_cb_t cb, void* user_data)
{
    if(!gesture)
    {
        return;
    }
    gesture->cb[event] = cb;
    gesture->user_data[event] = user_data;
}

void* lu_gesture_get_user_data(lu_gesture_t* gesture, lu_gesture_event_t event)
{
    if(!gesture)
    {
        return NULL;
    }
    return gesture->user_data[event];
}
