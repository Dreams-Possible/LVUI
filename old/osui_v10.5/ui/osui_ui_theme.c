#include"osui_ui_theme.h"

//UI动画
#include"osui/ui/osui_ui_anim.h"

// //主题动画状态类
// typedef enum theme_anim_status_t
// {
//     NAVBAR_IDLE = 0,
//     NAVBAR_BUSY = 1,
//     // NAVBAR_WAIT = 2,
// }theme_anim_status_t;
//主题逻辑类
typedef struct theme_logic_t
{
    // osui_ui_anim_t* anim;
    // theme_anim_status_t anim_status;
    // bool wait_flag;
    lv_color_t color;
    // lv_color_t curr_color;
} theme_logic_t;
static theme_logic_t theme_logic = {0};

//主题颜色取反
lv_color_t osui_theme_color_invert(lv_color_t color);
//主题颜色智能取反
lv_color_t osui_theme_smart_invert(lv_color_t color);
//设置控件颜色
void osui_ui_locker_set_font_color(lv_color_t color);
void osui_ui_locker_set_back_color(lv_color_t color);
void osui_ui_desktop_set_font_color(lv_color_t color);
void osui_ui_desktop_set_back_color(lv_color_t color);
void osui_ui_statbar_set_font_color(lv_color_t color);
void osui_ui_navbar_set_color(lv_color_t color);
static void set_obj_color(lv_color_t color);
//UI主题初始化
void osui_ui_theme_init();
//UI设置主题色
void osui_ui_theme_set_color(lv_color_t color);
//UI获取主题色
lv_color_t osui_ui_theme_get_color();

//主题颜色取反
lv_color_t osui_theme_color_invert(lv_color_t color)
{
    return lv_color_make(
        255 - color.red,
        255 - color.green,
        255 - color.blue
    );
}

//主题颜色智能取反
lv_color_t osui_theme_smart_invert(lv_color_t color)
{
    uint8_t brightness = lv_color_brightness(color);

    if (brightness > 128)
    {
        // 亮色 -> 返回深色
        return lv_color_darken(color, 200);
    } else
    {
        // 暗色 -> 返回亮色
        return lv_color_lighten(color, 200);
    }
}

//设置控件颜色
// void osui_ui_theme_set_color(lv_color_t color)
static void set_obj_color(lv_color_t color)
{
    lv_color_t inv_color=osui_theme_smart_invert(color);
    // lv_color_t inv_color=osui_theme_color_invert(color);
    osui_ui_locker_set_font_color(inv_color);
    osui_ui_locker_set_back_color(color);
    osui_ui_desktop_set_font_color(inv_color);
    osui_ui_desktop_set_back_color(color);
    osui_ui_statbar_set_font_color(inv_color);
    osui_ui_navbar_set_color(inv_color);
}

// //rgb565转lvgl颜色
// static lv_color_t u16_to_lvc(int16_t c)
// {
//     lv_color_t ret;
//     ret.red = (c >> 11) & 0xff;
//     ret.green = (c >> 6) & 0xff;
//     ret.blue = (c >> 0) & 0xff;
//     return ret;
// }

//UI主题初始化
void osui_ui_theme_init()
{
    theme_logic.color = OSUI_THEME_LIGHT;
    set_obj_color(OSUI_THEME_LIGHT);
}

//UI设置主题色
void osui_ui_theme_set_color(lv_color_t color)
{
    theme_logic.color = color;
    set_obj_color(color);
}

//UI获取主题色
lv_color_t osui_ui_theme_get_color()
{
    return theme_logic.color;
}
