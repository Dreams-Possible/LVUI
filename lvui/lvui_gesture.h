#if 1
#ifndef LVUI_GESTURE_H
#define LVUI_GESTURE_H

//include
#include"lvui_manager.h"

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

#endif//#ifndef LVUI_GESTURE_H
#endif//#if 1
