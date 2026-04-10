#include"osui_ui_lable.h"

// //UI标签初始化
// void osui_ui_lable_init()
// {
    
// }

//获取标签高度
uint16_t osui_ui_lable_get_ver(const lv_font_t *font);
//获取标签宽度
uint16_t osui_ui_lable_get_hor(const char *text, const lv_font_t *font);
//获取标签尺寸
void osui_ui_lable_get_size(const char *text, const lv_font_t *font, uint16_t *hor, uint16_t *ver);

//获取标签高度
uint16_t osui_ui_lable_get_ver(const lv_font_t *font)
{
    lv_point_t size;
    lv_text_get_size(&size,//大小
                   "A",//文本
                   font,//字体
                   0,//字间距
                   0,//行间距
                   LV_COORD_MAX,//不限制宽度
                   LV_TEXT_FLAG_NONE//无特殊标记
                );
    return size.y;
}

//获取标签宽度
uint16_t osui_ui_lable_get_hor(const char *text, const lv_font_t *font)
{
    lv_point_t size;
    lv_text_get_size(&size,//大小
                   text, //文本
                   font,//字体
                   0,//字间距
                   0,//行间距
                   LV_COORD_MAX,//不限制宽度
                   LV_TEXT_FLAG_NONE//无特殊标记
                );
    return size.x;
}

//获取标签尺寸
void osui_ui_lable_get_size(const char *text, const lv_font_t *font, uint16_t *hor, uint16_t *ver)
{
    lv_point_t size;
    lv_text_get_size(&size,//大小
                   text, //文本
                   font,//字体
                   0,//字间距
                   0,//行间距
                   LV_COORD_MAX,//不限制宽度
                   LV_TEXT_FLAG_NONE//无特殊标记
                );
    if(hor)
    {
        *hor = size.x;
    }
    if(ver)
    {
        *ver = size.y;
    }
    osui_log("label size: hor=%d,ver=%d\r\n",size.x,size.y);
}
