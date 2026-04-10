#include"osui_ui_desktop.h"

//UI框架
#include"osui/ui/osui_ui_frame.h"
//UI标签
#include"osui/ui/osui_ui_lable.h"
//UI动画
#include"osui/ui/osui_ui_anim.h"
//UI调试
#include"osui/ui/osui_ui_debug.h"

//参数定义
#define DESKTOP_HOME_POS (osui_disp_get()->ver)//桌面位置
// #define DESKTOP_ANIM_APP_POS (osui_ui_statbar_get_height())//应用位置
#define DESKTOP_STATBAR_HEIGHT (osui_ui_statbar_get_height())//桌面状态栏高度
#define DESKTOP_NAVBAR_HEIGHT (osui_ui_navbar_get_height())//桌面导航条高度
#define DESKTOP_ANIM_HOME_POS DESKTOP_HOME_POS//桌面动画桌面位置
#define DESKTOP_ANIM_APP_POS (0)//桌面动画应用位置
#define DESKTOP_ANIM_MASK_HOME_OPA (LV_OPA_0)//桌面动画遮罩桌面透明度
#define DESKTOP_ANIM_MASK_APP_OPA (LV_OPA_20)//桌面动画遮罩应用透明度
#define DESKTOP_APP_BG_OPA (LV_OPA_0)//桌面应用背景透明度
#define DESKTOP_APP_OPA (LV_OPA_20)//桌面应用透明度
#define DESKTOP_INFO_LAYOUT_LEFT_RATIO (0.1)//桌面信息布局左间距比例
#define DESKTOP_INFO_LAYOUT_TOP_RATIO (0.1)//桌面信息布局顶间距比例
#define DESKTOP_LIST_LAYOUT_HOR_RATIO (0.6)//桌面列表布局水平比例
#define DESKTOP_LIST_LAYOUT_VER_RATIO (0.6)//桌面列表布局垂直比例
#define DESKTOP_SAMPLE_TIME (LV_INDEV_STATE_PR*2)//桌面采样时间
#define DESKTOP_APP_LAUNCH_TIME (600)//应用启动动画时间
#define DESKTOP_APP_DELAUNCH_TIME (600)//应用关闭动画时间
#define DESKTOP_APP_ANIM_LV_PATH_P1 (1024)//桌面应用动画LVGL轨迹参数1
#define DESKTOP_APP_ANIM_LV_PATH_P2 (1024)//桌面应用动画LVGL轨迹参数2
#define DESKTOP_APP_LIST_ITEM_HEIGHT_DP (32)//桌面应用列表项目DP高度
#define DESKTOP_APP_LIST_ITEM_HEIGHT ((int16_t)(osui_disp_dp2px(DESKTOP_APP_LIST_ITEM_HEIGHT_DP)))//桌面应用列表项目高度
#define DESKTOP_APP_LIST_ITEM_GAP ((int16_t)(osui_disp_dp2px(4)))//桌面应用列表项目间距

//桌面UI类
typedef struct desktop_ui_t
{
    lv_obj_t * parent;
    lv_obj_t * back;
    lv_obj_t * front;
    lv_obj_t * time;
    lv_obj_t * date;
    lv_obj_t * app_list;
    lv_obj_t * app_mask;
    lv_obj_t * app_scr;
    lv_obj_t * nav_bar;
    lv_obj_t * stat_bar;
} desktop_ui_t;
static desktop_ui_t desktop_ui = {0};

//桌面动画状态类
typedef enum desktop_anim_status_t
{
    DESKTOP_HOME = 0,
    DESKTOP_APP = 1,
    DESKTOP_LAUNCH = 2,
    DESKTOP_DELAUNCH = 3,
}desktop_anim_status_t;
//桌面逻辑类
typedef struct desktop_logic_t
{
    lv_obj_t* curr_app;
    // osui_ui_anim_t* app_anim;
    osui_ui_anim_lv_t* app_anim;
    // osui_ui_anim_t* mask_anim;
    osui_ui_anim_lv_t* mask_anim;
    desktop_anim_status_t anim_status;
} desktop_logic_t;
static desktop_logic_t desktop_logic = {0};

//桌面UI初始化
// lv_obj_t*osui_ui_navbar_init();
// void osui_ui_navbar_init_clean();
// lv_obj_t*osui_ui_statbar_init();
// void osui_ui_statbar_init_clean();
static void desktop_ui_init();
//添加应用
static void add_app( const char *name, lv_obj_t*app);
//进入锁屏
void osui_ui_locker_enter();
static void enter_locker(lv_event_t*e);
//桌面运行时
bool osui_ui_locker_get_status();
static void desktop_runtime(lv_timer_t*timer);
//启动应用
uint16_t osui_ui_statbar_get_height();
uint16_t osui_ui_navbar_get_height();
static void app_launch(lv_event_t * e);
//退出应用
static void app_delaunch();
//应用打开回调
static void app_launch_cb();
//应用退出回调
static void app_delaunch_cb();
//桌面逻辑初始化
static void desktop_logic_init();
//UI桌面初始化
void osui_ui_desktop_init();
//桌面添加应用
void osui_ui_desktop_add_app(const char *name, lv_obj_t*app);
// //桌面启动应用
// void osui_ui_desktop_launch(lv_obj_t*app);
//桌面退出应用
void osui_ui_desktop_delaunch();
//获取桌面宽度
int16_t osui_ui_desktop_get_width();
//获取桌面高度
int16_t osui_ui_desktop_get_height();
//设置桌面时间
void osui_ui_desktop_set_time(const char* time);
//设置桌面日期
void osui_ui_desktop_set_date(const char* date);
//设置桌面字体颜色
void osui_ui_desktop_set_font_color(lv_color_t color);
//设置桌面背景颜色
void osui_ui_desktop_set_back_color(lv_color_t color);
//设置桌面壁纸
void osui_ui_desktop_set_wallpaper(char* path);

//桌面UI初始化
static void desktop_ui_init()
{
    //获取显示对象
    osui_dispinfo_t*osui_dispinfo=osui_disp_get();
    //获取UI框架对象
    lv_obj_t *obj = *osui_uiframe_get();
    //初始化桌面父对象
    desktop_ui.parent = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, osui_dispinfo->hor, osui_dispinfo->ver);
    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    //绘制桌面样式
    lv_obj_t *parent_obj = obj;
    {
        //背景
        lv_obj_t *obj = lv_image_create(parent_obj);
        desktop_ui.back = obj;
        lv_obj_set_pos(obj, 0, 0);
        lv_obj_set_size(obj, osui_dispinfo->hor, osui_dispinfo->ver);
        lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    {
        //前景
        lv_obj_t *obj = lv_obj_create(parent_obj);
        desktop_ui.front = obj;
        lv_obj_set_pos(obj, 0, 0);
        lv_obj_set_size(obj, osui_dispinfo->hor, osui_dispinfo->ver);
        lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        {
            lv_obj_t *parent_obj = obj;
            //计算布局
            uint16_t layout_hor=(uint16_t)(osui_dispinfo->hor*DESKTOP_INFO_LAYOUT_LEFT_RATIO);
            uint16_t layout_ver=(uint16_t)((osui_dispinfo->ver-DESKTOP_STATBAR_HEIGHT)*DESKTOP_INFO_LAYOUT_TOP_RATIO);
            {
                //桌面时间
                lv_obj_t *obj = lv_label_create(parent_obj);
                desktop_ui.time = obj;
                lv_obj_set_pos(obj, layout_hor, layout_ver);
                lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_text_font(obj, &OSUI_UI_LABLE_FONT_MAX, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_label_set_text(obj, "9:30");
                //更新布局
                layout_ver+=osui_ui_lable_get_ver(&OSUI_UI_LABLE_FONT_MAX);
            }
            {
                //桌面日期
                lv_obj_t *obj = lv_label_create(parent_obj);
                desktop_ui.date = obj;
                lv_obj_set_pos(obj, layout_hor, layout_ver);
                lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_text_font(obj, &OSUI_UI_LABLE_FONT_MIN, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_label_set_text(obj, "January 1");
                //更新布局
                layout_ver+=osui_ui_lable_get_ver(&OSUI_UI_LABLE_FONT_MIN);
            }
            {
                //更新布局
                layout_ver+=osui_ui_lable_get_ver(&OSUI_UI_LABLE_FONT_MIN);
                //桌面应用列表
                lv_obj_t * obj = lv_obj_create(parent_obj);
                desktop_ui.app_list = obj;
                lv_obj_set_pos(obj, layout_hor, layout_ver);
                lv_obj_set_size(obj, (int16_t)(osui_dispinfo->hor*DESKTOP_LIST_LAYOUT_HOR_RATIO), 0);
                lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_pad_row(obj, DESKTOP_APP_LIST_ITEM_GAP, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                // lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
                lv_obj_set_scroll_dir(obj, LV_DIR_VER);
            }
        }
    }
    {
        //遮罩
        lv_obj_t *obj = lv_obj_create(parent_obj);
        desktop_ui.app_mask = obj;
        lv_obj_set_pos(obj, 0, 0);
        lv_obj_set_size(obj, osui_dispinfo->hor, osui_dispinfo->ver);
        // lv_obj_set_pos(obj, 0, DESKTOP_STATBAR_HEIGHT);
        // lv_obj_set_size(obj, osui_dispinfo->hor, osui_dispinfo->ver-DESKTOP_STATBAR_HEIGHT);
        lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
    {
        //应用
        lv_obj_t *obj = lv_obj_create(parent_obj);
        desktop_ui.app_scr = obj;
        lv_obj_set_pos(obj, 0, DESKTOP_HOME_POS);
        lv_obj_set_size(obj, osui_dispinfo->hor, osui_dispinfo->ver);
        lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    // {
    //     //导航条
    //     lv_obj_t *obj = osui_ui_navbar_init();
    //     desktop_ui.nav_bar = obj;
    //     lv_obj_set_parent(obj, parent_obj);
    //     osui_ui_navbar_init_clean();
    // }
    // {
    //     //状态栏
    //     lv_obj_t *obj = osui_ui_statbar_init();
    //     desktop_ui.stat_bar = obj;
    //     lv_obj_set_parent(obj, parent_obj);
    //     osui_ui_statbar_init_clean();
    // }
}

//添加应用
static void add_app(const char *name, lv_obj_t*app)
{
    //创建图标
    // lv_obj_t * btn = lv_list_add_button(desktop_ui.app_list, icon, name);
    lv_obj_t * btn = lv_button_create(desktop_ui.app_list);
    lv_obj_set_pos(btn, 0, 0);
    lv_obj_set_size(btn, (int16_t)(osui_disp_get()->hor*DESKTOP_LIST_LAYOUT_HOR_RATIO), DESKTOP_APP_LIST_ITEM_HEIGHT);
    lv_obj_set_style_transform_height(btn, 0, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_transform_width(btn, 0, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_add_event_cb(btn, app_launch, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label = lv_label_create(btn);
    lv_obj_set_pos(label, 0, 0);
    lv_obj_set_size(label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_text_color(label, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &OSUI_UI_LABLE_FONT_CONTENT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text_fmt(label, name);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    //调整布局
    uint16_t curr_ver=lv_obj_get_child_count(desktop_ui.app_list)*(DESKTOP_APP_LIST_ITEM_HEIGHT+DESKTOP_APP_LIST_ITEM_GAP);
    // osui_log("count:%d",lv_obj_get_child_count(desktop_ui.app_list));
    // osui_log("height:%d",lv_obj_get_height(btn));
    // osui_log("curr_ver:%d",curr_ver);
    uint16_t max_ver=(int16_t)(osui_disp_get()->ver*DESKTOP_LIST_LAYOUT_VER_RATIO);
    // uint16_t max_ver=(int16_t)(osui_disp_get()->ver);
    // osui_log("max_ver:%d",max_ver);
    if(curr_ver<max_ver)
    {
        lv_obj_set_height(desktop_ui.app_list, curr_ver);
    }else
    {
        lv_obj_set_height(desktop_ui.app_list, max_ver);
    }
    //新建界面
    lv_obj_t *obj = lv_obj_create(desktop_ui.app_scr);
    lv_obj_set_pos(obj, 0, 0);
    // lv_obj_set_size(app, osui_disp_get()->hor, osui_disp_get()->ver-DESKTOP_STATBAR_HEIGHT);
    lv_obj_set_size(obj, osui_disp_get()->hor, osui_disp_get()->ver);
    // lv_obj_set_size(obj, osui_dispinfo->hor, osui_dispinfo->ver);
    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, DESKTOP_APP_BG_OPA, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(obj,LV_OBJ_FLAG_HIDDEN);
    //添加界面
    lv_obj_set_pos(app, 0, DESKTOP_STATBAR_HEIGHT);
    lv_obj_set_size(app, osui_disp_get()->hor, osui_disp_get()->ver-DESKTOP_STATBAR_HEIGHT);
    lv_obj_set_style_pad_left(app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(app, DESKTOP_APP_OPA, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_flag(app, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_parent(app, obj);
    lv_obj_set_user_data(btn,obj);
}

//进入锁屏
static void enter_locker(lv_event_t*e)
{
    osui_log("enter locker");
    osui_ui_locker_enter();
}

//桌面运行时
static void desktop_runtime(lv_timer_t*timer)
{
    //运行时检查
    if(!osui_ui_locker_get_status())
    {
        switch (desktop_logic.anim_status)
        {
            //桌面
            case DESKTOP_HOME:
            break;
            //启动
            case DESKTOP_LAUNCH:
                // //等待启动完成
                // if(osui_ui_anim_finish(desktop_logic.app_anim))
                // {
                //     //更新状态
                //     desktop_logic.anim_status=DESKTOP_APP;
                //     osui_log("enter app");
                //     // //立刻退出
                //     // app_delaunch();
                // }
            break;
            //应用
            case DESKTOP_APP:
            break;
            //退出
            case DESKTOP_DELAUNCH:
                // //等待退出完成
                // if(osui_ui_anim_finish(desktop_logic.app_anim))
                // {
                //     //更新状态
                //     desktop_logic.anim_status=DESKTOP_HOME;
                //     //隐藏应用
                //     lv_obj_add_flag(desktop_logic.curr_app,LV_OBJ_FLAG_HIDDEN);
                //     //隐藏遮罩
                //     lv_obj_add_flag(desktop_ui.app_mask, LV_OBJ_FLAG_HIDDEN);
                //     //清除当前应用
                //     desktop_logic.curr_app=NULL;
                //     osui_log("enter home");
                // }
            break;
            default:
            break;
        }
        // //应用动画
        // if(!osui_ui_anim_finish(desktop_logic.app_anim))
        // {
        //     osui_ui_anim_exec(desktop_logic.app_anim);
        // }
        // //遮罩动画
        // if(!osui_ui_anim_finish(desktop_logic.mask_anim))
        // {
        //     osui_ui_anim_exec(desktop_logic.mask_anim);
        // }
    }
}

//启动应用
static void app_launch(lv_event_t * e)
{
    //更新状态
    desktop_logic.anim_status=DESKTOP_LAUNCH;
    lv_obj_t * obj = lv_event_get_target(e);
    //更新当前应用
    desktop_logic.curr_app=lv_obj_get_user_data(obj);
    //显示应用
    lv_obj_remove_flag(desktop_logic.curr_app,LV_OBJ_FLAG_HIDDEN);
    //显示遮罩
    lv_obj_remove_flag(desktop_ui.app_mask, LV_OBJ_FLAG_HIDDEN);
    // //更新动画目标
    // osui_ui_anim_set_tg(desktop_logic.app_anim,DESKTOP_ANIM_APP_POS);
    // osui_ui_anim_set_tg(desktop_logic.mask_anim,DESKTOP_ANIM_MASK_APP_OPA);
    //应用动画
    osui_ui_anim_lv_set_param(desktop_logic.app_anim, DESKTOP_ANIM_HOME_POS, DESKTOP_ANIM_APP_POS);
    osui_ui_anim_lv_set_time(desktop_logic.app_anim, DESKTOP_APP_LAUNCH_TIME);
    osui_ui_anim_lv_set_exec_cb(desktop_logic.app_anim, osui_ui_anim_lv_exec_cb_y);
    osui_ui_anim_lv_set_finish_cb(desktop_logic.app_anim, app_launch_cb);
    osui_ui_anim_lv_set_bezier_param(desktop_logic.app_anim, DESKTOP_APP_ANIM_LV_PATH_P1, DESKTOP_APP_ANIM_LV_PATH_P2);
    osui_ui_anim_lv_start(desktop_logic.app_anim);
    //遮罩动画
    osui_ui_anim_lv_set_param(desktop_logic.mask_anim, DESKTOP_ANIM_MASK_HOME_OPA, DESKTOP_ANIM_MASK_APP_OPA);
    osui_ui_anim_lv_set_time(desktop_logic.mask_anim, DESKTOP_APP_LAUNCH_TIME);
    osui_ui_anim_lv_set_exec_cb(desktop_logic.mask_anim, osui_ui_anim_lv_exec_cb_bg_opa);
    // osui_ui_anim_lv_set_finish_cb(desktop_logic.mask_anim, app_launch_cb);
    osui_ui_anim_lv_set_bezier_param(desktop_logic.mask_anim, DESKTOP_APP_ANIM_LV_PATH_P1, DESKTOP_APP_ANIM_LV_PATH_P2);
    osui_ui_anim_lv_start(desktop_logic.mask_anim);
    osui_log("app launch");
}

//退出应用
static void app_delaunch()
{
    //检查状态
    if(desktop_logic.curr_app)
    {
        //更新状态
        desktop_logic.anim_status=DESKTOP_DELAUNCH;
        // //更新动画目标
        // osui_ui_anim_set_tg(desktop_logic.app_anim,DESKTOP_HOME_POS);
        // osui_ui_anim_set_tg(desktop_logic.mask_anim,DESKTOP_ANIM_MASK_HOME_OPA);
        //应用动画
        osui_ui_anim_lv_set_param(desktop_logic.app_anim, DESKTOP_ANIM_APP_POS, DESKTOP_ANIM_HOME_POS);
        osui_ui_anim_lv_set_time(desktop_logic.app_anim, DESKTOP_APP_DELAUNCH_TIME);
        osui_ui_anim_lv_set_exec_cb(desktop_logic.app_anim, osui_ui_anim_lv_exec_cb_y);
        osui_ui_anim_lv_set_finish_cb(desktop_logic.app_anim, app_delaunch_cb);
        osui_ui_anim_lv_set_bezier_param(desktop_logic.app_anim, DESKTOP_APP_ANIM_LV_PATH_P1, DESKTOP_APP_ANIM_LV_PATH_P2);
        osui_ui_anim_lv_start(desktop_logic.app_anim);
        //遮罩动画
        osui_ui_anim_lv_set_param(desktop_logic.mask_anim, DESKTOP_ANIM_MASK_APP_OPA, DESKTOP_ANIM_MASK_HOME_OPA);
        osui_ui_anim_lv_set_time(desktop_logic.mask_anim, DESKTOP_APP_DELAUNCH_TIME);
        osui_ui_anim_lv_set_exec_cb(desktop_logic.mask_anim, osui_ui_anim_lv_exec_cb_bg_opa);
        // osui_ui_anim_lv_set_finish_cb(desktop_logic.mask_anim, app_launch_cb);
        osui_ui_anim_lv_set_bezier_param(desktop_logic.mask_anim, DESKTOP_APP_ANIM_LV_PATH_P1, DESKTOP_APP_ANIM_LV_PATH_P2);
        osui_ui_anim_lv_start(desktop_logic.mask_anim);
    }
}

//应用打开回调
static void app_launch_cb()
{
    //更新状态
    desktop_logic.anim_status=DESKTOP_APP;
    osui_log("launch app");
}

//应用退出回调
static void app_delaunch_cb()
{
    //更新状态
    desktop_logic.anim_status=DESKTOP_HOME;
    //隐藏应用
    lv_obj_add_flag(desktop_logic.curr_app,LV_OBJ_FLAG_HIDDEN);
    //隐藏遮罩
    lv_obj_add_flag(desktop_ui.app_mask, LV_OBJ_FLAG_HIDDEN);
    //清除当前应用
    desktop_logic.curr_app=NULL;
    osui_log("enter home");
}

//桌面逻辑初始化
static void desktop_logic_init()
{
    //长按桌面
    lv_obj_add_event_cb(desktop_ui.front, enter_locker, LV_EVENT_LONG_PRESSED, NULL);
    //创建运行时
    lv_timer_create(desktop_runtime,DESKTOP_SAMPLE_TIME,NULL);
    //应用动画
    desktop_logic.app_anim = osui_ui_anim_lv_init();
    osui_ui_anim_lv_set_obj(desktop_logic.app_anim, desktop_ui.app_scr);
    // desktop_logic.app_anim = osui_ui_anim_init();
    // osui_ui_anim_set_obj(desktop_logic.app_anim, desktop_ui.app_scr);
    // osui_ui_anim_set_type(desktop_logic.app_anim, OSUI_UI_ANIM_TYPE_Y);
    // osui_ui_anim_set_pd(desktop_logic.app_anim, DESKTOP_APP_ANIM_PDC_P, DESKTOP_APP_ANIM_PDC_D);
    // osui_ui_anim_set_tg(desktop_logic.app_anim,DESKTOP_HOME_POS);
    //遮罩动画
    desktop_logic.mask_anim = osui_ui_anim_lv_init();
    osui_ui_anim_lv_set_obj(desktop_logic.mask_anim, desktop_ui.app_mask);
    // desktop_logic.mask_anim = osui_ui_anim_init();
    // osui_ui_anim_set_obj(desktop_logic.mask_anim, desktop_ui.app_mask);
    // osui_ui_anim_set_type(desktop_logic.mask_anim, OSUI_UI_ANIM_TYPE_OPA);
    // osui_ui_anim_set_pd(desktop_logic.mask_anim, DESKTOP_APP_ANIM_PDC_P, DESKTOP_APP_ANIM_PDC_D);
    // osui_ui_anim_set_tg(desktop_logic.mask_anim,DESKTOP_ANIM_MASK_HOME_OPA);
}

//UI桌面初始化
void osui_ui_desktop_init()
{
    desktop_ui_init();
    desktop_logic_init();
}

//桌面添加应用
void osui_ui_desktop_add_app(const char *name, lv_obj_t*app)
{
    add_app(name, app);
}

//桌面退出应用
void osui_ui_desktop_delaunch()
{
    app_delaunch();
}

//获取桌面宽度
int16_t osui_ui_desktop_get_width()
{
    return osui_disp_get()->hor;
}

//获取桌面高度
int16_t osui_ui_desktop_get_height()
{
    return osui_disp_get()->ver-DESKTOP_STATBAR_HEIGHT-DESKTOP_NAVBAR_HEIGHT;
}

//设置桌面时间
void osui_ui_desktop_set_time(const char* time)
{
    //设置锁屏
    lv_label_set_text(desktop_ui.time, time);
}

//设置桌面日期
void osui_ui_desktop_set_date(const char* date)
{
    //设置锁屏
    lv_label_set_text(desktop_ui.date, date);
}

//设置桌面字体颜色
void osui_ui_desktop_set_font_color(lv_color_t color)
{
    lv_obj_set_style_text_color(desktop_ui.time, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(desktop_ui.date, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    // 更新应用列表
    lv_obj_t * list = desktop_ui.app_list;
    for(uint16_t f = 0; f < lv_obj_get_child_count(list); ++f)
    {
        lv_obj_t * btn = lv_obj_get_child(list, f);
        for(uint16_t ff = 0; ff < lv_obj_get_child_count(btn); ++ff)
        {
            lv_obj_t * obj = lv_obj_get_child(btn, ff);
            if(lv_obj_check_type(obj, &lv_image_class))
            {
                lv_obj_set_style_text_color(obj, color, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
            if(lv_obj_check_type(obj, &lv_label_class))
            {
                lv_obj_set_style_text_color(obj, color, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        }
    }
}

//设置桌面背景颜色
void osui_ui_desktop_set_back_color(lv_color_t color)
{
    lv_obj_set_style_bg_color(desktop_ui.back, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    // 更新应用界面
    lv_obj_t * src = desktop_ui.app_scr;
    for(uint16_t f = 0; f < lv_obj_get_child_count(src); ++f)
    {
        lv_obj_t * bg = lv_obj_get_child(src, f);
        if(lv_obj_check_type(bg, &lv_obj_class))
        {
            lv_obj_set_style_bg_color(bg, color, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

//设置桌面壁纸
void osui_ui_desktop_set_wallpaper(char* path)
{
    //设置源
    lv_image_set_src(desktop_ui.back, path);
}
