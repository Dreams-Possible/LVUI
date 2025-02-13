#include"lvui_appexample.h"
#ifdef LVUI_APPEXAMPLE_H

//widget info
typedef struct layout_info_t
{
    uint16_t left;
    uint16_t top;
    uint16_t hor;
    uint16_t ver;
}layout_info_t;

//ui layout
typedef struct ui_layout_t
{
    layout_info_t base;
    layout_info_t appexample;
}ui_layout_t;
static ui_layout_t ui_layout;

//ui widget
typedef struct ui_widget_t
{
    lv_obj_t*base;
    lv_obj_t*appexample;
}ui_widget_t;
static ui_widget_t ui_widget;

//build base
static void build_base(uint16_t hor,uint16_t ver);
//build appexample
static void build_appexample();
//lvui init appexample
void lvui_appexample_init();
//lvui load appexample
lv_obj_t*lvui_appexample_load();

//build base
static void build_base(uint16_t hor,uint16_t ver)
{
    ui_widget.base = lv_obj_create(0);
    ui_layout.base.left=0;
    ui_layout.base.top=0;
    ui_layout.base.hor=hor;
    ui_layout.base.ver=ver;
    lv_obj_set_pos(ui_widget.base,ui_layout.base.left,ui_layout.base.top);
    lv_obj_set_size(ui_widget.base,ui_layout.base.hor,ui_layout.base.ver);
}

//build appexample
static void build_appexample()
{
    //creat
    //appexample
    ui_widget.appexample=lv_obj_create(ui_widget.base);
    //layout
    //appexample
    ui_layout.appexample.left=0;
    ui_layout.appexample.top=0;
    ui_layout.appexample.hor=ui_layout.base.hor;
    ui_layout.appexample.ver=ui_layout.base.ver;
    //init
    //appexample
    lv_obj_set_pos(ui_widget.appexample,ui_layout.appexample.left,ui_layout.appexample.top);
    lv_obj_set_size(ui_widget.appexample,ui_layout.appexample.hor,ui_layout.appexample.ver);
}

//lvui init appexample
void lvui_appexample_init()
{
    uint16_t hor=0;
    uint16_t ver=0;
    lvui_manager_screen(&hor,&ver);
    //build base
    build_base(hor,ver);
    //build appexample
    build_appexample();
}

//lvui load appexample
lv_obj_t*lvui_appexample_load()
{
    return ui_widget.appexample;
}

#endif//#ifndef LVUI_APPEXAMPLE_H
