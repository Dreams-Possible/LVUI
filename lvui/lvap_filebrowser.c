#include"lvap_filebrowser.h"
#ifdef LVAP_FILEBROWSER_H

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
    layout_info_t filebrowser;
    layout_info_t title;
    layout_info_t title_lable;
    layout_info_t back;
    layout_info_t back_lable;
    layout_info_t list;
}ui_layout_t;
static ui_layout_t ui_layout;

//ui widget
typedef struct ui_widget_t
{
    lv_obj_t*base;
    lv_obj_t*filebrowser;
    lv_obj_t*title;
    lv_obj_t*title_lable;
    lv_obj_t*back;
    lv_obj_t*back_lable;
    lv_obj_t*list;
}ui_widget_t;
static ui_widget_t ui_widget;

//filebrowser
typedef struct filebrowser_t
{
    struct filebrowser_t*last;
    lv_obj_t*button;
    lv_obj_t*lable;
    char*text;
    uint8_t number;
    struct filebrowser_t*next;
}filebrowser_t;
filebrowser_t*filebrowser;

//build base
static void build_base(uint16_t hor,uint16_t ver);
//build filebrowser
static void build_filebrowser();

// //filebrowser open
// static void filebrowser_open(lv_event_t*e);
//filebrowser init
static void filebrowser_init();
//filebrowser back
static void filebrowser_back(lv_event_t*e);
//filebrowser open
static void filebrowser_open(lv_event_t*e);
//filebrowser build
static void filebrowser_build(filebrowser_t*filebrowser_new);
//filebrowser add
void filebrowser_add(char*text);
//filebrowser clean
void filebrowser_clean();
//lvui init filebrowser
void lvui_filebrowser_init();
//lvui load filebrowser
lv_obj_t*lvui_filebrowser_load();

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

//build filebrowser
static void build_filebrowser()
{
    //creat
    //filebrowser
    ui_widget.filebrowser=lv_obj_create(ui_widget.base);
    //title
    ui_widget.title=lv_obj_create(ui_widget.filebrowser);
        //lable
        ui_widget.title_lable=lv_label_create(ui_widget.title);
    //back
    ui_widget.back=lv_obj_create(ui_widget.filebrowser);
        //lable
        ui_widget.back_lable=lv_label_create(ui_widget.back);
    //list
    ui_widget.list=lv_obj_create(ui_widget.filebrowser);
    //layout
    //filebrowser
    ui_layout.filebrowser.left=0;
    ui_layout.filebrowser.top=0;
    ui_layout.filebrowser.hor=ui_layout.base.hor;
    ui_layout.filebrowser.ver=ui_layout.base.ver;
    uint16_t ver=lvui_display_change(56);
    uint16_t hor_gap=lvui_display_change(12);
    //title
    ui_layout.title.left=0;
    ui_layout.title.top=lvui_statusbar_height();
    ui_layout.title.hor=ui_layout.base.hor;
    ui_layout.title.ver=ver;
        //lable
        lv_obj_update_layout(ui_widget.title_lable);
        uint16_t title_lable_ver=lv_obj_get_height(ui_widget.title_lable);
        ui_layout.title_lable.left=hor_gap;
        ui_layout.title_lable.top=ui_layout.title.ver/2-title_lable_ver/2;
        ui_layout.title_lable.hor=0;
        ui_layout.title_lable.ver=0;
    //back
    ui_layout.back.left=0;
    ui_layout.back.top=lvui_statusbar_height()+ui_layout.title.ver;
    ui_layout.back.hor=ui_layout.base.hor;
    ui_layout.back.ver=ver;
        //lable
        lv_obj_update_layout(ui_widget.back_lable);
        uint16_t back_lable_ver=lv_obj_get_height(ui_widget.back_lable);
        ui_layout.back_lable.left=hor_gap;
        ui_layout.back_lable.top=ui_layout.back.ver/2-back_lable_ver/2;
        ui_layout.back_lable.hor=0;
        ui_layout.back_lable.ver=0;
    //list
    ui_layout.list.left=0;
    ui_layout.list.top=lvui_statusbar_height()+ui_layout.title.ver+ui_layout.back.ver;
    ui_layout.list.hor=ui_layout.base.hor;
    ui_layout.list.ver=ui_layout.base.ver-ui_layout.list.top;
    //init
    //filebrowser
    lv_obj_set_pos(ui_widget.filebrowser,ui_layout.filebrowser.left,ui_layout.filebrowser.top);
    lv_obj_set_size(ui_widget.filebrowser,ui_layout.filebrowser.hor,ui_layout.filebrowser.ver);
    lv_obj_set_style_pad_left(ui_widget.filebrowser,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_widget.filebrowser,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_widget.filebrowser,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_widget.filebrowser,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_widget.filebrowser,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_widget.filebrowser,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_widget.filebrowser,255,LV_PART_MAIN|LV_STATE_DEFAULT);
    //title
    lv_obj_set_pos(ui_widget.title,ui_layout.title.left,ui_layout.title.top);
    lv_obj_set_size(ui_widget.title,ui_layout.title.hor,ui_layout.title.ver);
    lv_obj_set_style_pad_left(ui_widget.title,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_widget.title,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_widget.title,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_widget.title,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_widget.title,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_widget.title,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_widget.title,lv_color_hex(0xffd0d0d0),LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_widget.title,255,LV_PART_MAIN|LV_STATE_DEFAULT);
        //lable
        lv_obj_set_pos(ui_widget.title_lable,ui_layout.title_lable.left,ui_layout.title_lable.top);
        lv_obj_set_size(ui_widget.title_lable,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
        lv_label_set_text(ui_widget.title_lable,"title");
        lv_obj_set_style_text_color(ui_widget.title_lable,lv_color_hex(0xff000000),LV_PART_MAIN|LV_STATE_DEFAULT);
    //back
    lv_obj_set_pos(ui_widget.back,ui_layout.back.left,ui_layout.back.top);
    lv_obj_set_size(ui_widget.back,ui_layout.back.hor,ui_layout.back.ver);
    lv_obj_set_style_pad_left(ui_widget.back,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_widget.back,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_widget.back,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_widget.back,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_widget.back,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_widget.back,lv_color_hex(0xffffffff),LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_widget.back,255,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_widget.back,lv_color_hex(0xff000000),LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_widget.back,100,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_widget.back,2,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_widget.back,LV_BORDER_SIDE_BOTTOM,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_widget.back,lv_color_hex(0xff000000),LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_widget.back,50,LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_add_event_cb(ui_widget.back,(lv_event_cb_t)filebrowser_back,LV_EVENT_CLICKED,NULL);
        //lable
        lv_obj_set_pos(ui_widget.back_lable,ui_layout.back_lable.left,ui_layout.back_lable.top);
        lv_obj_set_size(ui_widget.back_lable,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
        lv_label_set_text(ui_widget.back_lable,"..");
        lv_obj_set_style_text_color(ui_widget.back_lable,lv_color_hex(0xff000000),LV_PART_MAIN|LV_STATE_DEFAULT);
    //list
    lv_obj_set_pos(ui_widget.list,ui_layout.list.left,ui_layout.list.top);
    lv_obj_set_size(ui_widget.list,ui_layout.list.hor,ui_layout.list.ver);
    lv_obj_set_style_pad_left(ui_widget.list,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_widget.list,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_widget.list,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_widget.list,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_widget.list,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_widget.list,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_widget.list,255,LV_PART_MAIN|LV_STATE_DEFAULT);
}

//filebrowser init
static void filebrowser_init()
{
    filebrowser=malloc(sizeof(filebrowser_t));
    filebrowser->last=NULL;
    filebrowser->button=NULL;
    filebrowser->lable=NULL;
    filebrowser->text=NULL;
    filebrowser->number=0;
    filebrowser->next=NULL;
}

//filebrowser back
static void filebrowser_back(lv_event_t*e)
{
    printf("back\n");
}


//filebrowser open
static void filebrowser_open(lv_event_t*e)
{
    printf("open\n");
}

//filebrowser build
static void filebrowser_build(filebrowser_t*filebrowser_new)
{
    uint8_t number=filebrowser_new->number;
    //layout
    uint16_t top=ui_layout.back.ver*(number-1);
    lv_obj_t*obj=NULL;
    //button
    obj=lv_obj_create(ui_widget.list);
    filebrowser_new->button=obj;
    lv_obj_set_pos(obj,ui_layout.back.left,top);
    lv_obj_set_size(obj,ui_layout.back.hor,ui_layout.back.ver);
    lv_obj_set_style_pad_left(obj,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(obj,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(obj,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(obj,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj,0,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj,lv_color_hex(0xffffffff),LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj,255,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(obj,lv_color_hex(0xff000000),LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(obj,100,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(obj,2,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(obj,LV_BORDER_SIDE_BOTTOM,LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj,lv_color_hex(0xff000000),LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(obj,50,LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_add_event_cb(obj,(lv_event_cb_t)filebrowser_open,LV_EVENT_CLICKED,NULL);
        //lable
        obj=lv_label_create(filebrowser_new->button);
        filebrowser_new->lable=obj;
        lv_obj_set_pos(obj,ui_layout.back_lable.left,ui_layout.back_lable.top);
        lv_obj_set_size(obj,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
        lv_label_set_text(obj,filebrowser_new->text);
        lv_obj_set_style_text_color(obj,lv_color_hex(0xff000000),LV_PART_MAIN|LV_STATE_DEFAULT);
}

//filebrowser add
void filebrowser_add(char*text)
{
    filebrowser_t*filebrowser_old=filebrowser;
    while(filebrowser_old->next!=NULL)
    {
        printf("addnum=%d\n",filebrowser_old->number);
        filebrowser_old=filebrowser_old->next;
    }
    uint8_t number=filebrowser_old->number;
    filebrowser_t*filebrowser_new=malloc(sizeof(filebrowser_t));
    filebrowser_old->next=filebrowser_new;
    filebrowser_new->last=filebrowser_old;
    filebrowser_new->button=NULL;
    filebrowser_new->text=text;
    filebrowser_new->number=number+1;
    printf("addnumber=%d\n",filebrowser_new->number);
    filebrowser_new->next=NULL;
    filebrowser_build(filebrowser_new);
}

//filebrowser clean
void filebrowser_clean()
{
    filebrowser_t*filebrowser_old=filebrowser;
    while(filebrowser_old->next!=NULL)
    {
        if(filebrowser_old->number!=0)
        {
            printf("delnumber=%d\n",filebrowser_old->number);
            lv_obj_del(filebrowser_old->button);
        }
        filebrowser_old=filebrowser_old->next;
        printf("delnextnumber=%d\n",filebrowser_old->number);
        if(filebrowser_old->last->number!=0)
        {
            free(filebrowser_old->last);
        }
    }
    if(filebrowser_old->number!=0)
    {
        lv_obj_del(filebrowser_old->button);
        free(filebrowser_old);        
    }
    filebrowser->next=NULL;
}

//filebrowser title
void filebrowser_title(char*text)
{
    lv_label_set_text(ui_widget.title_lable,text);
}

//lvui init filebrowser
void lvui_filebrowser_init()
{
    uint16_t hor=0;
    uint16_t ver=0;
    lvui_manager_screen(&hor,&ver);
    //build base
    build_base(hor,ver);
    //build filebrowser
    build_filebrowser();
    //filebrowser init
    filebrowser_init();
}

//lvui load filebrowser
lv_obj_t*lvui_filebrowser_load()
{
    return ui_widget.filebrowser;
}

#endif//#ifndef LVAP_FILEBROWSER_H
