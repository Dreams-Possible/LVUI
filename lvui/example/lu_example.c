#include "lvui/lvui.h"
#include "lvui/app/lu_app_theme.h"
#include <time.h>

static void app_event_cb(lu_app_t* app)
{
    LU_LOG("app_event_cb: app clicked");
}

static void ctrbtn_event_cb(lu_ctrbtn_t* ctrbtn)
{
    LU_LOG("ctrbtn_event_cb: control button clicked");
}

static void sidebtn_event_cb(lu_ctrbtn_t* sidebtn)
{
    LU_LOG("sidebtn_event_cb:%f", lu_ctrcent_get_ctrbtn_status(sidebtn));
}

lvui_t* lu_example_init(int32_t width, int32_t height, float size)
{
    lvui_t* lvui = lvui_create(width, height, size);
    if(!lvui)
    {
        LU_LOG("lu_example_init: lvui_create failed");
        return NULL;
    }

    /* App container - shared background for all app views */
    lv_obj_t* app_contain = lu_app_theme_create(lvui);
    if(!app_contain)
    {
        LU_LOG("lu_example_init: lu_app_theme_create failed");
        lvui_delete(&lvui);
        return NULL;
    }

    /* Register apps with unique icons and names */
    static const struct { lv_coord_t x, y; lu_app_size_t size; const char* icon; const char* name; } app_defs[] =
    {
        {0, 0, LU_APP_SIZE_SMALL,  LV_SYMBOL_CALL,        "Phone"},
        {0, 1, LU_APP_SIZE_SMALL,  LV_SYMBOL_AUDIO,       "Music"},
        {1, 0, LU_APP_SIZE_LARGE,  LV_SYMBOL_ENVELOPE,    "Mail"},
        {5, 0, LU_APP_SIZE_SMALL,  LV_SYMBOL_BELL,        "Alert"},
        {5, 1, LU_APP_SIZE_SMALL,  LV_SYMBOL_IMAGE,       "Photos"},
        {0, 2, LU_APP_SIZE_SMALL,  LV_SYMBOL_SETTINGS,    "Settings"},
        {1, 2, LU_APP_SIZE_SMALL,  LV_SYMBOL_LIST,        "Notes"},
        {2, 2, LU_APP_SIZE_SMALL,  LV_SYMBOL_KEYBOARD,    "Input"},
        {3, 2, LU_APP_SIZE_SMALL,  LV_SYMBOL_GPS,         "Maps"},
        {4, 2, LU_APP_SIZE_MEDIUM, LV_SYMBOL_FILE,        "Files"},
        {0, 3, LU_APP_SIZE_LARGE,  LV_SYMBOL_VIDEO,       "Video"},
        {4, 4, LU_APP_SIZE_MEDIUM, LV_SYMBOL_SAVE,        "Save"},
        {0, 5, LU_APP_SIZE_MEDIUM, LV_SYMBOL_REFRESH,     "Sync"},
        {2, 5, LU_APP_SIZE_MEDIUM, LV_SYMBOL_LOOP,        "Tasks"},
        {4, 6, LU_APP_SIZE_MEDIUM, LV_SYMBOL_DOWNLOAD,    "Cloud"},
        {0, 7, LU_APP_SIZE_LARGE,  LV_SYMBOL_CLOSE,       "Apps"},
        {4, 8, LU_APP_SIZE_MEDIUM, LV_SYMBOL_PLAY,        "Media"},
    };
    int app_count = sizeof(app_defs) / sizeof(app_defs[0]);
    lu_app_info_t app_info = { .app_contain = app_contain };
    for(int i = 0; i < app_count; i++)
    {
        app_info.app_pos.x = app_defs[i].x;
        app_info.app_pos.y = app_defs[i].y;
        app_info.app_size = app_defs[i].size;
        app_info.app_icon = app_defs[i].icon;
        app_info.app_name = app_defs[i].name;
        lu_app_t* app = lu_launcher_add_app(lu_sysui_get_launcher(lvui->sysui), &app_info);
        if(!app)
        {
            LU_LOG("lu_example_init: lu_launcher_add_app[%d] failed", i);
            lvui_delete(&lvui);
            return NULL;
        }
        lu_launcher_set_app_cb(app, app_event_cb);
    }

    /* Register control center buttons with unique icons and names */
    static const struct { lv_coord_t x, y; lu_ctrbtn_size_t size; const char* icon; const char* name; } ctrbtn_defs[] =
    {
        {0, 0, LU_CTRBTN_SIZE_LARGE,  LV_SYMBOL_WIFI,        "WiFi"},
        {2, 0, LU_CTRBTN_SIZE_SMALL,  LV_SYMBOL_BLUETOOTH,   "BT"},
        {3, 0, LU_CTRBTN_SIZE_SMALL,  LV_SYMBOL_AUDIO,       "Music"},
        {0, 2, LU_CTRBTN_SIZE_SMALL,  LV_SYMBOL_GPS,         "GPS"},
        {1, 2, LU_CTRBTN_SIZE_SMALL,  LV_SYMBOL_BATTERY_FULL,"Battery"},
        {2, 1, LU_CTRBTN_SIZE_MEDIUM, LV_SYMBOL_VOLUME_MID,  "Volume"},
        {3, 1, LU_CTRBTN_SIZE_MEDIUM, LV_SYMBOL_EYE_OPEN,   "Bright"},
        {0, 3, LU_CTRBTN_SIZE_SMALL,  LV_SYMBOL_TINT,        "Theme"},
        {1, 3, LU_CTRBTN_SIZE_SMALL,  LV_SYMBOL_BELL,        "Bell"},
        {2, 3, LU_CTRBTN_SIZE_LARGE,  LV_SYMBOL_SHUFFLE,     "Random"},
    };
    int ctrbtn_count = sizeof(ctrbtn_defs) / sizeof(ctrbtn_defs[0]);
    lu_ctrbtn_info_t ctrbtn_info = {};
    for(int i = 0; i < ctrbtn_count; i++)
    {
        ctrbtn_info.ctrbtn_pos.x = ctrbtn_defs[i].x;
        ctrbtn_info.ctrbtn_pos.y = ctrbtn_defs[i].y;
        ctrbtn_info.ctrbtn_size = ctrbtn_defs[i].size;
        ctrbtn_info.ctrbtn_icon = ctrbtn_defs[i].icon;
        ctrbtn_info.ctrbtn_name = ctrbtn_defs[i].name;
        lu_ctrbtn_t* ctrbtn = lu_ctrcent_add_ctrbtn(lu_sysui_get_ctrcent(lvui->sysui), &ctrbtn_info);
        if(!ctrbtn)
        {
            LU_LOG("lu_example_init: lu_ctrcent_add_ctrbtn[%d] failed", i);
            lvui_delete(&lvui);
            return NULL;
        }
        if(ctrbtn_defs[i].size == LU_CTRBTN_SIZE_MEDIUM)
        {
            lu_ctrcent_add_ctrbtn_cb(ctrbtn, sidebtn_event_cb, NULL);
        }else
        {
            lu_ctrcent_add_ctrbtn_cb(ctrbtn, ctrbtn_event_cb, NULL);
        }
    }

    /* Theme */
    lu_theme_set_mode(lu_sysui_get_theme(lvui->sysui), LU_THEME_MODE_BRIGHT);
    lu_theme_set_color(lu_sysui_get_theme(lvui->sysui), lv_color_white());

    /* System time and status bar icons */
    lu_sysui_set_time(lvui->sysui, time(NULL));
    lu_sysui_add_stat(lvui->sysui, LV_SYMBOL_WIFI,      LU_SYSUI_STAT_TYPE_DYNAMIC);
    lu_sysui_add_stat(lvui->sysui, LV_SYMBOL_BLUETOOTH, LU_SYSUI_STAT_TYPE_DYNAMIC);
    lu_sysui_add_stat(lvui->sysui, LV_SYMBOL_GPS,       LU_SYSUI_STAT_TYPE_DYNAMIC);
    lu_sysui_add_stat(lvui->sysui, LV_SYMBOL_CHARGE,    LU_SYSUI_STAT_TYPE_CONST);
    lu_sysui_add_stat(lvui->sysui, LV_SYMBOL_BELL,      LU_SYSUI_STAT_TYPE_CONST);

    /* Timezone */
    lu_sysui_set_timezone(lvui->sysui, 8);

    /* Wallpaper */
    lu_locker_set_wallpaper(lu_sysui_get_locker(lvui->sysui), "C:/Users/Windows/Desktop/project/lv_port_win_codeblocks-master_v2/src/1.bmp");
    lu_launcher_set_wallpaper(lu_sysui_get_launcher(lvui->sysui), "C:/Users/Windows/Desktop/project/lv_port_win_codeblocks-master_v2/src/1.bmp");

    return lvui;
}
