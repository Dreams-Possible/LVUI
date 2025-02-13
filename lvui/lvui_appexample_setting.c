#include"lvui_appexample_setting.h"
#ifdef LVUI_APPEXAMPLE_SETTING_H

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
    layout_info_t appexample_setting;
    layout_info_t list1;
}ui_layout_t;
static ui_layout_t ui_layout;

//ui widget
typedef struct ui_widget_t
{
    lv_obj_t*base;
    lv_obj_t*appexample_setting;
    lv_obj_t*list1;
}ui_widget_t;
static ui_widget_t ui_widget;

//build base
static void build_base(uint16_t hor,uint16_t ver);
//build appexample setting
static void build_appexample();
//lvui init appexample setting
void lvui_appexample_setting_init();
//lvui load appexample setting
lv_obj_t*lvui_appexample_setting_load();

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

//event handler
void event_handler()
{
    return;
}

//build appexample setting
static void build_appexample()
{
    //creat
    //appexample setting
    ui_widget.appexample_setting=lv_obj_create(ui_widget.base);
    //list1
        ui_widget.list1=lv_list_create(ui_widget.appexample_setting);
        lv_obj_t*btn=NULL;
        lv_list_add_text(ui_widget.list1,"File");
        btn=lv_list_add_button(ui_widget.list1,LV_SYMBOL_FILE,"New");
        lv_obj_add_event_cb(btn,event_handler,LV_EVENT_CLICKED, NULL);
        btn=lv_list_add_button(ui_widget.list1,LV_SYMBOL_DIRECTORY,"Open");
        lv_obj_add_event_cb(btn,event_handler,LV_EVENT_CLICKED, NULL);
        btn=lv_list_add_button(ui_widget.list1,LV_SYMBOL_SAVE,"Save");
        lv_obj_add_event_cb(btn,event_handler,LV_EVENT_CLICKED, NULL);
        btn=lv_list_add_button(ui_widget.list1,LV_SYMBOL_CLOSE,"Delete");
        lv_obj_add_event_cb(btn,event_handler,LV_EVENT_CLICKED, NULL);
        btn=lv_list_add_button(ui_widget.list1,LV_SYMBOL_EDIT,"Edit");
        lv_obj_add_event_cb(btn,event_handler,LV_EVENT_CLICKED, NULL);
        lv_list_add_text(ui_widget.list1,"Connectivity");
        btn=lv_list_add_button(ui_widget.list1,LV_SYMBOL_BLUETOOTH,"Bluetooth");
        lv_obj_add_event_cb(btn,event_handler,LV_EVENT_CLICKED, NULL);
        btn=lv_list_add_button(ui_widget.list1,LV_SYMBOL_GPS,"Navigation");
        lv_obj_add_event_cb(btn,event_handler,LV_EVENT_CLICKED, NULL);
        btn=lv_list_add_button(ui_widget.list1,LV_SYMBOL_USB,"USB");
        lv_obj_add_event_cb(btn,event_handler,LV_EVENT_CLICKED, NULL);
        btn=lv_list_add_button(ui_widget.list1,LV_SYMBOL_BATTERY_FULL,"Battery");
        lv_obj_add_event_cb(btn,event_handler,LV_EVENT_CLICKED, NULL);
        lv_list_add_text(ui_widget.list1,"Exit");
        btn=lv_list_add_button(ui_widget.list1,LV_SYMBOL_OK,"Apply");
        lv_obj_add_event_cb(btn,event_handler,LV_EVENT_CLICKED, NULL);
        btn=lv_list_add_button(ui_widget.list1,LV_SYMBOL_CLOSE,"Close");
        lv_obj_add_event_cb(btn,event_handler,LV_EVENT_CLICKED, NULL);
    //layout
    //appexample setting
    ui_layout.appexample_setting.left=0;
    ui_layout.appexample_setting.top=0;
    ui_layout.appexample_setting.hor=ui_layout.base.hor;
    ui_layout.appexample_setting.ver=ui_layout.base.ver;
        //list1
        ui_layout.list1.left=0;
        ui_layout.list1.top=lvui_statusbar_height();
        ui_layout.list1.hor=ui_layout.base.hor;
        ui_layout.list1.ver=ui_layout.base.ver-lvui_statusbar_height();
    //init
    //appexample setting
    lv_obj_set_pos(ui_widget.appexample_setting,ui_layout.appexample_setting.left,ui_layout.appexample_setting.top);
    lv_obj_set_size(ui_widget.appexample_setting,ui_layout.appexample_setting.hor,ui_layout.appexample_setting.ver);
    lv_obj_set_style_pad_left(ui_widget.appexample_setting,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_widget.appexample_setting,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_widget.appexample_setting,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_widget.appexample_setting,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_widget.appexample_setting,255,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_widget.appexample_setting,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_widget.appexample_setting,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        //list1
        lv_obj_set_pos(ui_widget.list1,ui_layout.list1.left,ui_layout.list1.top);
        lv_obj_set_size(ui_widget.list1,ui_layout.list1.hor,ui_layout.list1.ver);
}

//lvui init appexample setting
void lvui_appexample_setting_init()
{
    uint16_t hor=0;
    uint16_t ver=0;
    lvui_manager_screen(&hor,&ver);
    //build base
    build_base(hor,ver);
    //build appexample setting
    build_appexample();
}

//lvui load appexample setting
lv_obj_t*lvui_appexample_setting_load()
{
    return ui_widget.appexample_setting;
}

#endif//#ifndef LVUI_APPEXAMPLE_SETTING_H
