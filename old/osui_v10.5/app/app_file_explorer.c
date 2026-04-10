#include"app_file_explorer.h"

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

//文件系统依赖
#include <stdio.h>
#include <dirent.h>
// #include <sys/types.h>

//参数定义
// #define APP_MAIN_TAB_HEIGHT ((uint16_t)osui_disp_dp2px(36))//主选项卡高度
#define APP_MAIN_BAR_HEIGHT ((uint16_t)osui_disp_dp2px(42))//顶栏高度
// #define APP_MAIN_BAR_OPA (LV_OPA_50)//顶栏透明度
#define APP_FILE_PATH_MAX (1024)//文件路径最大长度
#define APP_FILE_SYS_ROOT "/"//文件系统根目录
#define APP_FILE_LIST_ITEM_HEIGHT ((uint16_t)osui_disp_dp2px(42))//文件列表项目高度
#define APP_FILE_LIST_ITEM_GAP ((int16_t)(osui_disp_dp2px(2)))//文件列表项目间距

// #define APP_TOOL_ANIM_TIME 600//工具动画时间

//应用UI类
typedef struct app_ui_t
{
    lv_obj_t *root;
    lv_obj_t *parent;
    lv_obj_t *top_bar;
    lv_obj_t *file_path;
    lv_obj_t *file_list;
    char current_path[APP_FILE_PATH_MAX];
    char last_path[APP_FILE_PATH_MAX];
} app_ui_t;
static app_ui_t app_ui = {0};

//文件管理器初始化
lv_obj_t* app_file_explorer_init();
//文件列表点击回调
static void file_list_event_cb(lv_event_t * e);
//打开目录
static uint8_t open_dir(const char* path);

//文件管理器初始化
lv_obj_t* app_file_explorer_init()
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
            //顶栏
            lv_obj_t *obj = lv_obj_create(parent_obj);
            app_ui.top_bar = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, osui_ui_desktop_get_width(), APP_MAIN_BAR_HEIGHT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_offset_y(obj, APP_FILE_LIST_ITEM_GAP, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    //文件路径
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    app_ui.file_path = obj;
                    lv_obj_set_align(obj, LV_ALIGN_CENTER);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &OSUI_UI_LABLE_FONT_CONTENT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_fmt(obj, APP_FILE_SYS_ROOT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_SCROLL_CIRCULAR);
                }
            }
        }
        {
            //文件列表
            lv_obj_t *obj = lv_list_create(parent_obj);
            app_ui.file_list = obj;
            lv_obj_set_pos(obj, 0, APP_MAIN_BAR_HEIGHT+APP_FILE_LIST_ITEM_GAP);
            lv_obj_set_size(obj, osui_ui_desktop_get_width(), osui_ui_desktop_get_height()-APP_MAIN_BAR_HEIGHT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_row(obj, APP_FILE_LIST_ITEM_GAP, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
            lv_obj_set_scroll_dir(obj, LV_DIR_VER);
        }
    }
    //打开根目录
    snprintf(app_ui.current_path, APP_FILE_PATH_MAX, APP_FILE_SYS_ROOT);
    open_dir(app_ui.current_path);
    return app_ui.parent;
}


//文件列表点击回调
static void file_list_event_cb(lv_event_t * e)
{
    const char * name = lv_event_get_user_data(e);
    osui_log("file_list_event_cb: %s", name);
    //备份当前路径
    memset(app_ui.last_path, 0, APP_FILE_PATH_MAX);
    snprintf(app_ui.last_path, APP_FILE_PATH_MAX, "%s", app_ui.current_path);
    //更新路径
    // osui_log("current_path=%s",app_ui.current_path);
    // osui_log("last_path=%s",app_ui.last_path);
    //当前路径
    if(!strcmp(name,"."))
    {
        return;
    }else
    //上级目录
    if(!strcmp(name,".."))
    {
        while(app_ui.current_path[strlen(app_ui.current_path)-1]!='/')
        {
            app_ui.current_path[strlen(app_ui.current_path)-1] = '\0';
        }
        if(strlen(app_ui.current_path) > strlen(APP_FILE_SYS_ROOT))
        {
            app_ui.current_path[strlen(app_ui.current_path)-1] = '\0';
        }
    }else
    {
        // osui_log("current_path=%s",app_ui.current_path);
        //下级目录
        if(app_ui.current_path[strlen(app_ui.current_path)-1]=='/')
        {
            // snprintf(app_ui.current_path, APP_FILE_PATH_MAX, "%s%s", app_ui.last_path, name);
            strncat(app_ui.current_path, name, APP_FILE_PATH_MAX-strlen(app_ui.current_path)-1);
        }else
        {
            app_ui.current_path[strlen(app_ui.current_path)] = '/';
            strncat(app_ui.current_path, name, APP_FILE_PATH_MAX-strlen(app_ui.current_path)-1);
        }
    }
    // osui_log("current_path=%s",app_ui.current_path);
    //打开目录
    if(open_dir(app_ui.current_path))
    {
        //恢复备份路径
        memset(app_ui.current_path, 0, APP_FILE_PATH_MAX);
        snprintf(app_ui.current_path, APP_FILE_PATH_MAX, "%s", app_ui.last_path);
        open_dir(app_ui.current_path);
    }
}



//打开目录
static uint8_t open_dir(const char* path)
{
    osui_log("opendir=%s", path);
    //打开目录
    DIR *dir = opendir(path);
    if (!dir)
    {
        osui_log("cannot open directory: %s", path);
        return 1;
    }
    //删除旧列表
    if(app_ui.file_list)
    {
        osui_log("delete old list");
        // 清理应用列表
        lv_obj_t * list = app_ui.file_list;
        for(uint16_t f = 0; f < lv_obj_get_child_count(list); ++f)
        {
            lv_obj_t * btn = lv_obj_get_child(list, f);
            char * file_name = (char*)lv_obj_get_user_data(btn);
            if(file_name)
            {
                osui_free(file_name);
            }
        }
        while(lv_obj_get_child_count(list))
        {
            lv_obj_delete(lv_obj_get_child(list, 0));
        }      
        // lv_obj_delete(app_ui.file_list);
        osui_log("number=%d",lv_obj_get_child_count(list));
    }
    // //创建新列表
    // lv_obj_t *obj = lv_list_create(app_ui.parent);
    // app_ui.file_list = obj;
    // lv_obj_set_size(obj, osui_ui_desktop_get_width(), osui_ui_desktop_get_height());
    // // lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // // lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    //遍历目录
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL)
    {
        // osui_log("  %s", entry->d_name);
        //保存文件名
        char * file_name = osui_malloc(strlen(entry->d_name) + 1);
        strcpy(file_name, entry->d_name);
        // lv_obj_add_event_cb(btn, file_list_event_cb, LV_EVENT_CLICKED, file_name);
        //创建列表项
        // lv_obj_t * btn = lv_list_add_button(app_ui.file_list, LV_SYMBOL_FILE, entry->d_name);
        lv_obj_t * btn = lv_button_create(app_ui.file_list);
        lv_obj_set_size(btn, osui_ui_desktop_get_width(), APP_FILE_LIST_ITEM_HEIGHT);
        lv_obj_set_style_transform_height(btn, 0, LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_set_style_transform_width(btn, 0, LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_set_style_shadow_width(btn, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_offset_y(btn, APP_FILE_LIST_ITEM_GAP, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_pad_top(btn, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_pad_bottom(btn, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_set_user_data(btn, (char*)file_name);
        lv_obj_add_event_cb(btn, file_list_event_cb, LV_EVENT_CLICKED, (char*)file_name);
        lv_obj_t * label = lv_label_create(btn);
        lv_obj_set_pos(label, 0, 0);
        lv_obj_set_size(label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_style_text_color(label, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(label, &OSUI_UI_LABLE_FONT_CONTENT, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(label, file_name);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        // osui_log("\n");
    }
    //关闭目录
    closedir(dir);
    //更新文件路径
    lv_label_set_text_fmt(app_ui.file_path, app_ui.current_path);
    return 0;
}

