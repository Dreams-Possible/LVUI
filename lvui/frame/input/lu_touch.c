#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/misc/lu_log.h"
#include "lvui/plat/lu_mem.h"
#include "lvui/misc/lu_sll.h"
#include <stdint.h>

typedef struct lu_touch_t
{
    lv_indev_t* indev;
    lu_sll_node_t* node;
    lv_indev_read_cb_t org_cb;
    bool status;
    int32_t press_time;
    int32_t release_time;
    int32_t press_x;
    int32_t press_y;
    int32_t release_x;
    int32_t release_y;
    int32_t now_x;
    int32_t now_y;
    int32_t moved_x;
    int32_t moved_y;
    bool long_press;
    bool double_click;
}lu_touch_t;

typedef struct
{
    lu_sll_t* touch_list;
}touch_data_t;
static touch_data_t touch_data={0};

static void inject_input_cb(lv_indev_t* lv_indev, lv_indev_data_t* data);
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

static void inject_input_cb(lv_indev_t* lv_indev, lv_indev_data_t* lv_indev_data)
{
    lu_sll_node_t* node=lu_sll_get_head(touch_data.touch_list);
    if(!node)
    {
        return;
    }
    lu_touch_t* touch=NULL;
    while(node)
    {
        touch=(lu_touch_t*)lu_sll_get_data(node);
        if(touch->indev==lv_indev)
        {
            break;
        }
        node=lu_sll_get_next(node);
    }
    if(!touch)
    {
        return;
    }
    touch->org_cb(lv_indev, lv_indev_data);
    lv_indev_state_t status=lv_indev_data->state;
    lv_point_t point = lv_indev_data->point;
    if(status==LV_INDEV_STATE_PRESSED)
    {
        if(!touch->status)
        {
            touch->press_time=lv_tick_get();
            touch->status=true;
            touch->press_x=point.x;
            touch->press_y=point.y;
        }
        touch->now_x=point.x;
        touch->now_y=point.y;
        touch->moved_x=point.x-touch->press_x;
        touch->moved_y=point.y-touch->press_y;
        if(lv_tick_get()-touch->press_time>500)
        {
            touch->long_press=true;
            // LU_LOG("touch long press");
        }else
        {
            touch->long_press=false;
        }
    }else
    if(status==LV_INDEV_STATE_RELEASED)
    {
        if(touch->status)
        {
            touch->release_time=lv_tick_get();
            touch->status=false;
            touch->release_x=point.x;
            touch->release_y=point.y;
            static int32_t last_released_time=0;
            static lv_point_t last_release_pos={0,0};
            static lv_point_t release_pos={0,0};
            static int32_t released_time = 0;
            if(touch->moved_x<10&&touch->moved_y<10)
            {
                last_released_time = released_time;
                last_release_pos.x=release_pos.x;
                last_release_pos.y=release_pos.y;
                released_time=lv_tick_get();
                release_pos.x=touch->release_x;
                release_pos.y=touch->release_y;
                // LU_LOG("touch release time: %d", released_time);
            }
            // LU_LOG("touch press time: %d", touch->press_time);
            // LU_LOG("touch moved x: %d", touch->moved_x);
            // LU_LOG("touch moved y: %d", touch->moved_y);
            if(released_time-last_released_time<400&&release_pos.x-last_release_pos.x<10&&release_pos.y-last_release_pos.y<10)
            {
                touch->double_click=true;
                // LU_LOG("touch double click");
            }else
            {
                touch->double_click=false;
            }
        }
    }
}

bool lu_touch_init()
{
    if(!touch_data.touch_list)
    {
        touch_data.touch_list=lu_sll_create();
    }
    if(!touch_data.touch_list)
    {
        return false;
    }
    return true;
}

void lu_touch_deinit()
{
    if(touch_data.touch_list)
    {
        lu_sll_delete(&touch_data.touch_list);
    }
}

lu_touch_t* lu_touch_create(lv_indev_t* indev_touch)
{
    if(!indev_touch)
    {
        return NULL;
    }
    lu_touch_t* touch = (lu_touch_t*)lu_calloc(1, sizeof(lu_touch_t));
    if(!touch)
    {
        return NULL;
    }
    touch->indev = indev_touch;
    touch->org_cb = lv_indev_get_read_cb(touch->indev);
    touch->node=lu_sll_append(touch_data.touch_list,touch);
    lv_indev_set_read_cb(touch->indev,inject_input_cb);
    return touch;
}

void lu_touch_delete(lu_touch_t** touch)
{
    if(!touch || !*touch)
    {
        return;
    }
    lv_indev_set_read_cb((*touch)->indev,(*touch)->org_cb);
    int32_t index=lu_sll_get_index(touch_data.touch_list,(*touch)->node);
    if(index>=0)
    {
        lu_sll_remove(touch_data.touch_list,index);
    }
    lu_free(*touch);
    *touch = NULL;
}

bool lu_touch_get_state(lu_touch_t* touch)
{
    if(!touch)
    {
        return false;
    }
    return touch->status;
}

int32_t lu_touch_get_press_time(lu_touch_t* touch)
{
    if(!touch)
    {
        return 0;
    }
    return touch->press_time;
}

int32_t lu_touch_get_release_time(lu_touch_t* touch)
{
    if(!touch)
    {
        return 0;
    }
    return touch->release_time;
}

int32_t lu_touch_get_press_x(lu_touch_t* touch)
{
    if(!touch)
    {
        return 0;
    }
    return touch->press_x;
}

int32_t lu_touch_get_press_y(lu_touch_t* touch)
{
    if(!touch)
    {
        return 0;
    }
    return touch->press_y;
}

int32_t lu_touch_get_release_x(lu_touch_t* touch)
{
    if(!touch)
    {
        return 0;
    }
    return touch->release_x;
}

int32_t lu_touch_get_release_y(lu_touch_t* touch)
{
    if(!touch)
    {
        return 0;
    }
    return touch->release_y;
}

int32_t lu_touch_get_now_x(lu_touch_t* touch)
{
    if(!touch)
    {
        return 0;
    }
    return touch->now_x;
}

int32_t lu_touch_get_now_y(lu_touch_t* touch)
{
    if(!touch)
    {
        return 0;
    }
    return touch->now_y;
}

int32_t lu_touch_get_moved_x(lu_touch_t* touch)
{
    if(!touch)
    {
        return 0;
    }
    return touch->moved_x;
}

int32_t lu_touch_get_moved_y(lu_touch_t* touch)
{
    if(!touch)
    {
        return 0;
    }
    return touch->moved_y;
}

bool lu_touch_get_long_press(lu_touch_t* touch)
{
    if(!touch)
    {
        return false;
    }
    return touch->long_press;
}

bool lu_touch_get_double_click(lu_touch_t* touch)
{
    if(!touch)
    {
        return false;
    }
    return touch->double_click;
}
