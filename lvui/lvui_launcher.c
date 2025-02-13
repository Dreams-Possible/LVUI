#include"lvui_launcher.h"
#ifdef LVUI_LAUNCHER_H

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
    layout_info_t launcher;
}ui_layout_t;
static ui_layout_t ui_layout;

//ui widget
typedef struct ui_widget_t
{
    lv_obj_t*base;
    lv_obj_t*launcher;
}ui_widget_t;
static ui_widget_t ui_widget;

//applist
typedef struct applist_t
{
    struct applist_t*last;
    lv_obj_t*button;
    lv_obj_t*lable;
    lv_obj_t*app_screen;
    lv_anim_t app_in;
    lv_anim_t app_out;
    char*text;
    uint8_t number;
    struct applist_t*next;
}applist_t;
applist_t*applist;
applist_t*applist_current;

//launcher layout
typedef struct launcher_layout_t
{
    //app icon
    uint16_t hor;
    uint16_t ver;
    uint16_t bt_rd;
    uint16_t bt_in;
    uint16_t hor_gap;
    uint16_t ver_gap;
    uint16_t hor_num;
}launcher_layout_t;
launcher_layout_t launcher_layout;

//build base
static void build_base(uint16_t hor,uint16_t ver);
//launcher layout init
static void launcher_layout_init();
//build launcher
static void build_launcher();
//lvui launcher init
void lvui_launcher_init();
//lvui launcher load
lv_obj_t*lvui_launcher_load();
//applist init
static void applist_init();
//applist open
static void applist_open(lv_event_t*e);
//animation app
static void animation_app(void*obj,int32_t value);
//animation app end
static void animation_app_end();
//applist build
static void applist_build(applist_t*applist_new);
//applist add
void applist_add(char*text,lv_obj_t*app_screen);
//lvui launcher back
void lvui_launcher_back();

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

//launcher layout init
static void launcher_layout_init()
{
    if(lvui_display_level()==LVUI_DISPLAY_LEVEL_SMALL)
    {
        launcher_layout.hor=lvui_display_change(62);
        launcher_layout.ver=lvui_display_change(62);
        launcher_layout.hor_gap=lvui_display_change(8);
        launcher_layout.ver_gap=lvui_display_change(8);
        launcher_layout.bt_rd=lvui_display_change(12);
        launcher_layout.bt_in=lvui_display_change(1);
    }else
    if(lvui_display_level()==LVUI_DISPLAY_LEVEL_NOMALL)
    {
        launcher_layout.hor=lvui_display_change(70);
        launcher_layout.ver=lvui_display_change(70);
        launcher_layout.hor_gap=lvui_display_change(12);
        launcher_layout.ver_gap=lvui_display_change(12);
        launcher_layout.bt_rd=lvui_display_change(16);
        launcher_layout.bt_in=lvui_display_change(2);
    }else
    {
        launcher_layout.hor=lvui_display_change(78);
        launcher_layout.ver=lvui_display_change(78);
        launcher_layout.hor_gap=lvui_display_change(16);
        launcher_layout.ver_gap=lvui_display_change(16);
        launcher_layout.bt_rd=lvui_display_change(20);
        launcher_layout.bt_in=lvui_display_change(3);
    }
    launcher_layout.hor_num=(ui_layout.base.hor-launcher_layout.hor_gap*2)/(launcher_layout.hor+launcher_layout.hor_gap)+1;
    if(launcher_layout.hor_num<1)
    {
        return;
    }
    uint16_t num=launcher_layout.hor_num;
    if(num>1)
    {
        while(launcher_layout.hor_num==num)
        {
            ++launcher_layout.hor_gap;
            launcher_layout.hor_num=(ui_layout.base.hor-launcher_layout.hor_gap*2)/(launcher_layout.hor+launcher_layout.hor_gap);
        }
        launcher_layout.hor_gap=(ui_layout.base.hor-launcher_layout.hor*launcher_layout.hor_num)/(launcher_layout.hor_num+1);
    }
}

//build launcher
static void build_launcher()
{
    //creat
    //launcher
    ui_widget.launcher=lv_obj_create(ui_widget.base);
    //layout
    //launcher
    ui_layout.launcher.left=0;
    ui_layout.launcher.top=0;
    ui_layout.launcher.hor=ui_layout.base.hor;
    ui_layout.launcher.ver=ui_layout.base.ver;
    //init
    //launcher
    lv_obj_set_pos(ui_widget.launcher,ui_layout.launcher.left,ui_layout.launcher.top);
    lv_obj_set_size(ui_widget.launcher,ui_layout.launcher.hor,ui_layout.launcher.ver);
    lv_obj_set_style_pad_left(ui_widget.launcher,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_widget.launcher,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_widget.launcher,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_widget.launcher,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_widget.launcher,255,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_widget.launcher,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_widget.launcher,0,LV_PART_MAIN|LV_STATE_DEFAULT);
}

//lvui launcher init
void lvui_launcher_init()
{
    uint16_t hor=0;
    uint16_t ver=0;
    lvui_manager_screen(&hor,&ver);
    //build base
    build_base(hor,ver);
    //launcher layout init
    launcher_layout_init();
    //build launcher
    build_launcher();
    //applist init
    applist_init();
}

//lvui launcher load
lv_obj_t*lvui_launcher_load()
{
    return ui_widget.base;
}

//applist init
void applist_init()
{
    applist=malloc(sizeof(applist_t));
    applist->last=NULL;
    applist->button=NULL;
    applist->lable=NULL;
    applist->app_screen=NULL;
    applist->text=NULL;
    applist->number=0;
    applist->next=NULL;
    applist_current=NULL;
}

//applist open
static void applist_open(lv_event_t*e)
{
    applist_t*applist=lv_event_get_user_data(e);
    applist_current=applist;
    lv_obj_remove_flag(applist_current->app_screen,LV_OBJ_FLAG_HIDDEN);
    lv_anim_start(&applist_current->app_in);
}

//animation app
static void animation_app(void*obj,int32_t value)
{
    //realize animation effects
    lv_obj_set_style_opa((lv_obj_t*)obj,value,LV_PART_MAIN|LV_STATE_DEFAULT);
}

//animation app end
static void animation_app_end()
{
    //hide
    lv_obj_add_flag(applist_current->app_screen,LV_OBJ_FLAG_HIDDEN);
    applist_current=NULL;
}

//applist build
static void applist_build(applist_t*applist_new)
{
    uint16_t hor=launcher_layout.hor;
    uint16_t ver=launcher_layout.ver;
    uint16_t hor_gap=launcher_layout.hor_gap;
    uint16_t ver_gap=launcher_layout.ver_gap;
    uint16_t hor_num=launcher_layout.hor_num;
    //layout
    uint16_t left=0;
    uint16_t top=0;
    uint8_t number=applist_new->number;
    if(number%hor_num==0)
    {
        left=hor_gap+(hor_num-1)*(hor+hor_gap);
        top=lvui_statusbar_height()+ver_gap+(number/hor_num-1)*(ver+ver_gap);
    }else
    {
        left=hor_gap+((number%hor_num)-1)*(hor+hor_gap);
        top=lvui_statusbar_height()+ver_gap+(number/hor_num)*(ver+ver_gap);
    }
    lv_obj_t*obj=NULL;
    //button
    obj=lv_btn_create(ui_widget.launcher);
    applist_new->button=obj;
    lv_obj_set_pos(obj,left,top);
    lv_obj_set_size(obj,hor,ver);
    lv_obj_set_style_radius(obj,launcher_layout.bt_rd,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_transform_width(obj,-launcher_layout.bt_in,LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_transform_height(obj,-launcher_layout.bt_in,LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_add_event_cb(obj,(lv_event_cb_t)applist_open,LV_EVENT_CLICKED,applist_new);
    //lable
    obj=lv_label_create(applist_new->button);
    applist_new->lable=obj;
    lv_obj_set_pos(obj,0,0);
    lv_obj_set_size(obj,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
    lv_label_set_text(obj,applist_new->text);
    lv_obj_set_style_align(obj,LV_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
    //add app
    lv_obj_set_parent(applist_new->app_screen,ui_widget.base);
    lv_obj_set_style_opa(applist_new->app_screen,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_flag(applist_new->app_screen,LV_OBJ_FLAG_HIDDEN);
    //animation in
    lv_anim_init(&applist_new->app_in);
    lv_anim_set_var(&applist_new->app_in,applist_new->app_screen);
    lv_anim_set_values(&applist_new->app_in,0,255);
    lv_anim_set_time(&applist_new->app_in,200);
    lv_anim_set_exec_cb(&applist_new->app_in,(lv_anim_exec_xcb_t)animation_app);
    lv_anim_set_path_cb(&applist_new->app_in,lv_anim_path_linear);
    //animation out
    lv_anim_init(&applist_new->app_out);
    lv_anim_set_var(&applist_new->app_out,applist_new->app_screen);
    lv_anim_set_values(&applist_new->app_out,255,0);
    lv_anim_set_time(&applist_new->app_out,200);
    lv_anim_set_exec_cb(&applist_new->app_out,(lv_anim_exec_xcb_t)animation_app);
    lv_anim_set_path_cb(&applist_new->app_out,lv_anim_path_linear);
    lv_anim_set_ready_cb(&applist_new->app_out,(lv_anim_ready_cb_t)animation_app_end);
    //load statusbar
    lv_obj_move_foreground(lvui_statusbar_load());
    //load controlcenter
    lv_obj_move_foreground(lvui_controlcenter_load());
}

//applist add
void applist_add(char*text,lv_obj_t*app_screen)
{
    applist_t*applist_old=applist;
    while(applist_old->next!=NULL)
    {
        applist_old=applist_old->next;
    }
    uint8_t number=applist_old->number;
    applist_t*applist_new=malloc(sizeof(applist_t));
    applist_old->next=applist_new;
    applist_new->last=applist_old;
    applist_new->button=NULL;
    applist_new->app_screen=app_screen;
    applist_new->text=text;
    applist_new->number=number+1;
    applist_new->next=NULL;
    applist_build(applist_new);
}

//lvui launcher back
void lvui_launcher_back()
{
    //back launcher
    if(applist_current!=NULL)
    {
        lv_anim_start(&applist_current->app_out);
    }

}

#endif//#ifndef LVUI_LAUNCHER_H
