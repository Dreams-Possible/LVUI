#include"app_example.h"

//UI框架
#include"osui/ui/osui_ui_frame.h"
//UI标签
#include"osui/ui/osui_ui_lable.h"
//UI动画
#include"osui/ui/osui_ui_anim.h"
//UI调试
#include"osui/ui/osui_ui_debug.h"
//UI锁屏
#include"osui/ui/osui_ui_locker.h"
//UI桌面
#include"osui/ui/osui_ui_desktop.h"
//UI主题
#include"osui/ui/osui_ui_theme.h"

//参数定义
#define APP_MAIN_TAB_HEIGHT ((uint16_t)osui_disp_dp2px(36))//主选项卡高度
#define APP_MAIN_BAR_HEIGHT ((uint16_t)osui_disp_dp2px(36))//顶栏高度
#define APP_MAIN_BAR_OPA (LV_OPA_50)//顶栏透明度
#define APP_TOOL_LIST_RATIO (0.8)//工具列表比例
#define APP_TOOL_ANIM_TIME 600//工具动画时间

//应用UI类
typedef struct app_ui_t
{
    lv_obj_t *root;
    lv_obj_t *parent;
    lv_obj_t *main_tab;
    lv_obj_t *top_bar;
    lv_obj_t *tool_obj;
    lv_obj_t *tool_mask;
    lv_obj_t *tool_list;
    osui_ui_anim_lv_t*tool_mask_anim;
} app_ui_t;
static app_ui_t app_ui = {0};

//应用示例初始化
lv_obj_t* app_example_init();
//工具按钮点击回调
static void tool_btn_event_cb(lv_event_t * e);
//工具遮罩点击回调
static void tool_mask_event_cb(lv_event_t * e);

//应用示例初始化
lv_obj_t* app_example_init()
{
    //创建容器
    app_ui.root = lv_obj_create(NULL);
    //创建父对象
    lv_obj_t *obj = lv_obj_create(app_ui.root);
    app_ui.parent = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, osui_ui_desktop_get_width(), osui_ui_desktop_get_height());
    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    //创建应用布局
    lv_obj_t *parent_obj = obj;
    {
        {
            //主选项卡
            lv_obj_t *obj = lv_tabview_create(parent_obj);
            app_ui.main_tab = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, osui_ui_desktop_get_width(), osui_ui_desktop_get_height());
            lv_tabview_set_tab_bar_position(obj, LV_DIR_BOTTOM);
            lv_tabview_set_tab_bar_size(obj, APP_MAIN_TAB_HEIGHT);
            //添加内容
            lv_obj_t* tab1 = lv_tabview_add_tab(obj, "Tab1");
            lv_obj_t* label1 = lv_label_create(tab1);
            lv_obj_set_style_text_font(label1, &OSUI_UI_LABLE_FONT_CONTENT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(label1, "This is tab1");
            lv_obj_set_align(label1, LV_ALIGN_CENTER);
            lv_obj_t* tab2 = lv_tabview_add_tab(obj, "Tab2");
            lv_obj_t* label2 = lv_label_create(tab2);
            lv_obj_set_style_text_font(label2, &OSUI_UI_LABLE_FONT_CONTENT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(label2, "This is tab2");
            lv_obj_set_align(label2, LV_ALIGN_CENTER);
        }
        {
            //顶栏
            lv_obj_t *obj = lv_obj_create(parent_obj);
            app_ui.top_bar = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, osui_ui_desktop_get_width(), APP_MAIN_BAR_HEIGHT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, osui_ui_theme_get_color(), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, APP_MAIN_BAR_OPA, LV_PART_MAIN | LV_STATE_DEFAULT);
            //添加内容
            lv_obj_t* label1 = lv_label_create(obj);
            lv_obj_set_style_text_font(label1, &OSUI_UI_LABLE_FONT_CONTENT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(label1, "This is top bar");
            lv_obj_set_align(label1, LV_ALIGN_CENTER);
            {
                lv_obj_t *parent_obj = obj;
                {
                    //工具按钮
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    app_ui.tool_obj = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, APP_MAIN_BAR_HEIGHT, APP_MAIN_BAR_HEIGHT);
                    lv_obj_set_style_transform_height(obj, 0, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_transform_width(obj, 0, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_offset_y(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_add_event_cb(obj, tool_btn_event_cb, LV_EVENT_CLICKED, NULL);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &OSUI_UI_LABLE_FONT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, LV_SYMBOL_LIST);
                        }
                    }
                }
            }
        }
    }
    {
        //工具遮罩
        lv_obj_t *obj = lv_obj_create(parent_obj);
        app_ui.tool_mask = obj;
        lv_obj_set_pos(obj, -osui_ui_desktop_get_width(), 0);
        lv_obj_set_size(obj, osui_ui_desktop_get_width(), osui_ui_desktop_get_height());
        lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_event_cb(obj, tool_mask_event_cb, LV_EVENT_CLICKED, NULL);
        {
            lv_obj_t *parent_obj = obj;
            {
                //工具列表
                lv_obj_t *obj = lv_list_create(parent_obj);
                app_ui.tool_list = obj;
                lv_obj_set_pos(obj, 0, 0);
                lv_obj_set_size(obj, APP_TOOL_LIST_RATIO*osui_ui_desktop_get_width(), osui_ui_desktop_get_height());
                // lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                // lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_list_add_button(obj, LV_SYMBOL_FILE, "File");
                lv_list_add_button(obj, LV_SYMBOL_EDIT, "Edit");
                lv_list_add_button(obj, LV_SYMBOL_CUT, "Cut");
                lv_list_add_button(obj, LV_SYMBOL_COPY, "Copy");
            }
        }
    }
    //初始化动画
    app_ui.tool_mask_anim = osui_ui_anim_lv_init();
    osui_ui_anim_lv_set_obj(app_ui.tool_mask_anim, app_ui.tool_mask);
    return app_ui.parent;
}


//工具按钮点击回调
static void tool_btn_event_cb(lv_event_t * e)
{
    //工具列表展开动画
    osui_ui_anim_lv_set_param(app_ui.tool_mask_anim, -osui_ui_desktop_get_width(), 0);
    osui_ui_anim_lv_set_time(app_ui.tool_mask_anim, APP_TOOL_ANIM_TIME);
    osui_ui_anim_lv_set_exec_cb(app_ui.tool_mask_anim, osui_ui_anim_lv_exec_cb_x);
    osui_ui_anim_lv_set_finish_cb(app_ui.tool_mask_anim, NULL);
    osui_ui_anim_lv_set_bezier_param(app_ui.tool_mask_anim, 1024, 1024);
    osui_ui_anim_lv_start(app_ui.tool_mask_anim);
}

//工具遮罩点击回调
static void tool_mask_event_cb(lv_event_t * e)
{
    //工具列表收起动画
    osui_ui_anim_lv_set_param(app_ui.tool_mask_anim, 0, -osui_ui_desktop_get_width());
    osui_ui_anim_lv_set_time(app_ui.tool_mask_anim, APP_TOOL_ANIM_TIME);
    osui_ui_anim_lv_set_exec_cb(app_ui.tool_mask_anim, osui_ui_anim_lv_exec_cb_x);
    osui_ui_anim_lv_set_finish_cb(app_ui.tool_mask_anim, NULL);
    osui_ui_anim_lv_set_bezier_param(app_ui.tool_mask_anim, 1024, 1024);
    osui_ui_anim_lv_start(app_ui.tool_mask_anim);
}
