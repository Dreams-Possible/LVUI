#if 1
#ifndef LVUI_LAUNCHER_H
#define LVUI_LAUNCHER_H

//include
#include"lvui_manager.h"

//lvui launcher init
void lvui_launcher_init();
//lvui launcher load
lv_obj_t*lvui_launcher_load();
//applist add
void applist_add(char*text,lv_obj_t*app);
//lvui launcher back
void lvui_launcher_back();

#endif//#ifndef LVUI_LAUNCHER_H
#endif//#if 1
