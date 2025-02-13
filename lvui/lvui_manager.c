#include"lvui_manager.h"
#ifdef LVUI_MANAGER_H

//manager info
typedef struct manager_t
{
    uint16_t hor;
    uint16_t ver;
}manager_t;
static manager_t manager;

//manager init
static void manager_init(uint16_t hor,uint16_t ver);
//lvui manager screen
void lvui_manager_screen(uint16_t*hor,uint16_t*ver);
//lvui to launcher
void lvui_to_launcher();
//lvui manager init
void lvui_manager_init(uint16_t hor,uint16_t ver,float size);

//manager init
static void manager_init(uint16_t hor,uint16_t ver)
{
    manager.hor=hor;
    manager.ver=ver;
}

//lvui manager screen
void lvui_manager_screen(uint16_t*hor,uint16_t*ver)
{
    if(hor)
    {
        *hor=manager.hor;
    }
    if(ver)
    {
        *ver=manager.ver;
    }
}

//lvui manager init
void lvui_manager_init(uint16_t hor,uint16_t ver,float size)
{
    //manager init
    manager_init(hor,ver);

    //lvui display init
    lvui_display_init(hor,ver,size);

    //lvui gesture init
    lvui_gesture_init();

    //lvui statusbar init
    lvui_statusbar_init();

    //lvui controlcenter init
    lvui_controlcenter_init();

    //lvui launcher init
    lvui_launcher_init();

    //load launcher
    lv_obj_t*screen=lvui_launcher_load();
    //load statusbar
    lv_obj_set_parent(lvui_statusbar_load(),screen);
    //load controlcenter
    lv_obj_set_parent(lvui_controlcenter_load(),screen);
    //load screen
    lv_scr_load_anim(screen,LV_SCR_LOAD_ANIM_FADE_IN,200,0,false);
    return;
}

#endif//#ifndef LVUI_MANAGER_H
