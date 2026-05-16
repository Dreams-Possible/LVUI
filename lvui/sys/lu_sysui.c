#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/plat/lu_mem.h"

#include "lvui/misc/lu_log.h"
#include "lvui/misc/lu_pdc.h"
#include "lvui/misc/lu_sll.h"

#include "lvui/frame/anim/lu_anim.h"
#include "lvui/frame/disp/lu_disp.h"
#include "lvui/frame/disp/lu_font.h"
#include "lvui/frame/input/lu_touch.h"
#include "lvui/frame/theme/lu_color.h"
#include "lvui/frame/theme/lu_theme.h"
#include "lvui/frame/widget/lu_widget.h"
#include "lvui/frame/widget/lu_layout.h"

#include "lvui/view/lu_desktop.h"
#include "lvui/view/lu_gesture.h"
#include "lvui/view/lu_ctrbar.h"
#include "lvui/view/lu_locker.h"

#include "lu_ctrcent.h"
#include "lu_launcher.h"

#include "lu_sysui.h"
#include "lu_sysinfo.h"

#include <stdio.h>

typedef struct lu_sysui_t
{
    // lv_disp_t* lv_disp;
    // lv_indev_t* lv_touch;
    // lv_theme_t* lv_theme;
    lu_disp_t* lu_disp;
    lu_touch_t* lu_touch;
    lu_theme_t* lu_theme;
    // lv_obj_t* contain;
    lu_desktop_t* desktop;
    lu_gesture_t* gesture;
    lu_ctrbar_t* ctrbar;
    lu_locker_t* locker;
    lu_ctrcent_t* ctrcent;
    lu_launcher_t* launcher;
    lu_sysinfo_t* sysinfo;

    lv_obj_t* locker_time;
    lv_obj_t* locker_date;
    lv_obj_t* statbar_time;
}lu_sysui_t;

static void launcher_cb(lu_launcher_t* launcher);
static void gesture_cb(lu_gesture_t* gesture);
static bool sys_ui_init(lu_sysui_t* sysui);
static void sys_logic_init(lu_sysui_t* sysui);
lu_sysui_t* lu_sysui_create(lu_disp_t* lu_disp, lu_touch_t* lu_touch);
void lu_sysui_delete(lu_sysui_t** sysui);
lu_theme_t* lu_sysui_get_theme(lu_sysui_t* sysui);
lu_gesture_t* lu_sysui_get_gesture(lu_sysui_t* sysui);
lu_locker_t* lu_sysui_get_locker(lu_sysui_t* sysui);
lu_ctrcent_t* lu_sysui_get_ctrcent(lu_sysui_t* sysui);
lu_launcher_t* lu_sysui_get_launcher(lu_sysui_t* sysui);
void lu_sysui_add_stat(lu_sysui_t* sysui, const char* icon, lu_sysui_stat_type_t stat_type);
void lu_sysui_set_time(lu_sysui_t* sysui, uint64_t time);
void lu_sysui_set_timezone(lu_sysui_t* sysui, int8_t timezone);
int8_t lu_sysui_get_timezone(lu_sysui_t* sysui);
int32_t lu_sysui_get_statbar_height(lu_sysui_t* sysui);

static void launcher_cb(lu_launcher_t* launcher)
{
    // lu_sysui_t* sysui = (lu_sysui_t*)lu_launcher_get_user_data(launcher, LU_LAUNCHER_SWIPE_DOWN);
    lu_sysui_t* sysui = (lu_sysui_t*)lu_launcher_get_user_data(launcher, LU_LAUNCHER_DOUBLE_CLICK);
    // lu_sysui_t* sysui = (lu_sysui_t*)lu_launcher_get_user_data(launcher, LU_LAUNCHER_LONG_PRESS);
    lu_locker_enter(sysui->locker);
}

static void gesture_cb(lu_gesture_t* gesture)
{
    lu_sysui_t* sysui = (lu_sysui_t*)lu_gesture_get_user_data(gesture, LU_GESTURE_SWIPE_UP);
    // LU_LOG("gesture_cb: swipe up: %p", sysui);
    // LU_LOG("gesture_cb: swipe up: %p", sysui->launcher);
    lu_launcher_close_all(sysui->launcher);
    // LU_LOG("gesture_cb: double click");
}

static bool sys_ui_init(lu_sysui_t* sysui)
{
    lu_locker_info_txt_t info = {0};
    info.txt = "TIME",
    // info.font = lu_font_get(sysui->lu_disp->dpi, LU_FONT_SIZE_MAX, 1.0);
    info.font = lu_font_get_auto(sysui->lu_disp,LU_FONT_AUTO_SIZE_LARGE);
    sysui->locker_time = lu_locker_add_info_txt(sysui->locker, &info);
    if(!sysui->locker_time)
    {
        LU_LOG("sys_ui_init: lu_locker_add_info_txt failed");
        return false;
    }
    info.txt = "DATE",
    // info.font = lu_font_get(sysui->lu_disp->dpi, LU_FONT_SIZE_HUGE, 1.0);
    info.font = lu_font_get_auto(sysui->lu_disp,LU_FONT_AUTO_SIZE_BIG);
    sysui->locker_date = lu_locker_add_info_txt(sysui->locker, &info);
    if(!sysui->locker_date)
    {
        LU_LOG("sys_ui_init: lu_locker_add_info_txt failed");
        return false;
    }
    lu_statbar_info_t statbar_info=
    {
        .icon = "TIME",
        .type = LU_STATBAR_TYPE_RIGHT,
    };
    sysui->statbar_time = lu_statbar_add_icon(lu_ctrbar_get_statbar(sysui->ctrbar), &statbar_info);
    if(!sysui->statbar_time)
    {
        LU_LOG("sys_ui_init: lu_statbar_add_icon failed");
        return false;
    }
    return true;
}

static void sys_logic_init(lu_sysui_t* sysui)
{
    lu_launcher_set_cb(sysui->launcher, LU_LAUNCHER_DOUBLE_CLICK, launcher_cb, sysui);
    lu_gesture_set_cb(sysui->gesture, LU_GESTURE_SWIPE_UP, gesture_cb, sysui);
}

lu_sysui_t* lu_sysui_create(lu_disp_t* lu_disp, lu_touch_t* lu_touch)
{
    if(!lu_disp||!lu_touch)
    {
        LU_LOG("lu_sysui_create: lu_disp, lu_touch is NULL");
        return NULL;
    }
    LU_LOG("lu_sysui_create: start");
    lu_sysui_t* sysui = (lu_sysui_t*)lu_calloc(1, sizeof(lu_sysui_t));
    if(!sysui)
    {
        LU_LOG("lu_sysui_create: sysui is NULL");
        return NULL;
    }
    sysui->lu_disp = lu_disp;
    sysui->lu_touch = lu_touch;
    
    sysui->lu_theme = lu_theme_create();
    if(!sysui->lu_theme)
    {
        lu_sysui_delete(&sysui);
        LU_LOG("lu_sysui_create: lu_theme_create failed");
        return NULL;
    }
    LU_LOG("lu_sysui_create: theme is %p", sysui->lu_theme);

    sysui->desktop = lu_desktop_create(lu_disp, sysui->lu_theme, sysui->lu_touch, sysui->lu_disp->contain);
    if(!sysui->desktop)
    {
        lu_sysui_delete(&sysui);
        LU_LOG("lu_sysui_create: lu_desktop_create failed");
        return NULL;
    }
    LU_LOG("lu_sysui_create: desktop is %p", sysui->desktop);
    sysui->gesture = lu_gesture_create(lu_disp, sysui->lu_theme, sysui->lu_touch, sysui->lu_disp->contain);
    if(!sysui->gesture)
    {
        lu_sysui_delete(&sysui);
        LU_LOG("lu_sysui_create: lu_gesture_create failed");
        return NULL;
    }
    LU_LOG("lu_sysui_create: gesture is %p", sysui->gesture);
    sysui->ctrbar = lu_ctrbar_create(lu_disp, sysui->lu_theme, sysui->lu_touch, sysui->lu_disp->contain);
    if(!sysui->ctrbar)
    {
        lu_sysui_delete(&sysui);
        LU_LOG("lu_sysui_create: lu_ctrbar_create failed");
        return NULL;
    }
    LU_LOG("lu_sysui_create: ctrbar is %p", sysui->ctrbar);
    sysui->locker = lu_locker_create(lu_disp, sysui->lu_theme, sysui->lu_touch, sysui->lu_disp->contain);
    if(!sysui->locker)
    {
        lu_sysui_delete(&sysui);
        LU_LOG("lu_sysui_create: lu_locker_create failed");
        return NULL;
    }
    LU_LOG("lu_sysui_create: locker is %p", sysui->locker);
    lu_launcher_info_t launcher_info=
    {
        .lu_disp = sysui->lu_disp,
        .lu_theme = sysui->lu_theme,
        .lu_touch = sysui->lu_touch,
        .parent = lu_desktop_get_contain(sysui->desktop),
        .desktop = sysui->desktop,
        .ctrbar = sysui->ctrbar,
        .gesture = sysui->gesture,
        .locker = sysui->locker,
    };
    sysui->launcher = lu_launcher_create(&launcher_info);
    if(!sysui->launcher)
    {
        lu_sysui_delete(&sysui);
        LU_LOG("lu_sysui_create: lu_launcher_create failed");
        return NULL;
    }
    LU_LOG("lu_sysui_create: launcher is %p", sysui->launcher);
    lu_ctrcent_info_t ctrcent_info=
    {
        .lu_disp = sysui->lu_disp,
        .lu_theme = sysui->lu_theme,
        .lu_touch = sysui->lu_touch,
        .parent = lu_ctrbar_get_contain(sysui->ctrbar),
        .desktop = sysui->desktop,
        .ctrbar = sysui->ctrbar,
        .gesture = sysui->gesture,
        .locker = sysui->locker,
    };
    sysui->ctrcent = lu_ctrcent_create(&ctrcent_info);
    if(!sysui->ctrcent)
    {
        lu_sysui_delete(&sysui);
        LU_LOG("lu_sysui_create: lu_ctrcent_create failed");
        return NULL;
    }
    sysui->sysinfo = lu_sysinfo_create();
    if(!sysui->sysinfo)
    {
        lu_sysui_delete(&sysui);
        LU_LOG("lu_sysui_create: lu_sysinfo_create failed");
        return NULL;
    }
    LU_LOG("lu_sysui_create: sysinfo is %p", sysui->sysinfo);
    if(!sys_ui_init(sysui))
    {
        lu_sysui_delete(&sysui);
        LU_LOG("lu_sysui_create: sys_ui_init failed");
        return NULL;
    }
    sys_logic_init(sysui);
    LU_LOG("lu_sysui_create: success");
    return sysui;
}

void lu_sysui_delete(lu_sysui_t** sysui)
{
    if(!sysui || !*sysui)
    {
        return;
    }
    if((*sysui)->sysinfo)
    {
        lu_sysinfo_delete(&(*sysui)->sysinfo);
        (*sysui)->sysinfo = NULL;
    }
    if((*sysui)->ctrcent)
    {
        lu_ctrcent_delete(&(*sysui)->ctrcent);
        (*sysui)->ctrcent = NULL;
    }
    if((*sysui)->launcher)
    {
        lu_launcher_delete(&(*sysui)->launcher);
        (*sysui)->launcher = NULL;
    }
    if((*sysui)->locker)
    {
        lu_locker_delete(&(*sysui)->locker);
        (*sysui)->locker = NULL;
    }
    if((*sysui)->ctrbar)
    {
        lu_ctrbar_delete(&(*sysui)->ctrbar);
        (*sysui)->ctrbar = NULL;
    }
    if((*sysui)->gesture)
    {
        lu_gesture_delete(&(*sysui)->gesture);
        (*sysui)->gesture = NULL;
    }
    // if((*sysui)->statbar)
    // {
    //     lu_statbar_delete(&(*sysui)->statbar);
    //     (*sysui)->statbar = NULL;
    // }
    if((*sysui)->desktop)
    {
        lu_desktop_delete(&(*sysui)->desktop);
        (*sysui)->desktop = NULL;
    }
    // if((*sysui)->contain)
    // {
    //     lv_obj_delete((*sysui)->contain);
    //     (*sysui)->contain = NULL;
    // }
    if((*sysui)->lu_theme)
    {
        lv_theme_default_deinit();
        (*sysui)->lu_theme = NULL;
    }
    lu_free(*sysui);
    *sysui = NULL;
}

lu_theme_t* lu_sysui_get_theme(lu_sysui_t* sysui)
{
    return sysui->lu_theme;
}

lu_gesture_t* lu_sysui_get_gesture(lu_sysui_t* sysui)
{
    return sysui->gesture;
}

lu_locker_t* lu_sysui_get_locker(lu_sysui_t* sysui)
{
    return sysui->locker;
}

lu_ctrcent_t* lu_sysui_get_ctrcent(lu_sysui_t* sysui)
{
    return sysui->ctrcent;
}

lu_launcher_t* lu_sysui_get_launcher(lu_sysui_t* sysui)
{
    return sysui->launcher;
}

void lu_sysui_add_stat(lu_sysui_t* sysui, const char* icon, lu_sysui_stat_type_t stat_type)
{
    if(!sysui || !icon)
    {
        return;
    }
    lu_statbar_info_t statbar_info=
    {
        .icon = icon,
    };
    switch (stat_type)
    {
        case LU_SYSUI_STAT_TYPE_CONST:
            statbar_info.type = LU_STATBAR_TYPE_RIGHT;
        break;
        case LU_SYSUI_STAT_TYPE_DYNAMIC:
            statbar_info.type = LU_STATBAR_TYPE_LEFT;
        break;
        default:
        break;
    }
    lu_statbar_add_icon(lu_ctrbar_get_statbar(sysui->ctrbar), &statbar_info);
    lu_locker_info_icon_t locker_info_icon=
    {
        .icon = icon,
    };
    switch (stat_type)
    {
        case LU_SYSUI_STAT_TYPE_CONST:
            locker_info_icon.type = LU_LOCKER_ICON_TYPE_RIGHT;
        break;
        case LU_SYSUI_STAT_TYPE_DYNAMIC:
            locker_info_icon.type = LU_LOCKER_ICON_TYPE_LEFT;
        break;
        default:
        break;
    }
    lu_locker_add_info_icon(sysui->locker, &locker_info_icon);
}

void lu_sysui_set_time(lu_sysui_t* sysui, uint64_t time)
{
    if(!sysui || !time)
    {
        return;
    }
    lu_sysinfo_set_time(sysui->sysinfo, time);
    char time_str[16] = {0};
    snprintf(time_str, sizeof(time_str), "%s:%s:%s", lu_sysinfo_get_hour_str(sysui->sysinfo), lu_sysinfo_get_minute_str(sysui->sysinfo), lu_sysinfo_get_second_str(sysui->sysinfo));
    char date_str[16] = {0};
    snprintf(date_str, sizeof(date_str), "%s %s %s", lu_sysinfo_get_year_str(sysui->sysinfo), lu_sysinfo_get_month_str_full(sysui->sysinfo), lu_sysinfo_get_day_str(sysui->sysinfo));
    lv_label_set_text(sysui->locker_time, time_str);
    lv_label_set_text(sysui->locker_date, date_str);
    lv_label_set_text(sysui->statbar_time, time_str);
    // lu_statbar_set_time(lu_ctrbar_get_statbar(sysui->ctrbar), lu_sysinfo_get_hour_str(sysui->sysinfo), lu_sysinfo_get_minute_str(sysui->sysinfo), lu_sysinfo_get_second_str(sysui->sysinfo));
    // lu_statbar_set_time(lu_ctrbar_get_statbar(sysui->ctrbar), lu_sysinfo_get_hour_str(sysui->sysinfo), lu_sysinfo_get_minute_str(sysui->sysinfo), "");
    // lu_locker_set_time(sysui->locker, lu_sysinfo_get_hour_str(sysui->sysinfo), lu_sysinfo_get_minute_str(sysui->sysinfo), lu_sysinfo_get_second_str(sysui->sysinfo));
    // lu_locker_set_time(sysui->locker, lu_sysinfo_get_hour_str(sysui->sysinfo), lu_sysinfo_get_minute_str(sysui->sysinfo), "");
}

void lu_sysui_set_timezone(lu_sysui_t* sysui, int8_t timezone)
{
    if(!sysui)
    {
        return;
    }
    lu_sysinfo_set_timezone(sysui->sysinfo, timezone);
}

int8_t lu_sysui_get_timezone(lu_sysui_t* sysui)
{
    if(!sysui)
    {
        return 0;
    }
    return lu_sysinfo_get_timezone(sysui->sysinfo);
}

int32_t lu_sysui_get_statbar_height(lu_sysui_t* sysui)
{
    if(!sysui)
    {
        return 0;
    }
    return lu_statbar_get_height(lu_ctrbar_get_statbar(sysui->ctrbar));
}
