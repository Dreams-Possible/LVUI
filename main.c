
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <unistd.h>

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static const wchar_t * title = L"LVGL port Windows CodeBlocks.      https://lvgl.io | https://docs.lvgl.io";

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/




#include <time.h>
#include "lvui/lvui.h"


lvui_t* lvui = NULL;


static void app_event_cb(lu_app_t* app)
{
    LV_LOG_USER("app_event_cb!");
    // lu_launcher_open_app(lu_sysui_get_launcher(lvui->sysui), app);
}

static void ctrbtn_event_cb(lu_ctrbtn_t* ctrbtn)
{
    LV_LOG_USER("ctrbtn_event_cb!");
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
    /*Initialize LVGL*/
    lv_init();

    /*Initialize the HAL for LVGL*/
    lv_display_t * display = lv_windows_create_display(title, 800, 600, 100, FALSE, FALSE);
    lv_windows_acquire_pointer_indev(display);

    /*Output prompt information to the console, you can also use printf() to print directly*/
    LV_LOG_USER("LVGL initialization completed!");

    // /*Run the demo*/
    // lv_demo_widgets();

    // char * demo_str[] = {"widgets"};
    // lv_demos_create(demo_str, strlen((char * )demo_str));


    lu_init();

    LV_LOG_USER("lvui_create start!");

    lvui = lvui_create(320, 480, 3.5);

    if(lvui)
    {
        LV_LOG_USER("lvui_create SUCCESS!");
    }else
    {
        LV_LOG_USER("lvui_create FAIL!");
    }



    lv_obj_t* app_contain = lu_widget_obj_init(lu_launcher_get_app_contain(lu_sysui_get_launcher(lvui->sysui)));
    lv_obj_set_style_bg_opa(app_contain, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(app_contain, lu_theme_get_color(lu_sysui_get_theme(lvui->sysui)), LV_PART_MAIN | LV_STATE_DEFAULT);
    lu_theme_info_t theme_info = 
    {
        .self_type = LU_THEME_TYPE_THEME,
        .flag = LU_THEME_FLAG_SELF,
    };
    lu_theme_add_obj(lu_sysui_get_theme(lvui->sysui), app_contain, &theme_info);
    lv_obj_t* txt= lu_widget_txt_init(
        app_contain,
        lu_font_get(lu_disp_get_dpi(lvui->lu_disp), LU_FONT_SIZE_MAX, 1));
    theme_info.self_type = LU_THEME_TYPE_INVMODE;
    lu_theme_add_obj(lu_sysui_get_theme(lvui->sysui), txt, &theme_info);
    lv_label_set_text(txt, "APP CONTAIN");
    lv_obj_set_align(txt, LV_ALIGN_CENTER);

    lu_app_info_t app_info =
    {
        .app_contain = app_contain,
        .app_name = "APP",
        .app_icon = LV_SYMBOL_SETTINGS,
        // .app_cb = app_event_cb,
    };
    app_info.app_pos.x = 0;
    app_info.app_pos.y = 0;
    app_info.app_size = LU_APP_SIZE_SMALL;
    lu_app_t* app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);
    app_info.app_pos.x = 0;
    app_info.app_pos.y = 1;
    app_info.app_size = LU_APP_SIZE_SMALL;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);
    app_info.app_pos.x = 1;
    app_info.app_pos.y = 0;
    app_info.app_size = LU_APP_SIZE_LARGE;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);
    app_info.app_pos.x = 5;
    app_info.app_pos.y = 0;
    app_info.app_size = LU_APP_SIZE_SMALL;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);
    app_info.app_pos.x = 5;
    app_info.app_pos.y = 1;
    app_info.app_size = LU_APP_SIZE_SMALL;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);

    app_info.app_pos.x = 0;
    app_info.app_pos.y = 2;
    app_info.app_size = LU_APP_SIZE_SMALL;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);
    app_info.app_pos.x = 1;
    app_info.app_pos.y = 2;
    app_info.app_size = LU_APP_SIZE_SMALL;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);
    app_info.app_pos.x = 2;
    app_info.app_pos.y = 2;
    app_info.app_size = LU_APP_SIZE_SMALL;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);
    app_info.app_pos.x = 3;
    app_info.app_pos.y = 2;
    app_info.app_size = LU_APP_SIZE_SMALL;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);
    app_info.app_pos.x = 4;
    app_info.app_pos.y = 2;
    app_info.app_size = LU_APP_SIZE_MEDIUM;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);

    app_info.app_pos.x = 0;
    app_info.app_pos.y = 3;
    app_info.app_size = LU_APP_SIZE_LARGE;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);
    app_info.app_pos.x = 4;
    app_info.app_pos.y = 4;
    app_info.app_size = LU_APP_SIZE_MEDIUM;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);

    app_info.app_pos.x = 0;
    app_info.app_pos.y = 5;
    app_info.app_size = LU_APP_SIZE_MEDIUM;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);
    app_info.app_pos.x = 2;
    app_info.app_pos.y = 5;
    app_info.app_size = LU_APP_SIZE_MEDIUM;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);
    app_info.app_pos.x = 4;
    app_info.app_pos.y = 6;
    app_info.app_size = LU_APP_SIZE_MEDIUM;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);

    app_info.app_pos.x = 0;
    app_info.app_pos.y = 7;
    app_info.app_size = LU_APP_SIZE_LARGE;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);
    app_info.app_pos.x = 4;
    app_info.app_pos.y = 8;
    app_info.app_size = LU_APP_SIZE_MEDIUM;
    app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
    lu_launcher_set_app_cb(app, app_event_cb);


    lu_ctrbtn_info_t ctrbtn_info =
    {
        .ctrbtn_name = "CTRBTN",
        .ctrbtn_icon = LV_SYMBOL_SETTINGS,
        // .ctrbtn_cb = ctrbtn_event_cb,
    };
    // LV_LOG_USER("ctrbtn_event_cb=%p",ctrbtn_event_cb);
    ctrbtn_info.ctrbtn_pos.x = 0;
    ctrbtn_info.ctrbtn_pos.y = 0;
    ctrbtn_info.ctrbtn_size = LU_CTRBTN_SIZE_LARGE;
    lu_ctrbtn_t* ctrbtn = lu_ctrcent_add_ctrbtn(lu_sysui_get_ctrcent(lvui->sysui), &ctrbtn_info);
    lu_ctrcent_add_ctrbtn_cb(ctrbtn, ctrbtn_event_cb, NULL);
    ctrbtn_info.ctrbtn_pos.x = 2;
    ctrbtn_info.ctrbtn_pos.y = 0;
    ctrbtn_info.ctrbtn_size = LU_CTRBTN_SIZE_SMALL;
    ctrbtn = lu_ctrcent_add_ctrbtn(lu_sysui_get_ctrcent(lvui->sysui), &ctrbtn_info);
    lu_ctrcent_add_ctrbtn_cb(ctrbtn, ctrbtn_event_cb, NULL);
    ctrbtn_info.ctrbtn_pos.x = 3;
    ctrbtn_info.ctrbtn_pos.y = 0;
    ctrbtn_info.ctrbtn_size = LU_CTRBTN_SIZE_SMALL;
    ctrbtn = lu_ctrcent_add_ctrbtn(lu_sysui_get_ctrcent(lvui->sysui), &ctrbtn_info);
    lu_ctrcent_add_ctrbtn_cb(ctrbtn, ctrbtn_event_cb, NULL);

    ctrbtn_info.ctrbtn_pos.x = 0;
    ctrbtn_info.ctrbtn_pos.y = 2;
    ctrbtn_info.ctrbtn_size = LU_CTRBTN_SIZE_SMALL;
    ctrbtn = lu_ctrcent_add_ctrbtn(lu_sysui_get_ctrcent(lvui->sysui), &ctrbtn_info);
    lu_ctrcent_add_ctrbtn_cb(ctrbtn, ctrbtn_event_cb, NULL);
    ctrbtn_info.ctrbtn_pos.x = 1;
    ctrbtn_info.ctrbtn_pos.y = 2;
    ctrbtn_info.ctrbtn_size = LU_CTRBTN_SIZE_SMALL;
    ctrbtn = lu_ctrcent_add_ctrbtn(lu_sysui_get_ctrcent(lvui->sysui), &ctrbtn_info);
    lu_ctrcent_add_ctrbtn_cb(ctrbtn, ctrbtn_event_cb, NULL);

    ctrbtn_info.ctrbtn_pos.x = 2;
    ctrbtn_info.ctrbtn_pos.y = 1;
    ctrbtn_info.ctrbtn_size = LU_CTRBTN_SIZE_MEDIUM;
    ctrbtn = lu_ctrcent_add_ctrbtn(lu_sysui_get_ctrcent(lvui->sysui), &ctrbtn_info);
    lu_ctrcent_add_ctrbtn_cb(ctrbtn, ctrbtn_event_cb, NULL);
    ctrbtn_info.ctrbtn_pos.x = 3;
    ctrbtn_info.ctrbtn_pos.y = 1;
    ctrbtn_info.ctrbtn_size = LU_CTRBTN_SIZE_MEDIUM;
    ctrbtn = lu_ctrcent_add_ctrbtn(lu_sysui_get_ctrcent(lvui->sysui), &ctrbtn_info);
    lu_ctrcent_add_ctrbtn_cb(ctrbtn, ctrbtn_event_cb, NULL);

    ctrbtn_info.ctrbtn_pos.x = 0;
    ctrbtn_info.ctrbtn_pos.y = 3;
    ctrbtn_info.ctrbtn_size = LU_CTRBTN_SIZE_SMALL;
    ctrbtn = lu_ctrcent_add_ctrbtn(lu_sysui_get_ctrcent(lvui->sysui), &ctrbtn_info);
    lu_ctrcent_add_ctrbtn_cb(ctrbtn, ctrbtn_event_cb, NULL);
    ctrbtn_info.ctrbtn_pos.x = 1;
    ctrbtn_info.ctrbtn_pos.y = 3;
    ctrbtn_info.ctrbtn_size = LU_CTRBTN_SIZE_SMALL;
    ctrbtn = lu_ctrcent_add_ctrbtn(lu_sysui_get_ctrcent(lvui->sysui), &ctrbtn_info);
    lu_ctrcent_add_ctrbtn_cb(ctrbtn, ctrbtn_event_cb, NULL);

    ctrbtn_info.ctrbtn_pos.x = 2;
    ctrbtn_info.ctrbtn_pos.y = 3;
    ctrbtn_info.ctrbtn_size = LU_CTRBTN_SIZE_LARGE;
    ctrbtn = lu_ctrcent_add_ctrbtn(lu_sysui_get_ctrcent(lvui->sysui), &ctrbtn_info);
    lu_ctrcent_add_ctrbtn_cb(ctrbtn, ctrbtn_event_cb, NULL);


    // lu_theme_set_mode(lu_sysui_get_theme(lvui->sysui), LU_THEME_MODE_DARK);
    lu_theme_set_mode(lu_sysui_get_theme(lvui->sysui), LU_THEME_MODE_BRIGHT);
    lu_theme_set_color(lu_sysui_get_theme(lvui->sysui), lv_palette_main(LV_PALETTE_YELLOW));





    lu_sysui_set_time(lvui->sysui, time(NULL));
    lu_sysui_add_stat(lvui->sysui, LV_SYMBOL_SETTINGS, LU_SYSUI_STAT_TYPE_DYNAMIC);
    lu_sysui_add_stat(lvui->sysui, LV_SYMBOL_FILE, LU_SYSUI_STAT_TYPE_DYNAMIC);
    lu_sysui_add_stat(lvui->sysui, LV_SYMBOL_WARNING, LU_SYSUI_STAT_TYPE_DYNAMIC);
    lu_sysui_add_stat(lvui->sysui, LV_SYMBOL_BACKSPACE, LU_SYSUI_STAT_TYPE_CONST);
    lu_sysui_add_stat(lvui->sysui, LV_SYMBOL_BLUETOOTH, LU_SYSUI_STAT_TYPE_CONST);




    while(1) {
        static uint64_t last_time = 0;
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_task_handler();

        // while1();

        // lvui_update();
        usleep(500);       /*Just to let the system breath*/
        ++last_time;
        if(last_time%200==0)
        {
            last_time=0;
            lu_sysui_set_time(lvui->sysui, time(NULL));
        }
    }
    return 0;
}
