/**
 * @file lu_app_theme.c
 * @brief 主题设置应用 - 提供主题模式切换和主题颜色选择功能
 *
 * 本应用展示如何使用LVUI的主题系统，包含三个核心功能：
 * 1. 主题模式切换 - 通过开关组件切换明亮/暗黑模式
 * 2. 主题颜色预览 - 实时显示当前主题色，带选中边框指示
 * 3. 主题颜色选择 - 通过颜色按钮行选择主题主色调
 *
 * 预览区域通过LVGL定时器每200ms自动刷新，同步外部主题变更
 */

#include "lvui/app/lu_app_theme.h"
#include "lvui/frame/widget/lu_widget.h"
#include "lvui/frame/disp/lu_font.h"
#include "lvui/frame/disp/lu_disp.h"
#include "lvui/frame/theme/lu_theme.h"
#include "lvui/sys/lu_sysui.h"

/*============================
 *    布局与颜色常量定义
 *============================*/

/* 整体内边距 */
#define PADDING         16
/* 颜色按钮尺寸 */
#define BTN_SIZE        42
/* 颜色按钮间距 */
#define BTN_GAP         8
/* 标题与控件之间的间距 */
#define SECTION_GAP     12
/* 区段之间的间距 */
#define BLOCK_GAP       20
/* 开关宽度 */
#define SWITCH_WIDTH    52
/* 开关高度 */
#define SWITCH_HEIGHT   28
/* 选中颜色的边框宽度 */
#define BORDER_WIDTH    3

/* 预设主题颜色调色板 */
static const lv_color_t THEME_COLORS[] =
{
    LV_COLOR_MAKE(0x21, 0x96, 0xF3),  /* 蓝色 - 默认 */
    LV_COLOR_MAKE(0x4C, 0xAF, 0x50),  /* 绿色 */
    LV_COLOR_MAKE(0xFF, 0x98, 0x00),  /* 橙色 */
    LV_COLOR_MAKE(0xE9, 0x1E, 0x63),  /* 粉色 */
    LV_COLOR_MAKE(0x9C, 0x27, 0xB0),  /* 紫色 */
    LV_COLOR_MAKE(0x00, 0xBC, 0xD4),  /* 青色 */
    LV_COLOR_MAKE(0xFF, 0x57, 0x22),  /* 深橙 */
    LV_COLOR_MAKE(0x79, 0x55, 0x48),  /* 棕色 */
    LV_COLOR_MAKE(0x60, 0x7D, 0x8B),  /* 蓝灰 */
    LV_COLOR_MAKE(0x33, 0x33, 0x33),  /* 深灰 */
};
#define COLOR_COUNT (sizeof(THEME_COLORS) / sizeof(THEME_COLORS[0]))

/* LVGL颜色比较辅助宏 */
#define COLOR_EQUAL(c1, c2) \
    ((c1).blue == (c2).blue && (c1).green == (c2).green && (c1).red == (c2).red)

/* 组件句柄和主题指针（供回调和定时器使用） */
static lv_obj_t* s_color_row = NULL;
static lv_obj_t* s_preview_box = NULL;
static lv_obj_t* s_mode_switch = NULL;
static lu_theme_t* s_theme = NULL;

/*============================
 *       内部辅助函数
 *============================*/

/**
 * @brief 同步颜色选择行的选中边框
 */
static void update_color_border(void)
{
    if(!s_color_row || !s_theme) return;

    lv_color_t cur = lu_theme_get_color(s_theme);
    uint32_t cnt = lv_obj_get_child_cnt(s_color_row);

    for(uint32_t i = 0; i < cnt && i < COLOR_COUNT; i++)
    {
        lv_obj_t* btn = lv_obj_get_child(s_color_row, i);
        if(!btn) continue;

        if(COLOR_EQUAL(THEME_COLORS[i], cur))
        {
            lv_obj_set_style_border_width(btn, BORDER_WIDTH, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(btn, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(btn, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_side(btn, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
        }else
        {
            lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

/*============================
 *     LVGL定时器回调
 *============================*/

/**
 * @brief 每200ms自动刷新一次预览和开关状态
 *        确保外部变更主题时UI同步更新
 */
static void theme_preview_timer_cb(lv_timer_t* timer)
{
    LV_UNUSED(timer);
    if(!s_theme) return;

    /* 同步开关指示器颜色 */
    if(s_mode_switch)
    {
        lv_obj_set_style_bg_color(s_mode_switch, lu_theme_get_color(s_theme),
            LV_PART_INDICATOR | LV_STATE_CHECKED);
    }

    /* 同步预览框颜色 */
    if(s_preview_box)
    {
        lv_obj_set_style_bg_color(s_preview_box, lu_theme_get_color(s_theme),
            LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

/*============================
 *       事件回调函数
 *============================*/

/**
 * @brief 主题模式开关回调
 *        开关打开时切换为暗黑模式，关闭时切换为明亮模式
 */
static void theme_mode_cb(lv_event_t* e)
{
    lv_obj_t* sw = lv_event_get_target_obj(e);
    if(!sw || !s_theme) return;

    if(lv_obj_has_state(sw, LV_STATE_CHECKED))
    {
        lu_theme_set_mode(s_theme, LU_THEME_MODE_DARK);
    }else
    {
        lu_theme_set_mode(s_theme, LU_THEME_MODE_BRIGHT);
    }
}

/**
 * @brief 主题颜色按钮点击回调
 *        设置主题色并同步更新边框指示和预览
 */
static void theme_color_cb(lv_event_t* e)
{
    lv_obj_t* btn = lv_event_get_target_obj(e);
    if(!btn || !s_theme) return;

    int32_t idx = lv_obj_get_index(btn);
    if(idx >= 0 && idx < (int32_t)COLOR_COUNT)
    {
        lu_theme_set_color(s_theme, THEME_COLORS[idx]);
        update_color_border();
        if(s_preview_box)
        {
            lv_obj_set_style_bg_color(s_preview_box, THEME_COLORS[idx],
                LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

/*============================
 *       应用创建入口
 *============================*/

lv_obj_t* lu_app_theme_create(lvui_t* lvui)
{
    if(!lvui) return NULL;

    lu_disp_t* disp = lvui->lu_disp;
    lu_sysui_t* sysui = lvui->sysui;
    s_theme = lu_sysui_get_theme(sysui);
    float dpi = lu_disp_get_dpi(disp);
    int32_t disp_w = lu_disp_get_hor(disp);
    int32_t disp_h = lu_disp_get_ver(disp);

    /* 标题起始位置：距顶部 1/10 屏幕高度 */
    int32_t title_y = disp_h / 10;

    /* 创建应用容器（全屏，继承主题） */
    lv_obj_t* app_contain = lu_widget_obj_init(lv_screen_active());
    if(!app_contain) return NULL;

    lv_obj_set_size(app_contain, disp_w, disp_h);
    lv_obj_set_style_bg_opa(app_contain, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(app_contain, lu_theme_get_color(s_theme), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(app_contain, PADDING, LV_PART_MAIN | LV_STATE_DEFAULT);

    lu_theme_info_t theme_info =
    {
        .self_type = LU_THEME_TYPE_MODE,
        .flag = LU_THEME_FLAG_SELF,
    };
    lu_theme_add_obj(s_theme, app_contain, &theme_info);

    /* ---- 标题 ---- */
    lv_obj_t* title = lu_widget_txt_init(app_contain,
        lu_font_get(dpi, LU_FONT_SIZE_XXLARGE, 1));
    theme_info.self_type = LU_THEME_TYPE_INVMODE;
    lu_theme_add_obj(s_theme, title, &theme_info);
    lv_label_set_text(title, "Theme Settings");
    lv_obj_set_pos(title, 0, title_y);

    /* ---- 主题模式切换 ---- */
    int32_t y = title_y + SECTION_GAP + lu_font_get(dpi, LU_FONT_SIZE_XXLARGE, 1)->line_height;

    /* 模式标签 */
    lv_obj_t* mode_label = lu_widget_txt_init(app_contain,
        lu_font_get(dpi, LU_FONT_SIZE_LARGE, 1));
    lu_theme_add_obj(s_theme, mode_label, &theme_info);
    lv_label_set_text(mode_label, "Dark Mode");
    lv_obj_set_pos(mode_label, 0, y);

    /* 模式开关（放在标签下方，使用LVGL原生样式） */
    int32_t label_h = lu_font_get(dpi, LU_FONT_SIZE_LARGE, 1)->line_height;
    y += label_h + 6;

    s_mode_switch = lv_switch_create(app_contain);
    lv_obj_set_size(s_mode_switch, SWITCH_WIDTH, SWITCH_HEIGHT);
    lv_obj_set_pos(s_mode_switch, 0, y);
    lv_obj_set_style_bg_color(s_mode_switch, lu_theme_get_color(s_theme),
        LV_PART_INDICATOR | LV_STATE_CHECKED);
    if(lu_theme_get_mode(s_theme) == LU_THEME_MODE_DARK)
    {
        lv_obj_add_state(s_mode_switch, LV_STATE_CHECKED);
    }
    lv_obj_add_event_cb(s_mode_switch, theme_mode_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* ---- 主题颜色预览 ---- */
    y += SWITCH_HEIGHT + BLOCK_GAP;

    /* 预览标签 */
    lv_obj_t* preview_label = lu_widget_txt_init(app_contain,
        lu_font_get(dpi, LU_FONT_SIZE_LARGE, 1));
    lu_theme_add_obj(s_theme, preview_label, &theme_info);
    lv_label_set_text(preview_label, "Theme Preview");
    lv_obj_set_pos(preview_label, 0, y);

    /* 预览按钮行（方形，不可点击，仅展示当前主题色） */
    y += SECTION_GAP + label_h;

    /* 预览框（单个方形色块，展示当前主题色） */
    s_preview_box = lu_widget_obj_init(app_contain);
    lv_obj_set_size(s_preview_box, BTN_SIZE, BTN_SIZE);
    lv_obj_set_pos(s_preview_box, 0, y);
    lv_obj_set_style_bg_opa(s_preview_box, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(s_preview_box, lu_theme_get_color(s_theme), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(s_preview_box, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(s_preview_box, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(s_preview_box, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(s_preview_box, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(s_preview_box, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);

    /* ---- 主题颜色选择 ---- */
    y += BTN_SIZE + BLOCK_GAP;

    /* 颜色标签 */
    lv_obj_t* color_label = lu_widget_txt_init(app_contain,
        lu_font_get(dpi, LU_FONT_SIZE_LARGE, 1));
    lu_theme_add_obj(s_theme, color_label, &theme_info);
    lv_label_set_text(color_label, "Theme Color");
    lv_obj_set_pos(color_label, 0, y);

    /* 颜色按钮行（圆形，可点击） */
    y += SECTION_GAP + label_h;

    s_color_row = lu_widget_obj_init(app_contain);
    lv_obj_set_size(s_color_row, disp_w - PADDING * 2, LV_SIZE_CONTENT);
    lv_obj_set_pos(s_color_row, 0, y);
    lv_obj_set_flex_flow(s_color_row, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_gap(s_color_row, BTN_GAP, LV_PART_MAIN | LV_STATE_DEFAULT);

    for(int i = 0; i < (int)COLOR_COUNT; i++)
    {
        lv_obj_t* btn = lu_widget_btn_init(s_color_row);
        lv_obj_set_size(btn, BTN_SIZE, BTN_SIZE);
        lv_obj_set_style_bg_color(btn, THEME_COLORS[i], LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(btn, BTN_SIZE / 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_event_cb(btn, theme_color_cb, LV_EVENT_CLICKED, NULL);
    }
    update_color_border();

    /* 创建定时器，每200ms自动刷新预览 */
    lv_timer_create(theme_preview_timer_cb, 200, NULL);

    return app_contain;
}
