#include"osui_disp.h"

//数学
#include<math.h>

//显示参数
static osui_dispinfo_t osui_dispinfo=
{
    .hor=320,
    .ver=480,
    .size=4.8,
    .dpi=120.0
};

//显示初始化
void osui_disp_init(uint16_t hor,uint16_t ver,float size);
//像素转像素无关
float osui_disp_px2dp(float px);
//像素无关转像素
float osui_disp_dp2px(float dp);
//显示获取
osui_dispinfo_t*osui_disp_get();

//显示初始化
void osui_disp_init(uint16_t hor,uint16_t ver,float size)
{
    osui_dispinfo.hor=hor;
    osui_dispinfo.ver=ver;
    osui_dispinfo.size=size;
    if(size)
    {
        osui_dispinfo.dpi=sqrt(pow(hor,2)+pow(ver,2))/size;
    }else
    {
        osui_dispinfo.dpi=OSUI_STAND_DPI;
    }
}

//像素转像素无关
float osui_disp_px2dp(float px)
{
    return px*osui_dispinfo.dpi/OSUI_STAND_DPI;
}

//像素无关转像素
float osui_disp_dp2px(float dp)
{
    return dp*OSUI_STAND_DPI/osui_dispinfo.dpi;
}

//显示获取
osui_dispinfo_t*osui_disp_get()
{
    return &osui_dispinfo;
}
