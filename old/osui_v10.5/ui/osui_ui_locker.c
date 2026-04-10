#include"osui_ui_locker.h"

#include<math.h>
//UI框架
#include"osui/ui/osui_ui_frame.h"
//UI标签
#include"osui/ui/osui_ui_lable.h"
//UI动画
#include"osui/ui/osui_ui_anim.h"

//参数定义
#define LOCKER_ONLINE_POS (0)//在线位置
#define LOCKER_OFFLINE_POS (-osui_disp_get()->ver)//离线位置
#define LOCKER_ONLINE_OPA (LV_OPA_80)//在线透明度
#define LOCKER_OFFLINE_OPA (LV_OPA_0)//离线透明度
#define LOCKER_SAMPLE_TIME (LV_INDEV_STATE_PR*2)//锁屏采样时间
#define LOCKER_SLIDE_EXIT_RATIO (0.3)//锁屏上滑退出阈值比例
#define LOCKER_INFO_LAYOUT_LEFT_RATIO (0.1)//锁屏信息布局左间距比例
#define LOCKER_INFO_LAYOUT_LEFT_BOTTOM_RATIO (0.2)//锁屏信息布局左底间距比例
#define LOCKER_INFO_LAYOUT_RIGHT_RATIO (0.1)//锁屏信息布局右间距比例
#define LOCKER_INFO_LAYOUT_RIGHT_BOTTOM_RATIO (0.2)//锁屏信息布局右底间距比例
#define LOCKER_LOGIN_TIME (800)//登录动画时间
#define LOCKER_LOGOUT_TIME (800)//注销动画时间
#define LOCKER_FRONT_ONLINE_ANIM_PDC_P (0.3)//锁屏前景在线动画PD控制器P参数
#define LOCKER_FRONT_ONLINE_ANIM_PDC_D (0.1)//锁屏前景在线动画PD控制器D参数
#define LOCKER_ANIM_LV_PATH_P1 (1024)//锁屏动画LVGL轨迹参数1
#define LOCKER_ANIM_LV_PATH_P2 (1024)//锁屏动画LVGL轨迹参数2

//锁屏UI类
typedef struct locker_ui_t
{
    lv_obj_t *parent;
    lv_obj_t *mask;
    lv_obj_t *back;
    lv_obj_t *front;
    lv_obj_t *time;
    lv_obj_t *date;
    lv_obj_t *year;
    lv_obj_t *icon;
    lv_obj_t *resp;
} locker_ui_t;
static locker_ui_t locker_ui = {0};

//锁屏动画状态类
typedef enum locker_anim_status_t
{
    LOCKER_OFFLINE = 0,
    LOCKER_ONLINE = 1,
    LOCKER_LOGIN = 2,
    LOCKER_LOGOUT = 3,
}locker_anim_status_t;
//锁屏逻辑类
typedef struct locker_logic_t
{
    bool status;
    bool pressed;
    osui_dispinfo_t* dispinfo;
    osui_ui_anim_lv_t*mask_anim;
    osui_ui_anim_t* front_anim;
    osui_ui_anim_lv_t*back_anim;
    locker_anim_status_t anim_status;
} locker_logic_t;
static locker_logic_t locker_logic = {0};

//锁屏UI初始化
static void locker_ui_init();
//锁屏运行时
static void locker_runtime(lv_timer_t*timer);
//锁屏被按下
static void locker_press_cb(lv_event_t*e);
//锁屏被释放
static void locker_release_cb(lv_event_t*e);
//锁屏登录
static void locker_login();
//锁屏注销
static void locker_logout();
//登录动画完成回调
static void login_anim_complete_cb();
//注销动画完成回调
static void logout_anim_complete_cb();
//锁屏逻辑初始化
static void locker_logic_init();
//UI锁屏初始化
void osui_ui_locker_init();
//进入锁屏
void osui_ui_locker_enter();
//退出锁屏
void osui_ui_locker_exit();
//获取锁屏状态
bool osui_ui_locker_get_status();
//设置锁屏图标
void osui_ui_locker_set_icon(const char* icon);
//设置锁屏时间
void osui_ui_locker_set_time(const char* time);
//设置锁屏日期
void osui_ui_locker_set_date(const char* date);
//设置锁屏年份
void osui_ui_locker_set_year(const char* year);
//设置锁屏字体颜色
void osui_ui_locker_set_font_color(lv_color_t color);
//设置锁屏背景颜色
void osui_ui_locker_set_back_color(lv_color_t color);
//设置锁屏壁纸
void osui_ui_locker_set_wallpaper(char* path);

//锁屏UI初始化
static void locker_ui_init()
{
    //获取显示对象
    osui_dispinfo_t*osui_dispinfo=osui_disp_get();
    //获取UI框架对象
    lv_obj_t *obj = lv_obj_create(*osui_uiframe_get());
    //初始化锁屏父对象
    locker_ui.parent = obj;
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
    //绘制锁屏样式
    lv_obj_t *parent_obj = obj;
    {
        //遮罩
        lv_obj_t *obj = lv_obj_create(parent_obj);
        locker_ui.mask = obj;
        lv_obj_set_pos(obj, 0, 0);
        lv_obj_set_size(obj, osui_dispinfo->hor, osui_dispinfo->ver);
        lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    {
        //背景
        lv_obj_t *obj = lv_image_create(parent_obj);
        locker_ui.back = obj;
        lv_obj_set_pos(obj, 0, 0);
        lv_obj_set_size(obj, osui_dispinfo->hor, osui_dispinfo->ver);
        // lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
        lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_LIGHT_BLUE), LV_PART_MAIN | LV_STATE_DEFAULT);
        {
            lv_obj_t *parent_obj = obj;
            {
                //前景
                lv_obj_t *obj = lv_obj_create(parent_obj);
                locker_ui.front = obj;
                lv_obj_set_pos(obj, 0, 0);
                lv_obj_set_size(obj, osui_dispinfo->hor, osui_dispinfo->ver);
                lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                {
                    lv_obj_t *parent_obj = obj;
                    {
                        //计算布局
                        uint16_t layout_hor=(int16_t)(osui_dispinfo->hor*LOCKER_INFO_LAYOUT_LEFT_RATIO);
                        uint16_t layout_ver=(int16_t)(osui_dispinfo->ver*(1-LOCKER_INFO_LAYOUT_LEFT_BOTTOM_RATIO));
                        {
                            uint16_t sum_ver=osui_ui_lable_get_ver(&OSUI_UI_LABLE_FONT_MAX)+
                                             osui_ui_lable_get_ver(&OSUI_UI_LABLE_FONT_MID)*2;
                            layout_ver-=sum_ver;
        
                        }
                        {
                            //锁屏时间
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            locker_ui.time = obj;
                            lv_obj_set_pos(obj, layout_hor, layout_ver);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &OSUI_UI_LABLE_FONT_MAX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "9:30");
                            //更新布局
                            layout_ver+=osui_ui_lable_get_ver(&OSUI_UI_LABLE_FONT_MAX);
                        }
                        {
                            //锁屏日期
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            locker_ui.date = obj;
                            lv_obj_set_pos(obj, layout_hor, layout_ver);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &OSUI_UI_LABLE_FONT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "January 1");
                            //更新布局
                            layout_ver+=osui_ui_lable_get_ver(&OSUI_UI_LABLE_FONT_MID);
                        }
                        {
                            //锁屏年份
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            locker_ui.year = obj;
                            lv_obj_set_pos(obj, layout_hor, layout_ver);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &OSUI_UI_LABLE_FONT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "2026");
                        }
                    }
                    {
                        //计算布局
                        uint16_t layout_hor=(int16_t)(osui_dispinfo->hor*(1-LOCKER_INFO_LAYOUT_RIGHT_RATIO));
                        uint16_t layout_ver=(int16_t)(osui_dispinfo->ver*(1-LOCKER_INFO_LAYOUT_RIGHT_BOTTOM_RATIO));
                        {
                            uint16_t sum_ver=osui_ui_lable_get_ver(&OSUI_UI_LABLE_FONT_MIN);
                            layout_ver-=sum_ver;
                            uint16_t sum_hor=osui_ui_lable_get_hor(LV_SYMBOL_WIFI " " LV_SYMBOL_BLUETOOTH " " LV_SYMBOL_BATTERY_FULL,&OSUI_UI_LABLE_FONT_MIN);
                            layout_hor-=sum_hor;
                        }
                        {
                            //锁屏图标
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            locker_ui.icon = obj;
                            lv_obj_set_pos(obj, layout_hor, layout_ver);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &OSUI_UI_LABLE_FONT_MIN, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, LV_SYMBOL_WIFI " " LV_SYMBOL_BLUETOOTH " " LV_SYMBOL_BATTERY_FULL);
                        }
                    }
                }
            }
        
        }
    }
    {
        //响应
        lv_obj_t *obj = lv_obj_create(parent_obj);
        locker_ui.resp = obj;
        lv_obj_set_pos(obj, 0, 0);
        lv_obj_set_size(obj, osui_dispinfo->hor, osui_dispinfo->ver);
        lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

//锁屏运行时
static void locker_runtime(lv_timer_t*timer)
{
    //运行时检查
    if(locker_logic.status)
    {
        //处理动画
        switch (locker_logic.anim_status)
        {
            //离线
            case LOCKER_OFFLINE:
            break;
            //在线
            case LOCKER_ONLINE:
                //活动
                if(locker_logic.pressed)
                {
                    //前景动画目标调整
                    osui_ui_anim_set_tg(locker_logic.front_anim,osui_input_get_move_y());
                }else
                //空闲
                {
                    //前景动画目标调整
                    osui_ui_anim_set_tg(locker_logic.front_anim,LOCKER_ONLINE_POS);
                }
            break;
            //登录
            case LOCKER_LOGIN:
                //前景动画目标调整
                osui_ui_anim_set_tg(locker_logic.front_anim,LOCKER_ONLINE_POS);
            break;
            //注销
            case LOCKER_LOGOUT:
                //前景动画目标调整
                osui_ui_anim_set_tg(locker_logic.front_anim,LOCKER_OFFLINE_POS);
            break;
            default:
            break;
        }
        //前景动画
        if(!osui_ui_anim_finish(locker_logic.front_anim))
        {
            osui_ui_anim_exec(locker_logic.front_anim);
        }
    }
}

//锁屏按下回调
static void locker_press_cb(lv_event_t*e)
{
    locker_logic.pressed=true;
}

//锁屏释放回调
static void locker_release_cb(lv_event_t*e)
{
    locker_logic.pressed=false;
    //注销挂起检查
    if(locker_logic.anim_status==LOCKER_ONLINE)
    {
        int16_t y = osui_input_get_move_y();
        if(y<-locker_logic.dispinfo->ver*LOCKER_SLIDE_EXIT_RATIO)
        {
            locker_logout();
        }
    }
}

//锁屏登录
static void locker_login()
{
    //状态更新
    locker_logic.anim_status=LOCKER_LOGIN;
    //遮罩动画
    osui_ui_anim_lv_set_param(locker_logic.mask_anim, LOCKER_OFFLINE_OPA, LOCKER_ONLINE_OPA);
    osui_ui_anim_lv_set_time(locker_logic.mask_anim, LOCKER_LOGIN_TIME);
    osui_ui_anim_lv_set_exec_cb(locker_logic.mask_anim, osui_ui_anim_lv_exec_cb_bg_opa);
    // osui_ui_anim_lv_set_finish_cb(locker_logic.mask_anim, logout_anim_complete_cb);
    osui_ui_anim_lv_set_bezier_param(locker_logic.mask_anim, LOCKER_ANIM_LV_PATH_P1, LOCKER_ANIM_LV_PATH_P2);
    osui_ui_anim_lv_start(locker_logic.mask_anim);
    //背景动画
    osui_ui_anim_lv_set_param(locker_logic.back_anim, LOCKER_OFFLINE_POS, LOCKER_ONLINE_POS);
    osui_ui_anim_lv_set_time(locker_logic.back_anim, LOCKER_LOGIN_TIME);
    osui_ui_anim_lv_set_exec_cb(locker_logic.back_anim, osui_ui_anim_lv_exec_cb_y);
    osui_ui_anim_lv_set_finish_cb(locker_logic.back_anim, login_anim_complete_cb);
    osui_ui_anim_lv_set_bezier_param(locker_logic.back_anim, LOCKER_ANIM_LV_PATH_P1, LOCKER_ANIM_LV_PATH_P2);
    osui_ui_anim_lv_start(locker_logic.back_anim);
    //打开锁屏运行时
    locker_logic.status = true;
    //显示锁屏
    lv_obj_remove_flag(locker_ui.parent, LV_OBJ_FLAG_HIDDEN);
    osui_log("login locker");
}

//锁屏注销
static void locker_logout()
{
    //状态更新
    locker_logic.anim_status=LOCKER_LOGOUT;
    //遮罩动画
    osui_ui_anim_lv_set_param(locker_logic.mask_anim, LOCKER_ONLINE_OPA, LOCKER_OFFLINE_OPA);
    osui_ui_anim_lv_set_time(locker_logic.mask_anim, LOCKER_LOGOUT_TIME);
    osui_ui_anim_lv_set_exec_cb(locker_logic.mask_anim, osui_ui_anim_lv_exec_cb_bg_opa);
    // osui_ui_anim_lv_set_finish_cb(locker_logic.mask_anim, logout_anim_complete_cb);
    osui_ui_anim_lv_set_bezier_param(locker_logic.mask_anim, LOCKER_ANIM_LV_PATH_P1, LOCKER_ANIM_LV_PATH_P2);
    osui_ui_anim_lv_start(locker_logic.mask_anim);
    //背景动画
    osui_ui_anim_lv_set_param(locker_logic.back_anim, LOCKER_ONLINE_POS, LOCKER_OFFLINE_POS);
    osui_ui_anim_lv_set_time(locker_logic.back_anim, LOCKER_LOGOUT_TIME);
    osui_ui_anim_lv_set_exec_cb(locker_logic.back_anim, osui_ui_anim_lv_exec_cb_y);
    osui_ui_anim_lv_set_finish_cb(locker_logic.back_anim, logout_anim_complete_cb);
    osui_ui_anim_lv_set_bezier_param(locker_logic.back_anim, LOCKER_ANIM_LV_PATH_P1, LOCKER_ANIM_LV_PATH_P2);
    osui_ui_anim_lv_start(locker_logic.back_anim);
    osui_log("logout locker");
}

//注销动画完成回调
static void logout_anim_complete_cb()
{
    //更新状态
    locker_logic.anim_status=LOCKER_OFFLINE;
    //关闭锁屏运行时
    locker_logic.status = false;
    //隐藏锁屏
    lv_obj_add_flag(locker_ui.parent, LV_OBJ_FLAG_HIDDEN);
    osui_log("exit locker");
}

//登录动画完成回调
static void login_anim_complete_cb()
{
    //更新状态
    locker_logic.anim_status=LOCKER_ONLINE;
    osui_log("enter locker");
}

//锁屏逻辑初始化
static void locker_logic_init()
{
    //获取显示参数对象
    locker_logic.dispinfo = osui_disp_get();
    //活动状态检查
    lv_obj_add_event_cb(locker_ui.resp, locker_press_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(locker_ui.resp, locker_release_cb, LV_EVENT_RELEASED, NULL);
    //创建运行时
    lv_timer_create(locker_runtime,LOCKER_SAMPLE_TIME,NULL);
    //遮罩动画
    locker_logic.mask_anim = osui_ui_anim_lv_init();
    osui_ui_anim_lv_set_obj(locker_logic.mask_anim, locker_ui.mask);
    //前景动画
    // locker_logic.front_anim = osui_ui_anim_lv_init();
    // osui_ui_anim_lv_set_obj(locker_logic.front_anim, locker_ui.front);
    locker_logic.front_anim = osui_ui_anim_init();
    osui_ui_anim_set_obj(locker_logic.front_anim, locker_ui.front);
    osui_ui_anim_set_type(locker_logic.front_anim, OSUI_UI_ANIM_TYPE_Y);
    osui_ui_anim_set_pd(locker_logic.front_anim, LOCKER_FRONT_ONLINE_ANIM_PDC_P, LOCKER_FRONT_ONLINE_ANIM_PDC_D);
    osui_ui_anim_set_tg(locker_logic.front_anim, LOCKER_ONLINE_POS);
    //背景动画
    locker_logic.back_anim = osui_ui_anim_lv_init();
    osui_ui_anim_lv_set_obj(locker_logic.back_anim, locker_ui.back);
}

//UI锁屏初始化
void osui_ui_locker_init()
{
    locker_ui_init();
    locker_logic_init();
}

//进入锁屏
void osui_ui_locker_enter()
{
    //离线状态检查
    if(locker_logic.anim_status==LOCKER_OFFLINE)
    {
        //登录
        locker_login();
    }
}

//退出锁屏
void osui_ui_locker_exit()
{
    //离线状态检查
    if(locker_logic.anim_status==LOCKER_ONLINE)
    {
        //注销
        locker_logout();
    }
}

//获取锁屏状态
bool osui_ui_locker_get_status()
{
    return locker_logic.status;
}

//设置锁屏图标
void osui_ui_locker_set_icon(const char* icon)
{
    //计算布局
    uint16_t layout_hor=(int16_t)(osui_disp_get()->hor*(1-LOCKER_INFO_LAYOUT_RIGHT_RATIO))-osui_ui_lable_get_hor(icon,&OSUI_UI_LABLE_FONT_MIN);
    uint16_t layout_ver=(int16_t)(osui_disp_get()->ver*(1-LOCKER_INFO_LAYOUT_RIGHT_BOTTOM_RATIO))-osui_ui_lable_get_ver(&OSUI_UI_LABLE_FONT_MIN);
    //设置锁屏
    lv_obj_set_pos(locker_ui.icon, layout_hor, layout_ver);
    lv_label_set_text(locker_ui.icon, icon);
}

//设置锁屏时间
void osui_ui_locker_set_time(const char* time)
{
    //设置锁屏
    lv_label_set_text(locker_ui.time, time);
}

//设置锁屏日期
void osui_ui_locker_set_date(const char* date)
{
    //设置锁屏
    lv_label_set_text(locker_ui.date, date);
}

//设置锁屏年份
void osui_ui_locker_set_year(const char* year)
{
    //设置锁屏
    lv_label_set_text(locker_ui.year, year);
}

//设置锁屏字体颜色
void osui_ui_locker_set_font_color(lv_color_t color)
{
    lv_obj_set_style_text_color(locker_ui.time, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(locker_ui.date, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(locker_ui.year, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(locker_ui.icon, color, LV_PART_MAIN | LV_STATE_DEFAULT);
}

//设置锁屏背景颜色
void osui_ui_locker_set_back_color(lv_color_t color)
{
    lv_obj_set_style_bg_color(locker_ui.back, color, LV_PART_MAIN | LV_STATE_DEFAULT);
}

//设置锁屏壁纸
void osui_ui_locker_set_wallpaper(char* path)
{
    //设置源
    lv_image_set_src(locker_ui.back, path);
}
