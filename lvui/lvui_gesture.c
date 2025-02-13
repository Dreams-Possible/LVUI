#include"lvui_gesture.h"
#ifdef LVUI_GESTURE_H

//gesture
typedef struct gesture_t
{
    uint16_t hor;
    uint16_t ver;
    lv_indev_t*touch;
    lv_indev_state_t state;
    lv_point_t point;
    uint16_t first_x;
    uint16_t first_y;
    uint16_t last_x;
    uint16_t last_y;
    int16_t move_x;
    int16_t move_y;
    uint8_t action;
}gesture_t;
static gesture_t gesture;

//gesture detection
static void gesture_detection(lv_timer_t*t);
//gesture action
static void gesture_action(lv_event_t*e);
//gesture launcher
static void gesture_launcher();
//lvui gesture init
void lvui_gesture_init();
//lvui gesture touch x
uint16_t lvui_gesture_touch_x();
//lvui gesture touch y
uint16_t lvui_gesture_touch_y();
//lvui gesture touch
uint8_t lvui_gesture_touch(uint16_t*x,uint16_t*y);
//lvui gesture first touch x
uint16_t lvui_gesture_first_touch_x();
//lvui gesture first touch y
uint16_t lvui_gesture_first_touch_y();
//lvui gesture first touch
void lvui_gesture_first_touch(uint16_t*x,uint16_t*y);
//lvui gesture last touch x
uint16_t lvui_gesture_last_touch_x();
//lvui gesture last touch y
uint16_t lvui_gesture_last_touch_y();
//lvui gesture last touch
void lvui_gesture_last_touch(uint16_t*x,uint16_t*y);
//lvui gesture move x
int16_t lvui_gesture_move_x();
//lvui gesture move y
int16_t lvui_gesture_move_y();
//lvui gesture move
void lvui_gesture_move(int16_t*x,int16_t*y);
//lvui gesture action
uint8_t lvui_gesture_action();

//gesture detection
static void gesture_detection(lv_timer_t*t)
{
    static uint8_t touch_state=0;
    gesture.state=lv_indev_get_state(gesture.touch);
    //if touch
    if(gesture.state==LV_INDEV_STATE_PR)
    {
        lv_indev_get_point(gesture.touch,&gesture.point);
        //record first touch
        if(touch_state==0)
        {
            gesture.first_x=gesture.point.x;
            gesture.first_y=gesture.point.y;
            gesture.move_x=0;
            gesture.move_y=0;
            touch_state=1;
        }
        //update last touch
        gesture.last_x=gesture.point.x;
        gesture.last_y=gesture.point.y;
        //process touch
        gesture.move_x=gesture.last_x-gesture.first_x;
        gesture.move_y=gesture.last_y-gesture.first_y;
    }else
    {
        touch_state=0;
    }
}

//gesture action
static void gesture_action(lv_event_t*e)
{
    uint16_t x_f=0;
    uint16_t y_f=0;
    lvui_gesture_first_touch(&x_f,&y_f);
    uint16_t x_l=0;
    uint16_t y_l=0;
    lvui_gesture_last_touch(&x_l,&y_l);
    int16_t x_m=0;
    int16_t y_m=0;
    lvui_gesture_move(&x_m,&y_m);
    //horizontal slide
    if(abs(x_l-x_f)>abs(y_l-y_f))
    {
        //threshold
        if(abs(x_l-x_f)>lvui_display_change(8))
        {
            if(x_l>x_f)
            {
                gesture.action=4;//right slide
            }else
            {
                gesture.action=3;//left slide
            }
        }else
        {
            gesture.action=0;//no slide
        }
    }else
    //vertical slide
    {
        //threshold
        if(abs(y_l-y_f)>lvui_display_change(8))
        {
            if(y_l>y_f)
            {
                gesture.action=2;//down slide
            }else
            {
                gesture.action=1;//up slide
            }
        }else
        {
            gesture.action=0;//no slide
        }
    }
}

//gesture launcher
static void gesture_launcher(lv_event_t*e)
{
    //threshold
    if(gesture.first_y>gesture.ver-lvui_display_change(32))
    {
        if(gesture.action==1)
        {
            // lvui_to_launcher();
            lvui_launcher_back();
            lvui_printf("lvui_launcher_back\n");
        }
    }
}

//lvui gesture init
void lvui_gesture_init()
{
    uint16_t hor=0;
    uint16_t ver=0;
    lvui_manager_screen(&hor,&ver);
    gesture.hor=hor;
    gesture.ver=ver;
    gesture.touch=lv_indev_get_next(NULL);
    gesture.state=0;
    gesture.point.x=0;
    gesture.point.y=0;
    gesture.first_x=0;
    gesture.first_y=0;
    gesture.last_x=0;
    gesture.last_y=0;
    gesture.move_x=0;
    gesture.move_y=0;
    gesture.action=0;
    lv_timer_create(gesture_detection,1,NULL);
    lv_indev_add_event_cb(lv_indev_get_next(NULL),gesture_action,LV_EVENT_RELEASED,NULL);
    lv_indev_add_event_cb(lv_indev_get_next(NULL),gesture_launcher,LV_EVENT_RELEASED,NULL);
}

//lvui gesture touch x
uint16_t lvui_gesture_touch_x()
{
    if(gesture.state==LV_INDEV_STATE_PR)
    {
        return gesture.point.x;
    }else
    {
        return 0;
    }
}

//lvui gesture touch y
uint16_t lvui_gesture_touch_y()
{
    if(gesture.state==LV_INDEV_STATE_PR)
    {
        return gesture.point.y;
    }else
    {
        return 0;
    }
}

//lvui gesture touch
uint8_t lvui_gesture_touch(uint16_t*x,uint16_t*y)
{
    if(gesture.state==LV_INDEV_STATE_PR)
    {
        if(x)
        {
            *x=gesture.point.x;
        }
        if(y)
        {
            *y=gesture.point.y;
        }
        return 1;
    }else
    {
        return 0;
    }
}

//lvui gesture first touch x
uint16_t lvui_gesture_first_touch_x()
{
    return gesture.first_x;
}

//lvui gesture first touch y
uint16_t lvui_gesture_first_touch_y()
{
    return gesture.first_y;
}

//lvui gesture first touch
void lvui_gesture_first_touch(uint16_t*x,uint16_t*y)
{
    if(x)
    {
        *x=gesture.first_x;
    }
    if(y)
    {
        *y=gesture.first_y;
    }
}

//lvui gesture last touch x
uint16_t lvui_gesture_last_touch_x()
{
    return gesture.last_x;
}

//lvui gesture last touch y
uint16_t lvui_gesture_last_touch_y()
{
    return gesture.last_y;
}

//lvui gesture last touch
void lvui_gesture_last_touch(uint16_t*x,uint16_t*y)
{
    if(x)
    {
        *x=gesture.last_x;
    }
    if(y)
    {
        *y=gesture.last_y;
    }
}

//lvui gesture move x
int16_t lvui_gesture_move_x()
{
    return gesture.move_x;
}

//lvui gesture move y
int16_t lvui_gesture_move_y()
{
    return gesture.move_y;
}

//lvui gesture move
void lvui_gesture_move(int16_t*x,int16_t*y)
{
    if(x)
    {
        *x=gesture.move_x;
    }
    if(y)
    {
        *y=gesture.move_y;
    }
}

//lvui gesture action
uint8_t lvui_gesture_action()
{
    return gesture.action;
}

#endif//#ifndef LVUI_GESTURE_H
