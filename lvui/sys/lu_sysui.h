#ifndef LU_SYSUI_H
#define LU_SYSUI_H

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

typedef enum lu_sysui_stat_type_t
{
    LU_SYSUI_STAT_TYPE_CONST = 0,
    LU_SYSUI_STAT_TYPE_DYNAMIC = 1,
}lu_sysui_stat_type_t;
typedef struct lu_sysui_t lu_sysui_t;

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

#endif
