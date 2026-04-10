#include"osui_ui_navbar.h"

//UI框架
#include"osui/ui/osui_ui_frame.h"
//UI动画
#include"osui/ui/osui_ui_anim.h"

//参数定义
#define NAVBAR_HEIGHT ((int16_t)(osui_disp_dp2px(NAVBAR_HEIGHT_DP)))//导航条高度
#define NAVBAR_REAL_POS ((int16_t)((NAVBAR_HEIGHT-osui_disp_dp2px(NAVBAR_THICK_DP))/2))//导航条相对位置
#define NAVBAR_ORG_POS (0)//导航条原始位置
#define NAVBAR_OFFSET_POS ((int16_t)(osui_disp_get()->ver-NAVBAR_HEIGHT))//导航条偏移位置
#define NAVBAR_OPA (LV_OPA_0)//导航条透明度
#define NAVBAR_SLIDE_EXIT_RATIO (0.1)//导航条上滑退出阈值比例
#define NAVBAR_HEIGHT_DP (16)//导航条DP高度
#define NAVBAR_THICK_DP (6)//导航条DP厚度
#define NAVBAR_PULL_RATIO (0.25)//导航条上拉比例
#define NAVBAR_SAMPLE_TIME (LV_INDEV_STATE_PR*2)//导航条采样时间
#define NAVBAR_ANIM_PDC_P (0.4)//导航条动画PD控制器P参数
#define NAVBAR_ANIM_PDC_D (0.1)//导航条动画PD控制器D参数

//导航条UI类
typedef struct navbar_ui_t
{
    // lv_obj_t * root;
    lv_obj_t * parent;
    lv_obj_t * back;
    lv_obj_t * front;
    lv_obj_t * bar;
} navbar_ui_t;
static navbar_ui_t navbar_ui = {0};

//导航条动画状态类
typedef enum navbar_anim_status_t
{
    NAVBAR_IDLE = 0,
    NAVBAR_BUSY = 1,
    NAVBAR_WAIT = 2,
}navbar_anim_status_t;
//导航条逻辑类
typedef struct navbar_logic_t
{
    osui_ui_anim_t* bar_anim;
    navbar_anim_status_t anim_status;
    bool wait_flag;
} navbar_logic_t;
static navbar_logic_t navbar_logic = {0};

//导航条UI初始化
static void navbar_ui_init();
//导航条被按下
static void navbar_pressed(lv_event_t*e);
//导航条被释放
void osui_ui_desktop_delaunch();
static void navbar_released(lv_event_t*e);
//导航条逻辑初始化
static void navbar_logic_init();
//UI导航条初始化
void osui_ui_navbar_init();
//获取导航条高度
uint16_t osui_ui_navbar_get_height();
//导航条运行时
bool osui_ui_locker_get_status();
static void navbar_runtime(lv_timer_t*timer);
//设置导航条颜色
void osui_ui_navbar_set_color(lv_color_t color);

//导航条UI初始化
static void navbar_ui_init()
{
    //获取显示对象
    osui_dispinfo_t*osui_dispinfo=osui_disp_get();
    //获取UI框架对象
    lv_obj_t *obj = lv_obj_create(*osui_uiframe_get());
    //初始化导航条父对象
    navbar_ui.parent = obj;
    lv_obj_set_pos(obj, 0, osui_dispinfo->ver-NAVBAR_HEIGHT);
    lv_obj_set_size(obj, osui_dispinfo->hor, NAVBAR_HEIGHT);
    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(obj, 127, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    //绘制导航条样式
    lv_obj_t *parent_obj = obj;
    {
        //背景
        lv_obj_t *obj = lv_obj_create(parent_obj);
        navbar_ui.back=obj;
        lv_obj_set_pos(obj, 0, 0);
        lv_obj_set_size(obj, osui_dispinfo->hor, NAVBAR_HEIGHT);
        lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(obj, NAVBAR_OPA, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    }
    {
        //前景
        lv_obj_t *obj = lv_obj_create(parent_obj);
        navbar_ui.front=obj;
        lv_obj_set_pos(obj, 0, 0);
        lv_obj_set_size(obj, osui_dispinfo->hor, NAVBAR_HEIGHT);
        lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_bg_opa(obj, 127, LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        {
            lv_obj_t *parent_obj = obj;
            {
                //导航条
                lv_obj_t *obj = lv_button_create(parent_obj);
                navbar_ui.bar=obj;
                lv_obj_set_pos(obj, (int16_t)(osui_dispinfo->hor/4.0), NAVBAR_REAL_POS);
                lv_obj_set_size(obj, (int16_t)(osui_dispinfo->hor/2.0), (int16_t)osui_disp_dp2px(NAVBAR_THICK_DP));
                lv_obj_set_style_transform_height(obj, 0, LV_PART_MAIN | LV_STATE_PRESSED);
                lv_obj_set_style_transform_width(obj, 0, LV_PART_MAIN | LV_STATE_PRESSED);
                lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_shadow_offset_y(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(navbar_ui.bar, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        }
    }
}

//导航条被按下
static void navbar_pressed(lv_event_t*e)
{
    //状态更新
    navbar_logic.anim_status=NAVBAR_BUSY;
    //调整导航条状态
    // osui_log("state=%d",lv_obj_get_state(navbar_ui.bar));
    // lv_state_t
    lv_obj_set_state(navbar_ui.bar, LV_STATE_PRESSED, true);
    //调整父控件位置
    lv_obj_set_pos(navbar_ui.parent, 0, 0);
    //调整父控件大小
    lv_obj_set_size(navbar_ui.parent, osui_disp_get()->hor, osui_disp_get()->ver);
    //调整背景位置
    lv_obj_set_pos(navbar_ui.back, 0, NAVBAR_OFFSET_POS);
    //调整导航条位置
    // lv_obj_set_pos(navbar_ui.front, (int16_t)(lv_obj_get_width(navbar_ui.parent)/4.0), NAVBAR_OFFSET_POS+NAVBAR_REAL_POS);
    lv_obj_set_pos(navbar_ui.front, 0, NAVBAR_OFFSET_POS);
    //动画目标调整
    // osui_ui_anim_set_tg(navbar_logic.bar_anim, NAVBAR_OFFSET_POS+NAVBAR_REAL_POS);
    osui_ui_anim_set_tg(navbar_logic.bar_anim, NAVBAR_OFFSET_POS);
    osui_log("press");
}

//导航条被释放
static void navbar_released(lv_event_t*e)
{
    //状态更新
    navbar_logic.anim_status=NAVBAR_WAIT;
    //动画目标调整
    // osui_ui_anim_set_tg(navbar_logic.bar_anim, NAVBAR_OFFSET_POS+NAVBAR_REAL_POS);
    osui_ui_anim_set_tg(navbar_logic.bar_anim, NAVBAR_OFFSET_POS);
    //调整导航条状态
    lv_obj_set_state(navbar_ui.bar, LV_STATE_PRESSED, false);
    // lv_obj_set_state(navbar_ui.bar, LV_STATE_DEFAULT, true);
    //桌面退出应用
    if(-osui_input_get_move_y()>(int16_t)(NAVBAR_SLIDE_EXIT_RATIO*osui_disp_get()->ver))
    {
        osui_ui_desktop_delaunch();
    }
    osui_log("relea");

}

//导航条逻辑初始化
static void navbar_logic_init()
{
    //创建运行时
    lv_timer_create(navbar_runtime,NAVBAR_SAMPLE_TIME,NULL);
    //活动状态检查
    lv_obj_add_event_cb(navbar_ui.front, navbar_pressed, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(navbar_ui.bar, navbar_pressed, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(navbar_ui.front, navbar_released, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(navbar_ui.bar, navbar_released, LV_EVENT_RELEASED, NULL);
    //动画
    navbar_logic.bar_anim = osui_ui_anim_init();
    osui_ui_anim_set_obj(navbar_logic.bar_anim, navbar_ui.front);
    osui_ui_anim_set_type(navbar_logic.bar_anim, OSUI_UI_ANIM_TYPE_Y);
    osui_ui_anim_set_pd(navbar_logic.bar_anim, NAVBAR_ANIM_PDC_P, NAVBAR_ANIM_PDC_D);
    // osui_ui_anim_set_tg(navbar_logic.bar_anim,NAVBAR_REAL_POS);
    osui_ui_anim_set_tg(navbar_logic.bar_anim,NAVBAR_ORG_POS);
}

//UI导航条初始化
void osui_ui_navbar_init()
{
    navbar_ui_init();
    navbar_logic_init();
    // return ret;
}

//获取导航条高度
uint16_t osui_ui_navbar_get_height()
{
    return (uint16_t)NAVBAR_HEIGHT;
}

//导航条运行时
static void navbar_runtime(lv_timer_t*timer)
{
    //运行时检查
    if(!osui_ui_locker_get_status())
    {
        switch (navbar_logic.anim_status)
        {
            //空闲
            case NAVBAR_IDLE:
            break;
            //正忙
            case NAVBAR_BUSY:
                //动画目标调整
                // osui_ui_anim_set_tg(navbar_logic.bar_anim, NAVBAR_OFFSET_POS+NAVBAR_REAL_POS+(int16_t)(osui_input_get_move_y()*NAVBAR_PULL_RATIO));
                osui_ui_anim_set_tg(navbar_logic.bar_anim, NAVBAR_OFFSET_POS+(int16_t)(osui_input_get_move_y()*NAVBAR_PULL_RATIO));
            break;
            //等待
            case NAVBAR_WAIT:
                //等待空闲
                if(osui_ui_anim_finish(navbar_logic.bar_anim))
                {
                    //状态更新
                    navbar_logic.anim_status=NAVBAR_IDLE;
                    //调整父控件位置
                    lv_obj_set_pos(navbar_ui.parent, 0, NAVBAR_OFFSET_POS);
                    //调整父控件大小
                    lv_obj_set_size(navbar_ui.parent, osui_disp_get()->hor, NAVBAR_HEIGHT);
                    //调整背景位置
                    lv_obj_set_pos(navbar_ui.back, 0, NAVBAR_ORG_POS);
                    //调整导航条位置
                    // lv_obj_set_pos(navbar_ui.front, (int16_t)(lv_obj_get_width(navbar_ui.parent)/4.0), NAVBAR_REAL_POS); 
                    lv_obj_set_pos(navbar_ui.front, 0, NAVBAR_ORG_POS);
                    // osui_log("set=%d",NAVBAR_REAL_POS);
                    // osui_log("OVER_STAT:%d,TG=%d,CURR=%d",navbar_logic.anim_status,navbar_logic.bar_anim->tg,lv_obj_get_y(navbar_ui.front));
                    //动画目标调整
                    // osui_ui_anim_set_tg(navbar_logic.bar_anim, NAVBAR_REAL_POS);
                    osui_ui_anim_set_tg(navbar_logic.bar_anim, NAVBAR_ORG_POS);
                    //动画等待
                    if(!osui_ui_anim_finish(navbar_logic.bar_anim))
                    {
                        //取消状态更新
                        navbar_logic.anim_status=NAVBAR_WAIT;
                        //等待动画就绪
                        navbar_logic.wait_flag=1;
                    }else
                    {
                        navbar_logic.wait_flag=0;
                    }
                    // osui_log("wait over");
                }
            break;
            default:
            break;
        }
        //动画
        if(!osui_ui_anim_finish(navbar_logic.bar_anim))
        {
            if(navbar_logic.wait_flag)
            {
                return;
            }
            osui_ui_anim_exec(navbar_logic.bar_anim);
        }
    }
}

//设置导航条颜色
void osui_ui_navbar_set_color(lv_color_t color)
{
    lv_obj_set_style_bg_color(navbar_ui.bar, color, LV_PART_MAIN | LV_STATE_DEFAULT);
}
