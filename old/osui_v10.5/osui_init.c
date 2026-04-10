#include"osui_init.h"

//显示
#include"osui/disp/osui_disp.h"
//输入
#include"osui/input/osui_input.h"
//系统
#include"osui/sys/osui_sys.h"
//UI框架
#include"osui/ui/osui_ui_frame.h"
//UI锁屏
#include"osui/ui/osui_ui_locker.h"
//UI桌面
#include"osui/ui/osui_ui_desktop.h"
//UI导航栏
#include"osui/ui/osui_ui_navbar.h"
//UI状态栏
#include"osui/ui/osui_ui_statbar.h"
//UI主题
#include"osui/ui/osui_ui_theme.h"

//应用
#include"osui/app/app_example.h"
#include"osui/app/app_file_explorer.h"

//壁纸路径
#define OSUI_WP_L "C:/Users/Windows/Desktop/project/lv_port_win_codeblocks-master/osui/wp/wp_l.bmp"
#define OSUI_WP_D "C:/Users/Windows/Desktop/project/lv_port_win_codeblocks-master/osui/wp/wp_d.bmp"

//测试回调
static void test_callback(lv_event_t*e)
{
    static bool a=0;
    if(!a)
    {
        osui_ui_theme_set_color(OSUI_THEME_DARK);
        //设置壁纸
        osui_ui_locker_set_wallpaper(OSUI_WP_D);
        osui_ui_desktop_set_wallpaper(OSUI_WP_D);    
        a=!a;
    }else
    {
        osui_ui_theme_set_color(OSUI_THEME_LIGHT);
        //设置壁纸
        osui_ui_locker_set_wallpaper(OSUI_WP_L);
        osui_ui_desktop_set_wallpaper(OSUI_WP_L);
        a=!a;
    }
}


//测试应用1
static lv_obj_t* test1()
{
    lv_obj_t * root = lv_obj_create(NULL);
    lv_obj_t * parent = lv_obj_create(root);
    lv_obj_set_pos(parent, 0, 0);
    lv_obj_t * obj = lv_win_create(parent);
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, osui_ui_desktop_get_width(), osui_ui_desktop_get_height());
    lv_win_add_button(obj, LV_SYMBOL_LEFT, 40);
    lv_win_add_title(obj, "A title");
    lv_win_add_button(obj, LV_SYMBOL_RIGHT, 40);
    lv_win_add_button(obj, LV_SYMBOL_CLOSE, 60);
    lv_obj_t * cont = lv_win_get_content(obj);  /*Content can be added here*/
    lv_obj_t * label = lv_label_create(cont);
    lv_label_set_text(label, "This is\n"
                      "a pretty\n"
                      "long text\n"
                      "to see how\n"
                      "the window\n"
                      "becomes\n"
                      "scrollable.\n"
                      "\n"
                      "\n"
                      "Some more\n"
                      "text to be\n"
                      "sure it\n"
                      "overflows. :)");
    return parent;
}

//测试应用2
static lv_obj_t* test2()
{
    lv_obj_t * root = lv_obj_create(NULL);
    lv_obj_t * parent = lv_obj_create(root);
    lv_obj_set_pos(parent, 0, 0);
    lv_obj_t * obj = lv_list_create(parent);
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, osui_ui_desktop_get_width(), osui_ui_desktop_get_height());
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_list_add_text(obj, "File");
    lv_list_add_button(obj, LV_SYMBOL_FILE, "New");
    lv_list_add_button(obj, LV_SYMBOL_DIRECTORY, "Open");
    lv_list_add_button(obj, LV_SYMBOL_SAVE, "Save");
    lv_list_add_button(obj, LV_SYMBOL_CLOSE, "Delete");
    lv_list_add_button(obj, LV_SYMBOL_EDIT, "Edit");
    lv_list_add_text(obj, "Connectivity");
    lv_list_add_button(obj, LV_SYMBOL_BLUETOOTH, "Bluetooth");
    lv_list_add_button(obj, LV_SYMBOL_GPS, "Navigation");
    lv_list_add_button(obj, LV_SYMBOL_USB, "USB");
    lv_list_add_button(obj, LV_SYMBOL_BATTERY_FULL, "Battery");
    lv_list_add_text(obj, "Exit");
    lv_list_add_button(obj, LV_SYMBOL_OK, "Apply");
    lv_list_add_button(obj, LV_SYMBOL_CLOSE, "Close");
    return parent;
}

//测试应用3
static lv_obj_t* test3()
{
    lv_obj_t * root = lv_obj_create(NULL);
    lv_obj_t * parent = lv_obj_create(root);
    lv_obj_set_pos(parent, 0, 0);
    // lv_obj_set_style_bg_opa(parent, 128, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_t *img = lv_image_create(parent);
    // lv_image_set_src(img, "C:/Users/Windows/Desktop/Desktop/LVUI/prj2/lv_port_win_codeblocks-master/osui/wallpaper.bmp");
    lv_obj_t * obj = lv_spinner_create(parent);
    lv_obj_set_pos(obj, 0, 0);
    // lv_obj_set_size(obj, osui_ui_desktop_get_width(), osui_ui_desktop_get_height());
    lv_obj_set_size(obj, 100, 100);
    lv_obj_center(obj);
    lv_spinner_set_anim_params(obj, 10000, 200);

    lv_obj_t*btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 80, 40);
    lv_obj_set_pos(btn, osui_ui_desktop_get_width()*0.5-40, osui_ui_desktop_get_height()*0.75);
    lv_obj_add_event_cb(btn, test_callback, LV_EVENT_PRESSED, NULL);

    return parent;
}

//OSUI初始化
void osui_init()
{
    //显示初始化
    osui_disp_init(320,480,0);
    // osui_disp_init(240,320,0);
    // osui_disp_init(480,720,0);
    //输入初始化
    osui_input_init();
    //UI框架初始化
    osui_ui_frame_init();
    //UI桌面初始化
    osui_ui_desktop_init();
    //UI导航条初始化
    osui_ui_navbar_init();
    //UI状态栏初始化
    osui_ui_statbar_init();
    //UI锁屏初始化
    osui_ui_locker_init();
    //UI框架载入
    // osui_ui_frame_enter_locker();
    osui_ui_frame_load();
    //UI锁屏进入
    osui_ui_locker_enter();

    //添加应用
    osui_ui_desktop_add_app(LV_SYMBOL_WARNING "  " "Test1", test1());
    osui_ui_desktop_add_app(LV_SYMBOL_SETTINGS "  " "Test2", test2());
    osui_ui_desktop_add_app(LV_SYMBOL_BELL "  " "Test333333333333333333333333333333333", test3());
    osui_ui_desktop_add_app(LV_SYMBOL_CHARGE "  " "APP EXAMPLE", app_example_init());
    osui_ui_desktop_add_app(LV_SYMBOL_FILE "  " "FILE EXPLORER", app_file_explorer_init());
    
    //系统初始化
    osui_sys_init();
    osui_sys_set_wifi_stat(1);
    osui_sys_set_bt_stat(1);
    osui_sys_refresh();

    // osui_ui_img_init();

    //UI设置主题色
    // osui_ui_theme_set_color(lv_palette_main(LV_PALETTE_GREEN));
    // osui_ui_theme_set_color(lv_color_hex(0xffffffff));

    //UI主题初始化
    osui_ui_theme_init();
    
    //设置壁纸
    osui_ui_locker_set_wallpaper(OSUI_WP_L);
    osui_ui_desktop_set_wallpaper(OSUI_WP_L);
    
    // //打开WiFi蓝牙
    // osui_sys_set_wifi_stat(1);
    // osui_sys_set_bt_stat(1);
    // osui_sys_refresh();
}
