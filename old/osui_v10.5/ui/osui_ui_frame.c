#include"osui_ui_frame.h"

//参数定义
#define FRAME_ANIM_TIME (200)//框架动画时间

//UI框架类
typedef struct osui_uiframe_t
{
    lv_obj_t *frame;
} osui_uiframe_t;
//UI框架
static osui_uiframe_t osui_uiframe;

//UI框架初始化
void osui_ui_frame_init();
//UI框架获取桌面
lv_obj_t **osui_uiframe_get();
//UI框架载入
void osui_ui_frame_load();

//UI框架初始化
void osui_ui_frame_init()
{
    //创建默认主题
    lv_disp_t *disp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, theme);
    //创建UI框架
    osui_uiframe.frame = lv_obj_create(0);
    //对于小端序SPI传输（可能的需要）
    // lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565_SWAPPED);
}

//UI框架获取桌面
lv_obj_t **osui_uiframe_get()
{
    return &osui_uiframe.frame;
}

//UI框架载入
void osui_ui_frame_load()
{
    lv_screen_load_anim(osui_uiframe.frame, LV_SCR_LOAD_ANIM_FADE_IN, FRAME_ANIM_TIME, 0, false);
}
