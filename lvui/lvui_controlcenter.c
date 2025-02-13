#include"lvui_controlcenter.h"
#ifdef LVUI_CONTROLCENTER_H

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
    layout_info_t controlcenter;
    layout_info_t controller;
    layout_info_t controller_widget;
    layout_info_t controller_widget_button_left;
    layout_info_t controller_widget_button_right;
    layout_info_t controller_widget_container;
    layout_info_t controller_widget_slider_left;
    layout_info_t controller_widget_slider_right;
    layout_info_t controller_shortcut;
    layout_info_t notification;
    layout_info_t notification_lable;
}ui_layout_t;
static ui_layout_t ui_layout;

//ui widget
typedef struct ui_widget_t
{
    lv_obj_t*base;
    lv_obj_t*controlcenter;
    lv_obj_t*controller;
    lv_obj_t*controller_widget;
    lv_obj_t*controller_widget_button_left;
    lv_obj_t*controller_widget_button_right;
    lv_obj_t*controller_widget_container;
    lv_obj_t*controller_widget_slider_left;
    lv_obj_t*controller_widget_slider_right;
    lv_obj_t*controller_shortcut;
    lv_obj_t*notification;
    lv_obj_t*notification_lable;
}ui_widget_t;
static ui_widget_t ui_widget;

//ui animation
typedef struct ui_animation_t
{
    lv_anim_t controlcenter_in;
    lv_anim_t controlcenter_out;
    lv_anim_t controller_in;
    lv_anim_t controller_out;
    lv_anim_t notification_in;
    lv_anim_t notification_out;
}ui_animation_t;
static ui_animation_t ui_animation;

//gesture
typedef struct gesture_t
{
    uint16_t hor;
    uint16_t ver;
    uint8_t up_down_state;
    uint8_t disable_flag;
    uint8_t left_right_flag;
}gesture_t;
static gesture_t gesture;

//controller
typedef struct controller_t
{
    uint16_t hor;
    uint16_t ver;
    uint16_t hor_gap;
    uint16_t ver_gap;
    uint8_t button_left_state;
    uint8_t button_right_state;
    uint8_t slider_left_value;
    uint8_t slider_right_value;
}controller_t;
static controller_t controller;

//shortcutlist
typedef struct shortcutlist_t
{
    struct shortcutlist_t*last;
    lv_obj_t*button;
    lv_obj_t*lable;
    uint8_t state;
    void(*shortcut_cb)(uint8_t*);
    char*text;
    uint8_t number;
    struct shortcutlist_t*next;
}shortcutlist_t;
shortcutlist_t*shortcutlist;

//notification
typedef struct notification_t
{
    uint16_t hor;
    uint16_t ver;
    uint16_t hor_gap;
    uint16_t ver_gap;
}notification_t;
static notification_t notification;

//noticelist
typedef struct noticelist_t
{
    struct noticelist_t*last;
    lv_obj_t*button;
    lv_obj_t*lable;
    void(*notice_cb)();
    char*text;
    uint8_t number;
    int16_t pos_x;
    int16_t pos_y;
    struct noticelist_t*next;
}noticelist_t;
noticelist_t*noticelist;

//controlcenter layout
typedef struct controlcenter_layout_t
{
    uint16_t bt_rd;
    uint16_t bt_in;
}controlcenter_layout_t;
controlcenter_layout_t controlcenter_layout;

//build base
static void build_base(uint16_t hor,uint16_t ver);
//controlcenter layout init
static void controlcenter_layout_init();
//build controlcenter
static void build_controlcenter();
//controlcenter press
static void controlcenter_press(lv_event_t*e);
//controller trig
static void controller_trig(lv_event_t*e);
//build controller
static void build_controller();
//controller init
static void controller_init();
//shortcutlist init
static void shortcutlist_init();
//shortcutlist click
static void shortcutlist_click(lv_event_t*e);
//shortcutlist build
static void shortcutlist_build(shortcutlist_t*shortcutlist_new);
//shortcutlist add
void shortcutlist_add(char*text,void(*shortcut_cb)(uint8_t*));
//build notification
static void build_notification();
//noticelist init
static void noticelist_init();
//noticelist click
static void noticelist_click(lv_event_t*e);
//noticelist release
static void noticelist_release(lv_event_t*e);
//noticelist press
static void noticelist_press(lv_event_t*e);
//noticelist build
static void noticelist_build(noticelist_t*noticelist_new);
//noticelist add
void noticelist_add(char*text,void(*notice_cb)());
//noticelist remove
static void noticelist_remove(noticelist_t*noticelist_this);
//animation controlcenter
static void animation_controlcenter(void*obj,int32_t value);
//animation controller
static void animation_controller(void*obj,int32_t value);
//animation notification
static void animation_notification(void*obj,int32_t value);
//animation controlcenter end
static void animation_controlcenter_end();
//animation controlcenter end
static void animation_controller_end();
//animation notification end
static void animation_notification_end();
//animation controlcenter user
static void animation_controlcenter_user(lv_timer_t*t);
//animation init
static void animation_init();
//animation executor
static void animation_executor(uint8_t state);
//gesture detection
static void gesture_detection(lv_event_t*e);
//gesture init
static void gesture_init(uint16_t hor,uint16_t ver);
//lvui controlcenter init
void lvui_controlcenter_init();
//lvui controlcenter load
lv_obj_t*lvui_controlcenter_load();
//lvui controlcenter get
uint8_t lvui_controlcenter_get(uint8_t widget);

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

//controlcenter layout init
static void controlcenter_layout_init()
{
    if(lvui_display_level()==LVUI_DISPLAY_LEVEL_SMALL)
    {
        controlcenter_layout.bt_rd=lvui_display_change(12);
        controlcenter_layout.bt_in=lvui_display_change(1);
    }else
    if(lvui_display_level()==LVUI_DISPLAY_LEVEL_NOMALL)
    {
        controlcenter_layout.bt_rd=lvui_display_change(16);
        controlcenter_layout.bt_in=lvui_display_change(2);
    }else
    {
        controlcenter_layout.bt_rd=lvui_display_change(20);
        controlcenter_layout.bt_in=lvui_display_change(3);
    }
}

//build controlcenter
static void build_controlcenter()
{
    //creat
    //controlcenter
    ui_widget.controlcenter=lv_obj_create(ui_widget.base);
    //layout controlcenter
    //controlcenter
    ui_layout.controlcenter.left=0;
    ui_layout.controlcenter.top=0;
    ui_layout.controlcenter.hor=ui_layout.base.hor;
    ui_layout.controlcenter.ver=ui_layout.base.ver;
    //init
    //controlcenter
    lv_obj_set_pos(ui_widget.controlcenter,ui_layout.controlcenter.left,ui_layout.controlcenter.top);
    lv_obj_set_size(ui_widget.controlcenter,ui_layout.controlcenter.hor,ui_layout.controlcenter.ver);
    lv_obj_set_style_pad_left(ui_widget.controlcenter,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_widget.controlcenter,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_widget.controlcenter,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_widget.controlcenter,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_widget.controlcenter,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_widget.controlcenter,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_widget.controlcenter,lv_color_hex(0xff000000),LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_widget.controlcenter,200,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_flag(ui_widget.controlcenter,LV_OBJ_FLAG_HIDDEN);
}

//controlcenter press
static void controlcenter_press(lv_event_t*e)
{
    //to prevent accidental contact
    gesture.disable_flag=1;
}

//controller trig
static void controller_trig(lv_event_t*e)
{
    //change state
    lv_obj_t*obj=lv_event_get_target(e);
    if(obj==ui_widget.controller_widget_button_left)
    {
        controller.button_left_state=!controller.button_left_state;
        lvui_printf("controller.button_left_state=%d\n",controller.button_left_state);
    }else
    if(obj==ui_widget.controller_widget_button_right)
    {
        controller.button_right_state=!controller.button_right_state;
        lvui_printf("controller.button_right_state=%d\n",controller.button_right_state);
    }else
    if(obj==ui_widget.controller_widget_slider_left)
    {
        controller.slider_left_value=lv_slider_get_value(ui_widget.controller_widget_slider_left);
        lvui_printf("controller.slider_left_value=%d\n",controller.slider_left_value);
    }else
    if(obj==ui_widget.controller_widget_slider_right)
    {
        controller.slider_right_value=lv_slider_get_value(ui_widget.controller_widget_slider_right);
        lvui_printf("controller.slider_right_value=%d\n",controller.slider_right_value);
    }
}

//build controller
static void build_controller()
{
    //creat
    //controller
    ui_widget.controller=lv_obj_create(ui_widget.controlcenter);
        //controller widget
        ui_widget.controller_widget=lv_obj_create(ui_widget.controller);
            //left button
            ui_widget.controller_widget_button_left=lv_btn_create(ui_widget.controller_widget);
            //right button
            ui_widget.controller_widget_button_right=lv_btn_create(ui_widget.controller_widget);
            //container area
            ui_widget.controller_widget_container=lv_obj_create(ui_widget.controller_widget);
            //left slider
            ui_widget.controller_widget_slider_left=lv_slider_create(ui_widget.controller_widget);
            //right slider
            ui_widget.controller_widget_slider_right=lv_slider_create(ui_widget.controller_widget);
        //controller shortcut
        ui_widget.controller_shortcut=lv_obj_create(ui_widget.controller);
    //layout
    //controller
    ui_layout.controller.left=0;
    ui_layout.controller.top=lvui_statusbar_height();
    ui_layout.controller.hor=ui_layout.base.hor;
    //limit the size of the controller
    if(ui_layout.controller.hor>lvui_display_change(720))
    {
        ui_layout.controller.left=ui_layout.controller.hor/2;
        ui_layout.controller.hor=ui_layout.controller.hor/2;
    }
    if(ui_layout.controller.hor>lvui_display_change(480))
    {
        ui_layout.controller.left=ui_layout.controller.hor-lvui_display_change(480);
        ui_layout.controller.hor=lvui_display_change(480);
    }
    ui_layout.controller.ver=ui_layout.base.ver*3/4-lvui_statusbar_height();;
        //controller widget
        controller.hor_gap=(uint16_t)((float)ui_layout.controller.hor/31*1);//gap width
        controller.ver_gap=(uint16_t)((float)ui_layout.controller.hor/31*1);//gap heigth
        controller.hor=(uint16_t)((float)ui_layout.controller.hor/31*(31-3)/2);//button width
        controller.ver=(uint16_t)((float)(controller.hor-controller.hor_gap)/2);//button heigth
        ui_layout.controller_widget.left=0;
        ui_layout.controller_widget.top=0;
        ui_layout.controller_widget.hor=ui_layout.controller.hor;
        ui_layout.controller_widget.ver=controller.ver_gap*3+controller.ver+controller.hor;
            //left button
            ui_layout.controller_widget_button_left.left=controller.hor_gap;
            ui_layout.controller_widget_button_left.top=controller.ver_gap;
            ui_layout.controller_widget_button_left.hor=controller.hor;
            ui_layout.controller_widget_button_left.ver=controller.ver;
            //right button
            ui_layout.controller_widget_button_right.left=controller.hor_gap*2+controller.hor;
            ui_layout.controller_widget_button_right.top=controller.ver_gap;
            ui_layout.controller_widget_button_right.hor=controller.hor;
            ui_layout.controller_widget_button_right.ver=controller.ver;
            //container area
            ui_layout.controller_widget_container.left=controller.hor_gap;
            ui_layout.controller_widget_container.top=controller.ver_gap*2+controller.ver;
            ui_layout.controller_widget_container.hor=controller.hor;
            ui_layout.controller_widget_container.ver=controller.hor;
            //left slider
            ui_layout.controller_widget_slider_left.left=controller.hor_gap*2+controller.hor;
            ui_layout.controller_widget_slider_left.top=controller.ver_gap*2+controller.ver;
            ui_layout.controller_widget_slider_left.hor=controller.ver;
            ui_layout.controller_widget_slider_left.ver=controller.hor;
            //right slider
            ui_layout.controller_widget_slider_right.left=controller.hor_gap*3+controller.hor+controller.ver;
            ui_layout.controller_widget_slider_right.top=controller.ver_gap*2+controller.ver;
            ui_layout.controller_widget_slider_right.hor=controller.ver;
            ui_layout.controller_widget_slider_right.ver=controller.hor;
        //controller shortcut
        ui_layout.controller_shortcut.left=0;
        ui_layout.controller_shortcut.top=ui_layout.controller_widget.ver;
        ui_layout.controller_shortcut.hor=ui_layout.controller.hor;
        ui_layout.controller_shortcut.ver=0;
    //init
    //controller
    lv_obj_set_pos(ui_widget.controller,ui_layout.controller.left,ui_layout.controller.top);
    lv_obj_set_size(ui_widget.controller,ui_layout.controller.hor,ui_layout.controller.ver);
    lv_obj_set_style_pad_left(ui_widget.controller,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_widget.controller,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_widget.controller,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_widget.controller,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_widget.controller,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_widget.controller,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_widget.controller,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_widget.controller,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_flag(ui_widget.controller,LV_OBJ_FLAG_HIDDEN);
        //controller widget
        lv_obj_set_pos(ui_widget.controller_widget,ui_layout.controller_widget.left,ui_layout.controller_widget.top);
        lv_obj_set_size(ui_widget.controller_widget,ui_layout.controller_widget.hor,ui_layout.controller_widget.ver);
        lv_obj_set_style_pad_left(ui_widget.controller_widget,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(ui_widget.controller_widget,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(ui_widget.controller_widget,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(ui_widget.controller_widget,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(ui_widget.controller_widget,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_radius(ui_widget.controller_widget,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_widget.controller_widget,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_add_event_cb(ui_widget.controller_widget,(lv_event_cb_t)controlcenter_press,LV_EVENT_PRESSED,NULL);
            //left button
            lv_obj_set_pos(ui_widget.controller_widget_button_left,ui_layout.controller_widget_button_left.left,ui_layout.controller_widget_button_left.top);
            lv_obj_set_size(ui_widget.controller_widget_button_left,ui_layout.controller_widget_button_left.hor,ui_layout.controller_widget_button_left.ver);
            lv_obj_set_style_radius(ui_widget.controller_widget_button_left,controlcenter_layout.bt_rd,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_transform_width(ui_widget.controller_widget_button_left,-controlcenter_layout.bt_in,LV_PART_MAIN|LV_STATE_PRESSED);
            lv_obj_set_style_transform_height(ui_widget.controller_widget_button_left,-controlcenter_layout.bt_in,LV_PART_MAIN|LV_STATE_PRESSED);
            lv_obj_set_style_bg_color(ui_widget.controller_widget_button_left,lv_color_hex(0xff005797),LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(ui_widget.controller_widget_button_left,250,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_add_event_cb(ui_widget.controller_widget_button_left,(lv_event_cb_t)controlcenter_press,LV_EVENT_PRESSED,NULL);
            lv_obj_add_event_cb(ui_widget.controller_widget_button_left,(lv_event_cb_t)controller_trig,LV_EVENT_CLICKED,NULL);
            lv_obj_t*cta_b1_t=lv_label_create(ui_widget.controller_widget_button_left);
            lv_obj_set_pos(cta_b1_t,0,0);
            lv_obj_set_size(cta_b1_t,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
            lv_label_set_text(cta_b1_t,"Wifi");
            lv_obj_set_style_align(cta_b1_t,LV_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_radius(cta_b1_t,0,LV_PART_MAIN|LV_STATE_DEFAULT);
            //right button
            lv_obj_set_pos(ui_widget.controller_widget_button_right,ui_layout.controller_widget_button_right.left,ui_layout.controller_widget_button_right.top);
            lv_obj_set_size(ui_widget.controller_widget_button_right,ui_layout.controller_widget_button_right.hor,ui_layout.controller_widget_button_right.ver);
            lv_obj_set_style_radius(ui_widget.controller_widget_button_right,controlcenter_layout.bt_rd,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_transform_width(ui_widget.controller_widget_button_right,-controlcenter_layout.bt_in,LV_PART_MAIN|LV_STATE_PRESSED);
            lv_obj_set_style_transform_height(ui_widget.controller_widget_button_right,-controlcenter_layout.bt_in,LV_PART_MAIN|LV_STATE_PRESSED);
            lv_obj_set_style_bg_color(ui_widget.controller_widget_button_right,lv_color_hex(0xff005797),LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(ui_widget.controller_widget_button_right,250,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_add_event_cb(ui_widget.controller_widget_button_right,(lv_event_cb_t)controlcenter_press,LV_EVENT_PRESSED,NULL);
            lv_obj_add_event_cb(ui_widget.controller_widget_button_right,(lv_event_cb_t)controller_trig,LV_EVENT_CLICKED,NULL);
            lv_obj_t*cta_b2_t=lv_label_create(ui_widget.controller_widget_button_right);
            lv_obj_set_pos(cta_b2_t,0,0);
            lv_obj_set_size(cta_b2_t,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
            lv_label_set_text(cta_b2_t,"Bluetooth");
            lv_obj_set_style_align(cta_b2_t,LV_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_radius(cta_b2_t,0,LV_PART_MAIN|LV_STATE_DEFAULT);
            //container area
            lv_obj_set_pos(ui_widget.controller_widget_container,ui_layout.controller_widget_container.left,ui_layout.controller_widget_container.top);
            lv_obj_set_size(ui_widget.controller_widget_container,ui_layout.controller_widget_container.hor,ui_layout.controller_widget_container.ver);
            lv_obj_set_style_pad_left(ui_widget.controller_widget_container,0,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(ui_widget.controller_widget_container,0,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(ui_widget.controller_widget_container,0,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(ui_widget.controller_widget_container,0,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(ui_widget.controller_widget_container,0,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_radius(ui_widget.controller_widget_container,controlcenter_layout.bt_rd,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_widget.controller_widget_container,lv_color_hex(0xff2196f3),LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(ui_widget.controller_widget_container,250,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_add_event_cb(ui_widget.controller_widget_container,(lv_event_cb_t)controlcenter_press,LV_EVENT_PRESSED,NULL);
            lv_obj_t*cta_c_t=lv_label_create(ui_widget.controller_widget_container);
            lv_obj_set_pos(cta_c_t,0,0);
            lv_obj_set_size(cta_c_t,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
            lv_label_set_text(cta_c_t,"Content\n Center");
            lv_obj_set_style_text_color(cta_c_t,lv_color_hex(0xffffffff),LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_align(cta_c_t,LV_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_radius(cta_c_t,0,LV_PART_MAIN|LV_STATE_DEFAULT);
            //left slider
            lv_obj_set_pos(ui_widget.controller_widget_slider_left,ui_layout.controller_widget_slider_left.left,ui_layout.controller_widget_slider_left.top);
            lv_obj_set_size(ui_widget.controller_widget_slider_left,ui_layout.controller_widget_slider_left.hor,ui_layout.controller_widget_slider_left.ver);
            lv_slider_set_value(ui_widget.controller_widget_slider_left,25,LV_ANIM_OFF);
            lv_obj_set_style_opa(ui_widget.controller_widget_slider_left,0,LV_PART_KNOB|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_widget.controller_widget_slider_left,lv_color_hex(0xff005797),LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(ui_widget.controller_widget_slider_left,250,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_radius(ui_widget.controller_widget_slider_left,controlcenter_layout.bt_rd,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_radius(ui_widget.controller_widget_slider_left,controlcenter_layout.bt_rd,LV_PART_INDICATOR|LV_STATE_DEFAULT);
            lv_obj_add_event_cb(ui_widget.controller_widget_slider_left,(lv_event_cb_t)controlcenter_press,LV_EVENT_PRESSED,NULL);
            lv_obj_add_event_cb(ui_widget.controller_widget_slider_left,(lv_event_cb_t)controller_trig,LV_EVENT_VALUE_CHANGED,NULL);
            lv_obj_t*cta_s1_t=lv_label_create(ui_widget.controller_widget_slider_left);
            lv_obj_set_pos(cta_s1_t,0,0);
            lv_obj_set_size(cta_s1_t,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
            lv_label_set_text(cta_s1_t,"Brigh\ntness");
            lv_obj_set_style_text_color(cta_s1_t,lv_color_hex(0xffffffff),LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_align(cta_s1_t,LV_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_radius(cta_s1_t,0,LV_PART_MAIN|LV_STATE_DEFAULT);
            //right slider
            lv_obj_set_pos(ui_widget.controller_widget_slider_right,ui_layout.controller_widget_slider_right.left,ui_layout.controller_widget_slider_right.top);
            lv_obj_set_size(ui_widget.controller_widget_slider_right,ui_layout.controller_widget_slider_right.hor,ui_layout.controller_widget_slider_right.ver);
            lv_slider_set_value(ui_widget.controller_widget_slider_right,25,LV_ANIM_OFF);
            lv_obj_set_style_opa(ui_widget.controller_widget_slider_right,0,LV_PART_KNOB|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_widget.controller_widget_slider_right,lv_color_hex(0xff005797),LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(ui_widget.controller_widget_slider_right,250,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_radius(ui_widget.controller_widget_slider_right,controlcenter_layout.bt_rd,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_radius(ui_widget.controller_widget_slider_right,controlcenter_layout.bt_rd,LV_PART_INDICATOR|LV_STATE_DEFAULT);
            lv_obj_add_event_cb(ui_widget.controller_widget_slider_right,(lv_event_cb_t)controlcenter_press,LV_EVENT_PRESSED,NULL);
            lv_obj_add_event_cb(ui_widget.controller_widget_slider_right,(lv_event_cb_t)controller_trig,LV_EVENT_VALUE_CHANGED,NULL);
            lv_obj_t*cta_s2_t=lv_label_create(ui_widget.controller_widget_slider_right);
            lv_obj_set_pos(cta_s2_t,0,0);
            lv_obj_set_size(cta_s2_t,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
            lv_label_set_text(cta_s2_t,"Volume");
            lv_obj_set_style_text_color(cta_s2_t,lv_color_hex(0xffffffff),LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_align(cta_s2_t,LV_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_radius(cta_s2_t,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        //controller shortcut
        lv_obj_set_pos(ui_widget.controller_shortcut,ui_layout.controller_shortcut.left,ui_layout.controller_shortcut.top);
        lv_obj_set_size(ui_widget.controller_shortcut,ui_layout.controller_shortcut.hor,ui_layout.controller_shortcut.ver);
        lv_obj_set_style_pad_left(ui_widget.controller_shortcut,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(ui_widget.controller_shortcut,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(ui_widget.controller_shortcut,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(ui_widget.controller_shortcut,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(ui_widget.controller_shortcut,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_radius(ui_widget.controller_shortcut,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_widget.controller_shortcut,0,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_add_event_cb(ui_widget.controller_shortcut,(lv_event_cb_t)controlcenter_press,LV_EVENT_PRESSED,NULL);
}

//controller init
static void controller_init()
{
    controller.button_left_state=0;
    controller.button_right_state=0;
    controller.slider_left_value=0;
    controller.slider_right_value=0;
}

//shortcutlist init
static void shortcutlist_init()
{
    shortcutlist=malloc(sizeof(shortcutlist_t));
    shortcutlist->last=NULL;
    shortcutlist->button=NULL;
    shortcutlist->lable=NULL;
    shortcutlist->state=0;
    shortcutlist->shortcut_cb=NULL;
    shortcutlist->text=NULL;
    shortcutlist->number=0;
    shortcutlist->next=NULL;
}

//shortcutlist click
static void shortcutlist_click(lv_event_t*e)
{
    shortcutlist_t*shortcutlist=lv_event_get_user_data(e);
    //change state
    shortcutlist->state=!shortcutlist->state;
    //load shortcut callback
    if(shortcutlist->shortcut_cb)
    {
        shortcutlist->shortcut_cb(&shortcutlist->state);
    }
    if(shortcutlist->state)
    {
        lv_obj_set_style_bg_color(shortcutlist->button,lv_color_hex(0xff2196f3),LV_PART_MAIN|LV_STATE_DEFAULT);
    }else
    {
        lv_obj_set_style_bg_color(shortcutlist->button,lv_color_hex(0xff005797),LV_PART_MAIN|LV_STATE_DEFAULT);
    }
}

//shortcutlist build
static void shortcutlist_build(shortcutlist_t*shortcutlist_new)
{
    uint8_t number=shortcutlist_new->number;
    //layout
    uint16_t left=0;
    uint16_t top=0;
    if(number%4==0)
    {
        left=controller.hor_gap+(4-1)*(controller.hor_gap+controller.ver);
        top=controller.ver_gap+(number/4-1)*(controller.ver+controller.hor_gap);
        //controller shortcut height
        ui_layout.controller_shortcut.ver=controller.ver_gap+(number/4)*(controller.ver+controller.hor_gap);
    }else
    {
        left=controller.hor_gap+((number%4)-1)*(controller.hor_gap+controller.ver);
        top=controller.ver_gap+(number/4)*(controller.ver+controller.hor_gap);
        //controller shortcut height
        ui_layout.controller_shortcut.ver=controller.ver_gap+(number/4+1)*(controller.ver+controller.hor_gap);
    }
    lv_obj_set_size(ui_widget.controller_shortcut,ui_layout.controller_shortcut.hor,ui_layout.controller_shortcut.ver);
    lv_obj_t*obj=NULL;
    //button
    obj=lv_btn_create(ui_widget.controller_shortcut);
    shortcutlist_new->button=obj;
    lv_obj_set_pos(obj,left,top);
    lv_obj_set_size(obj,controller.ver,controller.ver);
    lv_obj_set_style_radius(obj,controlcenter_layout.bt_rd,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_transform_width(obj,-controlcenter_layout.bt_in,LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_transform_height(obj,-controlcenter_layout.bt_in,LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(obj,lv_color_hex(0xff005797),LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj,250,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_event_cb(obj,(lv_event_cb_t)controlcenter_press,LV_EVENT_PRESSED,NULL);
    lv_obj_add_event_cb(obj,(lv_event_cb_t)shortcutlist_click,LV_EVENT_CLICKED,shortcutlist_new);
    //lable
    obj=lv_label_create(shortcutlist_new->button);
    shortcutlist_new->lable=obj;
    lv_obj_set_pos(obj,0,0);
    lv_obj_set_size(obj,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
    lv_label_set_text(obj,shortcutlist_new->text);
    lv_obj_set_style_align(obj,LV_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
}

//shortcutlist add
void shortcutlist_add(char*text,void(*shortcut_cb)(uint8_t*))
{
    shortcutlist_t*shortcutlist_old=shortcutlist;
    while(shortcutlist_old->next!=NULL)
    {
        shortcutlist_old=shortcutlist_old->next;
    }
    uint8_t number=shortcutlist_old->number;
    shortcutlist_t*shortcutlist_new=malloc(sizeof(shortcutlist_t));
    shortcutlist_old->next=shortcutlist_new;
    shortcutlist_new->last=shortcutlist_old;
    shortcutlist_new->state=0;
    shortcutlist_new->button=NULL;
    shortcutlist_new->shortcut_cb=shortcut_cb;
    shortcutlist_new->text=text;
    shortcutlist_new->number=number+1;
    shortcutlist_new->next=NULL;
    shortcutlist_build(shortcutlist_new);
}

//build notification
static void build_notification()
{
    //creat
    //notification
    ui_widget.notification=lv_obj_create(ui_widget.controlcenter);
        //lable
        ui_widget.notification_lable=lv_label_create(ui_widget.notification);
    //layout
    //notification
    ui_layout.notification.left=0;
    ui_layout.notification.top=lvui_statusbar_height();
    ui_layout.notification.hor=ui_layout.base.hor;
    //limit the size of the notification
    if(ui_layout.notification.hor>lvui_display_change(720))
    {
        ui_layout.notification.left=ui_layout.notification.hor/2;
        ui_layout.notification.hor=ui_layout.notification.hor/2;
    }
    if(ui_layout.notification.hor>lvui_display_change(480))
    {
        ui_layout.notification.left=ui_layout.notification.hor-lvui_display_change(480);
        ui_layout.notification.hor=lvui_display_change(480);
    }
    ui_layout.notification.ver=(uint16_t)((float)ui_layout.base.ver*3/4)-lvui_statusbar_height();
        //lable
        ui_layout.notification_lable.left=0;
        ui_layout.notification_lable.top=0;
        ui_layout.notification_lable.hor=0;
        ui_layout.notification_lable.ver=0;
    //init
    //notification
    lv_obj_set_pos(ui_widget.notification,ui_layout.notification.left,ui_layout.notification.top);
    lv_obj_set_size(ui_widget.notification,ui_layout.notification.hor,ui_layout.notification.ver);
    lv_obj_set_style_pad_left(ui_widget.notification,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_widget.notification,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_widget.notification,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_widget.notification,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_widget.notification,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_widget.notification,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_widget.notification,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_widget.notification,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_widget.notification,(lv_event_cb_t)controlcenter_press,LV_EVENT_PRESSED,NULL);
    lv_obj_add_flag(ui_widget.notification,LV_OBJ_FLAG_HIDDEN);
    // lv_obj_remove_flag(ui_widget.notification,LV_OBJ_FLAG_SCROLLABLE);
        //lable
        lv_obj_set_pos(ui_widget.notification_lable,0,0);
        lv_obj_set_size(ui_widget.notification_lable,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
        lv_label_set_text(ui_widget.notification_lable,"No Notification");
        lv_obj_set_style_text_color(ui_widget.notification_lable,lv_color_hex(0xffffffff),LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_align(ui_widget.notification_lable,LV_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_radius(ui_widget.notification_lable,0,LV_PART_MAIN|LV_STATE_DEFAULT);
}

//noticelist init
static void noticelist_init()
{
    noticelist=malloc(sizeof(noticelist_t));
    noticelist->last=NULL;
    noticelist->button=NULL;
    noticelist->lable=NULL;
    noticelist->notice_cb=NULL;
    noticelist->text=NULL;
    noticelist->number=0;
    noticelist->pos_x=0;
    noticelist->pos_y=0;
    noticelist->next=NULL;
}

//noticelist click
static void noticelist_click(lv_event_t*e)
{
    noticelist_t*noticelist=lv_event_get_user_data(e);
    //threshold
    if(abs(lvui_gesture_move_x())<lvui_display_change(8))
    {
        //load shortcut callback
        if(noticelist->notice_cb)
        {
            noticelist->notice_cb();
        }
        //noticelist remove
        noticelist_remove(noticelist);
    }
}

//noticelist release
static void noticelist_release(lv_event_t*e)
{
    noticelist_t*noticelist=lv_event_get_user_data(e);
    noticelist->pos_x=notification.hor_gap;
}

//noticelist press
static void noticelist_press(lv_event_t*e)
{
    noticelist_t*noticelist=lv_event_get_user_data(e);
    uint16_t x=lvui_gesture_touch_x();
    if(x)
    {
        int16_t x_m=lvui_gesture_move_x();
        noticelist->pos_x=notification.hor_gap+x_m;
        //threshold
        if(x_m>notification.hor/2)
        {
            noticelist_remove(noticelist);
        }
    }
}

//noticelist build
static void noticelist_build(noticelist_t*noticelist_new)
{
    notification.ver_gap=ui_layout.notification.hor/31*1;
    notification.hor_gap=ui_layout.notification.hor/31*1;
    notification.hor=ui_layout.notification.hor-notification.hor_gap*2;
    notification.ver=notification.hor/4;
    //layout
    uint16_t left=notification.hor_gap;
    uint16_t top=notification.ver_gap+(noticelist_new->number-1)*(notification.ver_gap+notification.ver);
    lv_obj_t*obj=NULL;
    //button
    obj=lv_btn_create(ui_widget.notification);
    noticelist_new->button=obj;
    noticelist_new->pos_x=left;
    noticelist_new->pos_y=top;
    lv_obj_set_pos(obj,left,top);
    lv_obj_set_size(obj,notification.hor,notification.ver);
    lv_obj_set_style_radius(obj,controlcenter_layout.bt_rd,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_transform_width(obj,-controlcenter_layout.bt_in,LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_transform_height(obj,-controlcenter_layout.bt_in,LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(obj,lv_color_hex(0xff2196f3),LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj,250,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_event_cb(obj,(lv_event_cb_t)controlcenter_press,LV_EVENT_PRESSED,NULL);
    lv_obj_add_event_cb(obj,(lv_event_cb_t)noticelist_click,LV_EVENT_CLICKED,noticelist_new);
    lv_obj_add_event_cb(obj,(lv_event_cb_t)noticelist_press,LV_EVENT_PRESSING,noticelist_new);
    lv_obj_add_event_cb(obj,(lv_event_cb_t)noticelist_release,LV_EVENT_RELEASED,noticelist_new);
    //lable
    obj=lv_label_create(noticelist_new->button);
    noticelist_new->lable=obj;
    lv_obj_set_pos(obj,0,0);
    lv_obj_set_size(obj,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
    lv_label_set_text(obj,noticelist_new->text);
    lv_obj_set_style_align(obj,LV_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
    //set noticelist height
    uint16_t height=(noticelist->number)*(notification.ver+notification.ver_gap);
    if(height>ui_layout.notification.ver)
    {
        lv_obj_set_height(ui_widget.notification,ui_layout.notification.ver);
    }else
    {
        lv_obj_set_height(ui_widget.notification,height+notification.ver);
    }
}

//noticelist add
void noticelist_add(char*text,void(*notice_cb)())
{
    noticelist_t*noticelist_old=noticelist;
    ++noticelist->number;
    while(noticelist_old->next!=NULL)
    {
        noticelist_old=noticelist_old->next;
    }
    noticelist_t*noticelist_new=malloc(sizeof(noticelist_t));
    noticelist_old->next=noticelist_new;
    noticelist_new->last=noticelist_old;
    noticelist_new->button=NULL;
    noticelist_new->notice_cb=notice_cb;
    noticelist_new->text=text;
    noticelist_new->number=noticelist->number;
    noticelist_new->pos_x=0;
    noticelist_new->pos_y=0;
    noticelist_new->next=NULL;
    noticelist_build(noticelist_new);
}

//noticelist remove
static void noticelist_remove(noticelist_t*noticelist_this)
{
    //remove
    --noticelist->number;
    lv_obj_del(noticelist_this->button);
    if(noticelist_this->next!=NULL)
    {
        noticelist_this->last->next=noticelist_this->next;
        noticelist_this->next->last=noticelist_this->last;
    }else
    {
        noticelist_this->last->next=NULL;
    }
    //update
    noticelist_t*noticelist_old=noticelist_this;
    --noticelist_old->number;
    while(noticelist_old->next!=NULL)
    {
        //update number
        noticelist_old->next->number=noticelist_old->number+1;
        //update postation
        noticelist_old->next->pos_y=notification.ver_gap+(noticelist_old->next->number-1)*(notification.ver_gap+notification.ver);
        noticelist_old=noticelist_old->next;
    }
    //set noticelist height
    uint16_t top=(noticelist->number)*(notification.ver+notification.ver_gap);
    if(top>ui_layout.notification.ver)
    {
        lv_obj_set_height(ui_widget.notification,ui_layout.notification.ver);
    }else
    {
        lv_obj_set_height(ui_widget.notification,top+notification.ver);
    }
    free(noticelist_this);
}

//animation controlcenter
static void animation_controlcenter(void*obj,int32_t value)
{
    //realize animation effects
    lv_obj_set_style_bg_opa((lv_obj_t*)obj,value,LV_PART_MAIN|LV_STATE_DEFAULT);
}

//animation controller
static void animation_controller(void*obj,int32_t value)
{
    //realize animation effects
    lv_obj_set_style_opa((lv_obj_t*)obj,value,LV_PART_MAIN|LV_STATE_DEFAULT);
}

//animation notification
static void animation_notification(void*obj,int32_t value)
{
    //realize animation effects
    lv_obj_set_style_opa((lv_obj_t*)obj,value,LV_PART_MAIN|LV_STATE_DEFAULT);
}

//animation controlcenter end
static void animation_controlcenter_end()
{
    //hide all
    lv_obj_add_flag(ui_widget.controlcenter,LV_OBJ_FLAG_HIDDEN);
    //reset left right flag
    gesture.left_right_flag=0;
}

//animation controlcenter end
static void animation_controller_end()
{
    //hide self
    lv_obj_add_flag(ui_widget.controller,LV_OBJ_FLAG_HIDDEN);
    //reset position
    lv_obj_scroll_to(ui_widget.controller,0,0,LV_ANIM_ON);
}

//animation notification end
static void animation_notification_end()
{
    //hide self
    lv_obj_add_flag(ui_widget.notification,LV_OBJ_FLAG_HIDDEN);
    //reset position
    lv_obj_scroll_to(ui_widget.notification,0,0,LV_ANIM_ON);
}

//animation controlcenter user
static void animation_controlcenter_user(lv_timer_t*t)
{
    if(gesture.up_down_state)
    {
        if(gesture.left_right_flag==1)
        {
            //update state
            if(controller.button_left_state)
            {
                lv_obj_set_style_bg_color(ui_widget.controller_widget_button_left,lv_color_hex(0xff2196f3),LV_PART_MAIN|LV_STATE_DEFAULT);
            }else
            {
                lv_obj_set_style_bg_color(ui_widget.controller_widget_button_left,lv_color_hex(0xff005797),LV_PART_MAIN|LV_STATE_DEFAULT);
            }
            if(controller.button_right_state)
            {
                lv_obj_set_style_bg_color(ui_widget.controller_widget_button_right,lv_color_hex(0xff2196f3),LV_PART_MAIN|LV_STATE_DEFAULT);
            }else
            {
                lv_obj_set_style_bg_color(ui_widget.controller_widget_button_right,lv_color_hex(0xff005797),LV_PART_MAIN|LV_STATE_DEFAULT);
            }
            lv_slider_set_value(ui_widget.controller_widget_slider_left,controller.slider_left_value,LV_ANIM_ON);
            lv_slider_set_value(ui_widget.controller_widget_slider_right,controller.slider_right_value,LV_ANIM_ON);
        }else
        if(gesture.left_right_flag==2)
        {
            if(noticelist->next==NULL)
            {
                lv_obj_remove_flag(ui_widget.notification_lable,LV_OBJ_FLAG_HIDDEN);
            }else
            {
                lv_obj_add_flag(ui_widget.notification_lable,LV_OBJ_FLAG_HIDDEN);
            }
            noticelist_t*noticelist_old=noticelist;
            while(noticelist_old->next!=NULL)
            {
                noticelist_old=noticelist_old->next;
                int16_t x=lv_obj_get_x(noticelist_old->button);
                if(x!=noticelist_old->pos_x)
                {
                    x+=(noticelist_old->pos_x-x)/2;
                    if(x>notification.hor_gap)
                    {
                        lv_obj_set_x(noticelist_old->button,x);
                        lv_obj_set_width(noticelist_old->button,notification.hor-(x-notification.hor_gap));
                    }
                }
                int16_t y=lv_obj_get_y(noticelist_old->button);
                if(y!=noticelist_old->pos_y)
                {
                    y+=(noticelist_old->pos_y-y)/2;
                    lv_obj_set_y(noticelist_old->button,y);
                }
            }
        }
    }
}

//animation init
static void animation_init()
{
    //controlcenter in
    lv_anim_init(&ui_animation.controlcenter_in);
    lv_anim_set_var(&ui_animation.controlcenter_in,ui_widget.controlcenter);
    lv_anim_set_values(&ui_animation.controlcenter_in,0,150);
    lv_anim_set_time(&ui_animation.controlcenter_in,200);
    lv_anim_set_exec_cb(&ui_animation.controlcenter_in,(lv_anim_exec_xcb_t)animation_controlcenter);
    lv_anim_set_path_cb(&ui_animation.controlcenter_in,lv_anim_path_linear);
    //controlcenter out
    lv_anim_init(&ui_animation.controlcenter_out);
    lv_anim_set_var(&ui_animation.controlcenter_out,ui_widget.controlcenter);
    lv_anim_set_values(&ui_animation.controlcenter_out,150,0);
    lv_anim_set_time(&ui_animation.controlcenter_out,200);
    lv_anim_set_exec_cb(&ui_animation.controlcenter_out,(lv_anim_exec_xcb_t)animation_controlcenter);
    lv_anim_set_path_cb(&ui_animation.controlcenter_out,lv_anim_path_linear);
    lv_anim_set_ready_cb(&ui_animation.controlcenter_out,(lv_anim_ready_cb_t)animation_controlcenter_end);
    //controller in
    lv_anim_init(&ui_animation.controller_in);
    lv_anim_set_var(&ui_animation.controller_in,ui_widget.controller);
    lv_anim_set_values(&ui_animation.controller_in,0,250);
    lv_anim_set_time(&ui_animation.controller_in,200);
    lv_anim_set_exec_cb(&ui_animation.controller_in,(lv_anim_exec_xcb_t)animation_controller);
    lv_anim_set_path_cb(&ui_animation.controller_in,lv_anim_path_linear);
    //controller out
    lv_anim_init(&ui_animation.controller_out);
    lv_anim_set_var(&ui_animation.controller_out,ui_widget.controller);
    lv_anim_set_values(&ui_animation.controller_out,250,0);
    lv_anim_set_time(&ui_animation.controller_out,200);
    lv_anim_set_exec_cb(&ui_animation.controller_out,(lv_anim_exec_xcb_t)animation_controller);
    lv_anim_set_path_cb(&ui_animation.controller_out,lv_anim_path_linear);
    lv_anim_set_ready_cb(&ui_animation.controller_out,(lv_anim_ready_cb_t)animation_controller_end);
    //notification in
    lv_anim_init(&ui_animation.notification_in);
    lv_anim_set_var(&ui_animation.notification_in,ui_widget.notification);
    lv_anim_set_values(&ui_animation.notification_in,0,250);
    lv_anim_set_time(&ui_animation.notification_in,200);
    lv_anim_set_exec_cb(&ui_animation.notification_in,(lv_anim_exec_xcb_t)animation_notification);
    lv_anim_set_path_cb(&ui_animation.notification_in,lv_anim_path_linear);
    //notification out
    lv_anim_init(&ui_animation.notification_out);
    lv_anim_set_var(&ui_animation.notification_out,ui_widget.notification);
    lv_anim_set_values(&ui_animation.notification_out,250,0);
    lv_anim_set_time(&ui_animation.notification_out,200);
    lv_anim_set_exec_cb(&ui_animation.notification_out,(lv_anim_exec_xcb_t)animation_notification);
    lv_anim_set_path_cb(&ui_animation.notification_out,lv_anim_path_linear);
    lv_anim_set_ready_cb(&ui_animation.notification_out,(lv_anim_ready_cb_t)animation_notification_end);
    //notification user
    lv_timer_create(animation_controlcenter_user,10,NULL);
}

//animation executor
static void animation_executor(uint8_t state)
{
    //slide down
    if(state==2)
    {
        lvui_printf("controlcenter open\n");
        lv_anim_start(&ui_animation.controlcenter_in);
        //load animation in
        lv_obj_clear_flag(ui_widget.controlcenter,LV_OBJ_FLAG_HIDDEN);
        if(gesture.left_right_flag==1)
        {
            //controller
            lv_obj_clear_flag(ui_widget.controller,LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_widget.notification,LV_OBJ_FLAG_HIDDEN);
            lv_anim_start(&ui_animation.controller_in);
            lvui_printf("controller open\n");
        }else
        if(gesture.left_right_flag==2)
        {
            //notification
            lv_obj_clear_flag(ui_widget.notification,LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_widget.controller,LV_OBJ_FLAG_HIDDEN);
            lv_anim_start(&ui_animation.notification_in);
            lvui_printf("notification open\n");
        }
    }else
    //slide up
    if(state==1)
    {
        lvui_printf("controlcenter close\n");
        //load animation out
        if(gesture.left_right_flag==1)
        {
            //controller
            lv_anim_start(&ui_animation.controller_out);
        }else
        if(gesture.left_right_flag==2)
        {
            //notification
            lv_anim_start(&ui_animation.notification_out);
        }
        lv_anim_start(&ui_animation.controlcenter_out);
    }else
    //slide right
    if(state==4)
    {
        //to notification
        gesture.left_right_flag=2;
        lv_anim_start(&ui_animation.controller_out);
        lv_obj_clear_flag(ui_widget.notification,LV_OBJ_FLAG_HIDDEN);
        lv_anim_start(&ui_animation.notification_in);
    }
    //slide left
    if(state==3)
    {
        //to controller
        gesture.left_right_flag=1;
        lv_anim_start(&ui_animation.notification_out);
        lv_obj_clear_flag(ui_widget.controller,LV_OBJ_FLAG_HIDDEN);
        lv_anim_start(&ui_animation.controller_in);
    }
}

//gesture detection
static void gesture_detection(lv_event_t*e)
{
    uint16_t x=0;
    uint16_t y=0;
    lvui_gesture_first_touch(&x,&y);

    //slide down
    if(gesture.up_down_state==0)
    {
        if(y<lvui_statusbar_height())
        {
            if(lvui_gesture_action()==2)
            {
                if(x>gesture.hor/2)
                {
                    //controller
                    gesture.left_right_flag=1;
                }else
                {
                    //notification
                    gesture.left_right_flag=2;
                }
                //load animation
                animation_executor(2);
                //update controlcenter state
                gesture.up_down_state=1;
            }
        }
    }else
    if(gesture.up_down_state==1)
    {
        if(gesture.disable_flag==0)
        {
            //slide up
            if(lvui_gesture_action()==1)
            {
                //load animation
                animation_executor(1);
                //update controlcenter state
                gesture.up_down_state=0;
            }
            //slide right
            if(lvui_gesture_action()==4&&gesture.left_right_flag==1)
            {
                //load animation
                animation_executor(4);
                //update controlcenter state
                gesture.up_down_state=1;
            }
            //slide left
            if(lvui_gesture_action()==3&&gesture.left_right_flag==2)
            {
                //load animation
                animation_executor(3);
                //update controlcenter state
                gesture.up_down_state=1;
            }
        }else
        {
            gesture.disable_flag=0;
        }
    }
}

//gesture init
static void gesture_init(uint16_t hor,uint16_t ver)
{
    gesture.hor=hor;
    gesture.ver=ver;
    gesture.up_down_state=0;
    gesture.disable_flag=0;
    gesture.left_right_flag=0;
    lv_indev_add_event_cb(lv_indev_get_next(NULL),gesture_detection,LV_EVENT_RELEASED,NULL);
}

//lvui controlcenter init
void lvui_controlcenter_init()
{
    uint16_t hor=0;
    uint16_t ver=0;
    lvui_manager_screen(&hor,&ver);
    //build base
    build_base(hor,ver);
    //controlcenter layout init
    controlcenter_layout_init();
    //build controlcenter
    build_controlcenter();
    //build controller
    build_controller();
    //controller init
    controller_init();
    //shortcutlist init
    shortcutlist_init();
    //build notification
    build_notification();
    //noticelist init
    noticelist_init();
    //gesture init
    gesture_init(hor,ver);
    //animation init
    animation_init();
}

//lvui controlcenter load
lv_obj_t*lvui_controlcenter_load()
{
    return ui_widget.controlcenter;
}

//lvui controlcenter get
uint8_t lvui_controlcenter_get(uint8_t widget)
{
    if(widget==LVUI_CONTROLCENTER_BUTTON_LEFT)
    {
        return controller.button_left_state;
    }else
    if(widget==LVUI_CONTROLCENTER_BUTTON_RIGHT)
    {
        return controller.button_right_state;
    }else
    if(widget==LVUI_CONTROLCENTER_SLIDER_LEFT)
    {
        return controller.slider_left_value;
    }else
    {
        return controller.slider_right_value;
    }
}

#endif//#ifndef LVUI_CONTROLCENTER_H
