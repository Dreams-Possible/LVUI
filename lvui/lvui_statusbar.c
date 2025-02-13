#include"lvui_statusbar.h"
#ifdef LVUI_STATUSBAR_H

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
    layout_info_t statusbar;
    layout_info_t statusbar_time;
    layout_info_t statusbar_battery;
}ui_layout_t;
static ui_layout_t ui_layout;

//ui widget
typedef struct ui_widget_t
{
    lv_obj_t*base;
    lv_obj_t*statusbar;
    lv_obj_t*statusbar_time;
    lv_obj_t*statusbar_battery;
}ui_widget_t;
static ui_widget_t ui_widget;

//statusbar layout
typedef struct statusbar_layout_t
{
    uint16_t ver;
}statusbar_layout_t;
statusbar_layout_t statusbar_layout;

//build base
static void build_base(uint16_t hor,uint16_t ver);
//statusbar layout init
static void statusbar_layout_init();
//build statusbar
static void build_statusbar();
//lvui statusbar init
void lvui_statusbar_init();
//lvui statusbar load
lv_obj_t*lvui_statusbar_load();
//lvui statusbar height
uint8_t lvui_statusbar_height();

//build base
static void build_base(uint16_t hor,uint16_t ver)
{
    ui_widget.base=lv_obj_create(0);
    ui_layout.base.left=0;
    ui_layout.base.top=0;
    ui_layout.base.hor=hor;
    ui_layout.base.ver=ver;
    lv_obj_set_pos(ui_widget.base,ui_layout.base.left,ui_layout.base.top);
    lv_obj_set_size(ui_widget.base,ui_layout.base.hor,ui_layout.base.ver);
}

//statusbar layout init
static void statusbar_layout_init()
{
    if(lvui_display_level()==LVUI_DISPLAY_LEVEL_SMALL)
    {
        statusbar_layout.ver=lvui_display_change(32);
    }else
    if(lvui_display_level()==LVUI_DISPLAY_LEVEL_NOMALL)
    {
        statusbar_layout.ver=lvui_display_change(34);
    }else
    {
        statusbar_layout.ver=lvui_display_change(36);
    }
    //avoid occupying too much screen space
    if(statusbar_layout.ver>ui_layout.base.ver/4)
    {
        statusbar_layout.ver=0;
    }
}

//build statusbar
static void build_statusbar()
{
    //creat
    //statusbar
    ui_widget.statusbar=lv_obj_create(ui_widget.base);
    //statusbar_time
    ui_widget.statusbar_time=lv_label_create(ui_widget.statusbar);
    //statusbar_battery
    ui_widget.statusbar_battery=lv_label_create(ui_widget.statusbar);
    //layout
    //statusbar
    ui_layout.statusbar.left=0;
    ui_layout.statusbar.top=0;
    ui_layout.statusbar.hor=ui_layout.base.hor;
    ui_layout.statusbar.ver=lvui_statusbar_height();
    //statusbar time
    lv_obj_update_layout(ui_widget.statusbar_time);
    uint16_t st_ver=lv_obj_get_height(ui_widget.statusbar_time);
    ui_layout.statusbar_time.left=ui_layout.base.hor/40;
    ui_layout.statusbar_time.top=ui_layout.statusbar.ver/2-st_ver/2;
    ui_layout.statusbar_time.hor=0;
    ui_layout.statusbar_time.ver=0;
    //statusbar battery
    lv_obj_update_layout(ui_widget.statusbar_battery);
    uint16_t sb_hor=lv_obj_get_width(ui_widget.statusbar_battery);
    uint16_t sb_ver=lv_obj_get_height(ui_widget.statusbar_battery);
    ui_layout.statusbar_battery.left=ui_layout.base.hor-ui_layout.base.hor/40-sb_hor;
    ui_layout.statusbar_battery.top=ui_layout.statusbar.ver/2-sb_ver/2;
    ui_layout.statusbar_battery.hor=0;
    ui_layout.statusbar_battery.ver=0;
    //init
    //statusbar
    lv_obj_set_pos(ui_widget.statusbar,ui_layout.statusbar.left,ui_layout.statusbar.top);
    lv_obj_set_size(ui_widget.statusbar,ui_layout.statusbar.hor,ui_layout.statusbar.ver);
    lv_obj_set_style_pad_left(ui_widget.statusbar,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_widget.statusbar,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_widget.statusbar,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_widget.statusbar,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_widget.statusbar,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_widget.statusbar,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_widget.statusbar,lv_color_hex(0xff000000),LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_widget.statusbar,100,LV_PART_MAIN|LV_STATE_DEFAULT);
    //statusbar time
    lv_obj_set_pos(ui_widget.statusbar_time,ui_layout.statusbar_time.left,ui_layout.statusbar_time.top);
    lv_obj_set_size(ui_widget.statusbar_time,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
    lv_label_set_text(ui_widget.statusbar_time,"9:30");
    //statusbar battery
    lv_obj_set_pos(ui_widget.statusbar_battery,ui_layout.statusbar_battery.left,ui_layout.statusbar_battery.top);
    lv_obj_set_size(ui_widget.statusbar_battery,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
    lv_label_set_text(ui_widget.statusbar_battery,"99%");
}

//lvui statusbar init
void lvui_statusbar_init()
{
    uint16_t hor=0;
    uint16_t ver=0;
    lvui_manager_screen(&hor,&ver);
    //build base
    build_base(hor,ver);
    //statusbar layout init
    statusbar_layout_init();
    //build statusbar
    build_statusbar();
}

//lvui statusbar load
lv_obj_t*lvui_statusbar_load()
{
    return ui_widget.statusbar;
}

//lvui statusbar height
uint8_t lvui_statusbar_height()
{
    return statusbar_layout.ver;
}

#endif//#ifndef LVUI_STATUSBAR_H
