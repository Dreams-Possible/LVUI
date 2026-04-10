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

#include "lvui/sys/lu_sysui.h"

#include "lvui.h"

lvui_t* lvui_create(int16_t width, int16_t height, float size);
void lvui_delete(lvui_t** lvui);

lvui_t* lvui_create(int16_t width, int16_t height, float size)
{
    lvui_t* lvui = lu_calloc(1, sizeof(lvui_t));
    if(!lvui)
    {
        LU_LOG("lvui_create: lu_calloc failed");
        return NULL;
    }
    lvui->lv_disp = lv_display_get_default();
    if(!lvui->lv_disp)
    {
        lvui_delete(&lvui);
        LU_LOG("lvui_create: lv_display_get_default failed");
        return NULL;
    }
    lvui->lv_indev = lv_indev_get_next(NULL);
    if(!lvui->lv_indev)
    {
        lvui_delete(&lvui);
        LU_LOG("lvui_create: lv_indev_get_next failed");
        return NULL;
    }

    lv_obj_t* disp_obj = lv_display_get_screen_active(lvui->lv_disp);
    if(!disp_obj)
    {
        lvui_delete(&lvui);
        LU_LOG("lvui_create: lv_display_get_screen_active failed");
        return NULL;
    }
    LU_LOG("lvui_create: disp_obj is %p", disp_obj);
    lv_obj_remove_flag(disp_obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_t* contain = lu_widget_obj_init(disp_obj);
    lv_obj_remove_flag(disp_obj, LV_OBJ_FLAG_SCROLLABLE);
    lu_disp_t disp_info={
        .contain = contain,
    };
    if(width&&height&&size)
    {
        lv_obj_set_size(contain, width, height);
        disp_info.dpi = lu_disp_calc_dpi(width, height, size);

    }else
    {
        lv_obj_set_size(contain, LV_PCT(100), LV_PCT(100));
        disp_info.dpi = lu_disp_get_default_dpi();
    }
    if(!contain)
    {
        lvui_delete(&lvui);
        LU_LOG("lvui_create: lu_widget_obj_init failed");
        return NULL;
    }
    LU_LOG("lvui_create: contain is %p", contain);
    lvui->lu_disp = lu_disp_create(&disp_info);
    if(!lvui->lu_disp)
    {
        lvui_delete(&lvui);
        LU_LOG("lvui_create: lu_disp_create failed");
        return NULL;
    }

    LU_LOG("lvui_create: lv_indev is %p", lvui->lv_indev);
    lvui->lu_touch = lu_touch_create(lvui->lv_indev);
    if(!lvui->lu_touch)
    {
        lvui_delete(&lvui);
        LU_LOG("lvui_create: lu_touch_create failed");
        return NULL;
    }
    LU_LOG("lvui_create: lu_touch is %p", lvui->lu_touch);



    // lvui->lv_theme = lv_theme_default_init(lvui->lv_disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, lu_font_get(lu_disp_get_dpi(lvui->lu_disp), LU_FONT_SIZE_NORMAL, 1.0));
    // if(!lvui->lv_theme)
    // {
    //     lvui_delete(&lvui);    
    //     LU_LOG("lvui_create: lv_theme_default_init failed");
    //     return NULL;
    // }

    lvui->sysui = lu_sysui_create(lvui->lu_disp, lvui->lu_touch);
    if(!lvui->sysui)
    {
        lvui_delete(&lvui);
        LU_LOG("lvui_create: lu_sysui_create failed");
        return NULL;
    }
    LU_LOG("lvui_create: success");
    return lvui;
}

void lvui_delete(lvui_t** lvui)
{
    if(!lvui || !*lvui)
    {
        return;
    }
    if((*lvui)->sysui)
    {
        lu_sysui_delete(&(*lvui)->sysui);
    }
    if((*lvui)->lu_disp)
    {
        lu_disp_delete(&(*lvui)->lu_disp);
    }
    if((*lvui)->lu_touch)
    {
        lu_touch_delete(&(*lvui)->lu_touch);
    }
    // if((*lvui)->lv_theme)
    // {
    //     lv_theme_default_deinit();
    //     (*lvui)->lv_theme = NULL;
    // }
    lu_free(*lvui);
    *lvui = NULL;
}
