#ifndef LU_TOUCH_H
#define LU_TOUCH_H

#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"

typedef struct lu_touch_t lu_touch_t;

bool lu_touch_init();
void lu_touch_deinit();
lu_touch_t* lu_touch_create(lv_indev_t* indev_touch);
void lu_touch_delete(lu_touch_t** touch);
bool lu_touch_get_state(lu_touch_t* touch);
int32_t lu_touch_get_press_time(lu_touch_t* touch);
int32_t lu_touch_get_release_time(lu_touch_t* touch);
int32_t lu_touch_get_press_x(lu_touch_t* touch);
int32_t lu_touch_get_press_y(lu_touch_t* touch);
int32_t lu_touch_get_release_x(lu_touch_t* touch);
int32_t lu_touch_get_release_y(lu_touch_t* touch);
int32_t lu_touch_get_now_x(lu_touch_t* touch);
int32_t lu_touch_get_now_y(lu_touch_t* touch);
int32_t lu_touch_get_moved_x(lu_touch_t* touch);
int32_t lu_touch_get_moved_y(lu_touch_t* touch);
bool lu_touch_get_double_click(lu_touch_t* touch);
bool lu_touch_get_long_press(lu_touch_t* touch);

#endif
