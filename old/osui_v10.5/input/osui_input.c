#include"osui_input.h"

//输入参数类
typedef struct input_info_t
{
    lv_indev_read_cb_t org_cb;
    bool status;
    uint32_t start_time;
    uint32_t end_time;
    uint16_t start_x;
    uint16_t start_y;
    int16_t move_x;
    int16_t move_y;
    uint16_t last_x;
    uint16_t last_y;
    uint16_t end_x;
    uint16_t end_y;
    bool rtspeed_enable;
    float rtspeed_x;
    float rtspeed_y;
}input_info_t;
static input_info_t input_info={0};

//注入输入回调
static void inject_input();
//注入后输入回调
static void injected_input_cb(lv_indev_t* drv, lv_indev_data_t* data);
//输入初始化
void osui_input_init();
//是否启用实时速度纪录
void osui_input_enable_rtspeed(bool enable);
//获取输入状态
bool osui_input_get_status();
//获取输入坐标
void osui_input_get_point(uint16_t* x,uint16_t* y);
//单独获取X坐标
uint16_t osui_input_get_x(void);
//单独获取Y坐标
uint16_t osui_input_get_y(void);
//获取输入移动距离
void osui_input_get_move(int16_t* x,int16_t* y);
//单独获取X移动距离
int16_t osui_input_get_move_x(void);
//单独获取Y移动距离
int16_t osui_input_get_move_y(void);
//获取实时输入速度
void osui_input_get_rtspeed(float* x,float* y);
//单独获取X速度
float osui_input_get_rtspeed_x(void);
//单独获取Y速度
float osui_input_get_rtspeed_y(void);
//获取像素无关实时输入速度
void osui_input_get_rtspeed_dp(float* x,float* y);
//单独获取像素无关X速度
float osui_input_get_rtspeed_dp_x(void);
//单独获取像素无关Y速度
float osui_input_get_rtspeed_dp_y(void);

//注入输入回调
static void inject_input()
{   
    //获取输入设备
    lv_indev_t* indev = lv_indev_get_next(NULL);
    //劫持原始输入回调
    input_info.org_cb = lv_indev_get_read_cb(indev);
    //注入输入回调
    lv_indev_set_read_cb(indev,injected_input_cb);
}

//注入后输入回调
static void injected_input_cb(lv_indev_t* drv, lv_indev_data_t* data)
{
    //劫持原始输入回调
    if(input_info.org_cb)
    {
        input_info.org_cb(drv, data);
    }
    //更新输入参数
    lv_indev_state_t status=data->state;
    //按下
    if(status==LV_INDEV_STATE_PR)
    {
        lv_point_t point = data->point;
        //首次按下
        if(input_info.status==false)
        {          
            //记录开始时间
            input_info.start_time=lv_tick_get();
            //更新状态
            input_info.status=true;
            //记录起始坐标
            input_info.start_x=point.x;
            input_info.start_y=point.y;
        }
        //更新坐标
        input_info.last_x=input_info.end_x;
        input_info.last_y=input_info.end_y;
        input_info.end_x=point.x;
        input_info.end_y=point.y;
        //更新移动距离
        input_info.move_x=input_info.end_x-input_info.start_x;
        input_info.move_y=input_info.end_y-input_info.start_y;
        //计算速度
        if(input_info.rtspeed_enable)
        {
            uint32_t time=lv_tick_get()-input_info.start_time;
            if(time>0)
            {
                if(input_info.move_x)
                {
                    input_info.rtspeed_x=(float)(input_info.end_x-input_info.last_x)/time;
                }
                if(input_info.move_y)
                {
                    input_info.rtspeed_y=(float)(input_info.end_y-input_info.last_y)/time;
                }
            }   
        }
    }else
    //释放
    if(status==LV_INDEV_STATE_REL)
    {
        //首次释放
        if(input_info.status==true)
        {
            //记录结束时间
            input_info.end_time=lv_tick_get();
            // //清空数据
            // input_info.move_x=0;
            // input_info.move_y=0;
            // input_info.rtspeed_x=0;
            // input_info.rtspeed_y=0;
            //更新状态
            input_info.status=false;
        }
    }
}

//输入初始化
void osui_input_init()
{
    //注入输入回调
    inject_input();   
}

//是否启用实时速度纪录
void osui_input_enable_rtspeed(bool enable)
{
    input_info.rtspeed_enable=enable;
}

//获取输入状态
bool osui_input_get_status()
{
    return input_info.status;
}

//获取输入坐标
void osui_input_get_point(uint16_t* x,uint16_t* y)
{
    if(x)
    {
        *x=input_info.end_x;
    }
    if(y)
    {
        *y=input_info.end_y;
    }
}

//获取输入移动距离
void osui_input_get_move(int16_t* x,int16_t* y)
{
    if(x)
    {
        *x=input_info.move_x;
    }
    if(y)
    {
        *y=input_info.move_y;
    }
}

//获取实时输入速度
void osui_input_get_rtspeed(float* x,float* y)
{
    if(x)
    {
        *x=input_info.rtspeed_x;
    }
    if(y)
    {
        *y=input_info.rtspeed_y;
    }
}

//获取像素无关实时输入速度
void osui_input_get_rtspeed_dp(float* x,float* y)
{
    if(x)
    {
        *x=osui_disp_px2dp(input_info.rtspeed_x);
    }
    if(y)
    {
        *y=osui_disp_px2dp(input_info.rtspeed_y);
    }
}

//单独获取X坐标
uint16_t osui_input_get_x(void)
{
    return input_info.end_x;
}

//单独获取Y坐标
uint16_t osui_input_get_y(void)
{
    return input_info.end_y;
}

//单独获取X移动距离
int16_t osui_input_get_move_x(void)
{
    return input_info.move_x;
}

//单独获取Y移动距离
int16_t osui_input_get_move_y(void)
{
    return input_info.move_y;
}

//单独获取X速度
float osui_input_get_rtspeed_x(void)
{
    return input_info.rtspeed_x;
}

//单独获取Y速度
float osui_input_get_rtspeed_y(void)
{
    return input_info.rtspeed_y;
}

//单独获取像素无关X速度
float osui_input_get_rtspeed_dp_x(void)
{
    return osui_disp_px2dp(input_info.rtspeed_x);
}

//单独获取像素无关Y速度
float osui_input_get_rtspeed_dp_y(void)
{
    return osui_disp_px2dp(input_info.rtspeed_y);
}
