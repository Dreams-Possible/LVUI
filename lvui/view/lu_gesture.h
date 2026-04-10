#ifndef LU_GESTURE_H
#define LU_GESTURE_H

#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/frame/disp/lu_disp.h"
#include "lvui/frame/theme/lu_theme.h"
#include "lvui/frame/input/lu_touch.h"

typedef enum
{
    LU_GESTURE_SWIPE_UP = 0,
    LU_GESTURE_LONG_PRESS,
    LU_GESTURE_DOUBLE_CLICK,
    LU_GESTURE_EVENT_SUMNUM,
}lu_gesture_event_t;
typedef struct lu_gesture_t lu_gesture_t;
typedef void (*lu_gesture_cb_t)(lu_gesture_t* gesture);

int32_t lu_gesture_get_height(lu_gesture_t* gesture);
lu_gesture_t* lu_gesture_create(lu_disp_t* lu_disp, lu_theme_t* lu_theme, lu_touch_t* lu_touch, lv_obj_t* parent);
void lu_gesture_delete(lu_gesture_t** gesture);
void lu_gesture_set_cb(lu_gesture_t* gesture, lu_gesture_event_t event, lu_gesture_cb_t cb, void* user_data);
void* lu_gesture_get_user_data(lu_gesture_t* gesture, lu_gesture_event_t event);

#endif
