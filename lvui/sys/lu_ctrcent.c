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
// #include "lvui/view/lu_statbar.h"
#include "lvui/view/lu_gesture.h"
#include "lvui/view/lu_ctrbar.h"
#include "lvui/view/lu_locker.h"

#include "lu_ctrcent.h"

#include <string.h>

typedef struct lu_ctrbtn_t
{
    lu_ctrcent_t* ctrcent;
    lu_ctrbtn_info_t ctrbtn_info;
    // lv_obj_t* ctrbtn_contain;
    lv_obj_t* ctrbtn_item;
    lu_theme_obj_t* ctrbtn_item_theme_obj;
    lv_obj_t* ctrbtn_item_mask;
    lu_theme_obj_t* ctrbtn_item_mask_theme_obj;
    lu_ctrbtn_cb_t ctrbtn_cb;
    void* ctrbtn_user_data;
    float ctrbtn_status;
}lu_ctrbtn_t;
typedef enum ctrcent_status_t
{
    CTRCENT_STATUS_CLOSED = 0,
    CTRCENT_STATUS_OPENING,
    CTRCENT_STATUS_OPENED,
    CTRCENT_STATUS_CLOSING,
} ctrcent_status_t;

typedef struct lu_ctrcent_t
{
    lu_disp_t* lu_disp;
    lu_theme_t* lu_theme;
    lu_touch_t* lu_touch;
    lv_obj_t* parent;
    lu_desktop_t* desktop;
    lu_ctrbar_t* ctrbar;
    lu_gesture_t* gesture;
    lu_locker_t* locker;

    lv_obj_t* contain;
    lv_obj_t* bg;
    lv_obj_t* ctrbtn_area_contain;
    lv_obj_t* ctrbtn_area;
    lu_layout_t* ctrbtn_layout;
    int32_t ctrbtn_gap;
    int32_t ctrbtn_square;
    lv_obj_t* ctrbar_mask;
    lu_sll_t* ctrbtn_list;

    lu_anim_t* statbar_anim;
    lu_anim_t* ctrbtn_area_contain_anim;
    lu_anim_t* bg_anim;
    lu_anim_t* contain_anim;
    lv_timer_t* timer;

    ctrcent_status_t status;
}lu_ctrcent_t;

static void ctrbtn_slider_event_cb(lv_event_t *event);
static void ctrbtn_switch_event_cb(lv_event_t *event);
static void contain_event_cb(lv_event_t *event);
static void ctrbtn_area_contain_event_cb(lv_event_t *event);
static void ctrcent_timer_cb(lv_timer_t* timer);
lu_ctrcent_t* lu_ctrcent_create(const lu_ctrcent_info_t* ctrcent_info);
void lu_ctrcent_delete(lu_ctrcent_t** ctrcent);
static void ctrbtn_event_cb(lv_event_t *event);
void lu_ctrcent_add_ctrbtn_cb(lu_ctrbtn_t* ctrbtn, lu_ctrbtn_cb_t cb, void* user_data);
void* lu_ctrcent_get_ctrbtn_user_data(lu_ctrbtn_t* ctrbtn);
lu_ctrbtn_t* lu_ctrcent_add_ctrbtn(lu_ctrcent_t* ctrcent, const lu_ctrbtn_info_t* ctrbtn_info);
void lu_ctrcent_remove_ctrbtn(lu_ctrcent_t* ctrcent, lu_ctrbtn_t** ctrbtn);
static void ctrbtn_status_update(lu_ctrbtn_t* ctrbtn);
float lu_ctrcent_get_ctrbtn_status(lu_ctrbtn_t* ctrbtn);
void lu_ctrcent_set_ctrbtn_status(lu_ctrbtn_t* ctrbtn, float status);

static void ctrbtn_slider_event_cb(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lu_ctrbtn_t* ctrbtn = (lu_ctrbtn_t*)lv_event_get_user_data(event);
    if(code == LV_EVENT_VALUE_CHANGED)
    {
        ctrbtn->ctrbtn_status = (float)lv_slider_get_value(ctrbtn->ctrbtn_item);
        ctrbtn_event_cb(event);
        // LU_LOG("ctrbtn_slider_event_cb: %f", ctrbtn->ctrbtn_status);
    }
}

static void ctrbtn_switch_event_cb(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lu_ctrbtn_t* ctrbtn = (lu_ctrbtn_t*)lv_event_get_user_data(event);
    if(code == LV_EVENT_CLICKED)
    {
        ctrbtn->ctrbtn_status = !ctrbtn->ctrbtn_status;
        ctrbtn_status_update(ctrbtn);
        ctrbtn_event_cb(event);
        // LU_LOG("ctrbtn_switch_event_cb: %f", ctrbtn->ctrbtn_status);
    }
}

static void contain_event_cb(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lu_ctrcent_t* ctrcent = (lu_ctrcent_t*)lv_event_get_user_data(event);
    switch(code)
    {
        case LV_EVENT_PRESSED:
            switch (ctrcent->status)
            {
                case CTRCENT_STATUS_CLOSED:
                    ctrcent->status = CTRCENT_STATUS_OPENING;
                break;
                case CTRCENT_STATUS_OPENED:
                    ctrcent->status = CTRCENT_STATUS_CLOSING;
                break;
                default:
                break;
            }
        break;
        case LV_EVENT_RELEASED:
            switch (ctrcent->status)
            {
                case CTRCENT_STATUS_OPENING:
                    // ctrcent->status = CTRCENT_STATUS_OPENING;
                    if(lu_touch_get_moved_y(ctrcent->lu_touch)>lu_disp_get_ver(ctrcent->lu_disp)*0.3)
                    {
                        ctrcent->status = CTRCENT_STATUS_OPENED;
                    }else
                    {
                        ctrcent->status = CTRCENT_STATUS_CLOSED;
                    }
                break;
                case CTRCENT_STATUS_CLOSING:
                    if(lu_touch_get_moved_y(ctrcent->lu_touch)<-lu_disp_get_ver(ctrcent->lu_disp)*0.3)
                    {
                        ctrcent->status = CTRCENT_STATUS_CLOSED;
                    }else
                    {
                        ctrcent->status = CTRCENT_STATUS_OPENED;
                    }
                break;
                default:
                break;
            }
        break;
        default:
        break;
    }
    // LU_LOG("contain_event_cb: %d", lu_anim_get_value(ctrcent->contain_anim));
}

static void ctrbtn_area_contain_event_cb(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lu_ctrcent_t* ctrcent = (lu_ctrcent_t*)lv_event_get_user_data(event);
    switch(code)
    {
        case LV_EVENT_PRESSED:
            // ctrcent->is_close_activate = true;
            switch (ctrcent->status)
            {
                case CTRCENT_STATUS_OPENED:
                    ctrcent->status = CTRCENT_STATUS_CLOSING;
                break;
                default:
                break;
            }
        break;
        case LV_EVENT_RELEASED:
            switch (ctrcent->status)
            {
                case CTRCENT_STATUS_CLOSING:
                    // ctrcent->is_close_activate = false;
                    // LU_LOG("VER= %d", lu_disp_get_ver(ctrcent->lu_disp));
                    // LU_LOG("moved:%d,ver:%f", lu_touch_get_moved_y(ctrcent->lu_touch),-lu_disp_get_ver(ctrcent->lu_disp)*0.3);
                    if(lu_touch_get_moved_y(ctrcent->lu_touch)<-lu_disp_get_ver(ctrcent->lu_disp)*0.3)
                    {
                        ctrcent->status = CTRCENT_STATUS_CLOSED;
                        // LU_LOG("CLOSED");
                    }else
                    {
                        ctrcent->status = CTRCENT_STATUS_OPENED;
                        // LU_LOG("OPENED");
                    }
                break;
                default:
                break;
            }
        break;
        default:
        break;
    }
}

static void ctrcent_timer_cb(lv_timer_t* timer)
{
    lu_ctrcent_t* ctrcent = (lu_ctrcent_t*)lv_timer_get_user_data(timer);
    if(lu_locker_get_status(ctrcent->locker))
    {
        return;
    }
    switch (ctrcent->status)
    {
        case CTRCENT_STATUS_CLOSED:
            lu_anim_set_value(ctrcent->contain_anim, lu_ctrbar_get_height(ctrcent->ctrbar));
            lu_anim_set_value(ctrcent->bg_anim, LV_OPA_0);
            lu_anim_set_value(ctrcent->statbar_anim, 0.0);
            lu_anim_set_value(ctrcent->ctrbtn_area_contain_anim, lu_ctrbar_get_height(ctrcent->ctrbar));
        break;
        case CTRCENT_STATUS_CLOSING:
        {
            int32_t value = lu_disp_get_ver(ctrcent->lu_disp)+lu_touch_get_moved_y(ctrcent->lu_touch);
            lu_anim_set_value(ctrcent->contain_anim, value);
            if(value<0)
            {
                lu_anim_set_value(ctrcent->bg_anim, LV_OPA_70*value/lu_disp_get_ver(ctrcent->lu_disp));
            }
        }
        break;
        case CTRCENT_STATUS_OPENING:
        {
            int32_t value = lu_touch_get_now_y(ctrcent->lu_touch);
            lu_anim_set_value(ctrcent->contain_anim, value);
            if(value>0)
            {
                lu_anim_set_value(ctrcent->bg_anim, LV_OPA_70*value/lu_disp_get_ver(ctrcent->lu_disp));
            }
            lu_anim_set_value(ctrcent->statbar_anim, value*0.2);
            lu_anim_set_value(ctrcent->ctrbtn_area_contain_anim, lu_ctrbar_get_height(ctrcent->ctrbar)+value*0.2);
        }
        break;
        case CTRCENT_STATUS_OPENED:
            lu_anim_set_value(ctrcent->contain_anim, lu_disp_get_ver(ctrcent->lu_disp));
            lu_anim_set_value(ctrcent->bg_anim, LV_OPA_70);
            lu_anim_set_value(ctrcent->statbar_anim, 0);
            lu_anim_set_value(ctrcent->ctrbtn_area_contain_anim, lu_ctrbar_get_height(ctrcent->ctrbar));
        break;
        default:
        break;
    }
}

lu_ctrcent_t* lu_ctrcent_create(const lu_ctrcent_info_t* ctrcent_info)
{
    if(!ctrcent_info)
    {
        return NULL;
    }
    lu_ctrcent_t* ctrcent = (lu_ctrcent_t*)lu_calloc(1, sizeof(lu_ctrcent_t));
    if(!ctrcent)
    {
        return NULL;
    }
    ctrcent->lu_disp = ctrcent_info->lu_disp;
    ctrcent->lu_theme = ctrcent_info->lu_theme;
    ctrcent->lu_touch = ctrcent_info->lu_touch;
    ctrcent->parent = ctrcent_info->parent;
    ctrcent->desktop = ctrcent_info->desktop;
    ctrcent->ctrbar = ctrcent_info->ctrbar;
    ctrcent->gesture = ctrcent_info->gesture;
    ctrcent->locker = ctrcent_info->locker;
    lv_obj_t* obj = lu_widget_obj_init(lu_ctrbar_get_contain(ctrcent->ctrbar));
    if(!obj)
    {
        lu_ctrcent_delete(&ctrcent);
        return NULL;
    }
    ctrcent->contain = obj;
    // lv_obj_set_pos(obj, 0, lu_ctrbar_get_height(ctrcent->ctrbar));
    lv_obj_set_size(obj, LV_PCT(100), lu_ctrbar_get_height(ctrcent->ctrbar));

    // lv_obj_set_style_bg_opa(obj, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    // lu_theme_add_obj(ctrcent->lu_theme, obj, LU_THEME_TYPE_INVMODE);
    {
        {
            lv_obj_t* obj = lu_widget_obj_init(ctrcent->contain);
            if(!obj)
            {
                lu_ctrcent_delete(&ctrcent);
                return NULL;
            }
            ctrcent->bg = obj;
            // lv_obj_set_pos(obj, 0, lu_statbar_get_height(ctrcent->statbar));
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            // lv_obj_set_style_bg_opa(obj, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lu_theme_info_t theme_info =
            {
                .self_type = LU_THEME_TYPE_INVMODE,
                .flag = LU_THEME_FLAG_SELF,
            };
            lu_theme_add_obj(ctrcent->lu_theme, obj, &theme_info);
        }
        {
            lv_obj_t* obj = lu_widget_obj_init(ctrcent->contain);
            if(!obj)
            {
                lu_ctrcent_delete(&ctrcent);
                return NULL;
            }
            ctrcent->ctrbtn_area_contain = obj;
            lv_obj_set_pos(obj, 0, lu_ctrbar_get_height(ctrcent->ctrbar));
            lv_obj_set_size(obj, LV_PCT(100), lu_disp_get_ver(ctrcent->lu_disp)-lu_ctrbar_get_height(ctrcent->ctrbar));
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_add_event_cb(obj, ctrbtn_area_contain_event_cb, LV_EVENT_PRESSED, ctrcent);
            lv_obj_add_event_cb(obj, ctrbtn_area_contain_event_cb, LV_EVENT_RELEASED, ctrcent);
            {
                lv_obj_t* obj = lu_widget_obj_init(ctrcent->ctrbtn_area_contain);
                if(!obj)
                {
                    lu_ctrcent_delete(&ctrcent);
                    return NULL;
                }
                ctrcent->ctrbtn_area = obj;
                {
                    const int32_t ratio = 8;
                    const int32_t num = 4;
                    ctrcent->ctrbtn_gap = lu_disp_get_hor(ctrcent->lu_disp) / (ratio * num + num + 1);
                    ctrcent->ctrbtn_square = (lu_disp_get_hor(ctrcent->lu_disp) - ctrcent->ctrbtn_gap * (num + 1)) / num;
                    // LU_LOG("lu_ctrcent_create: ctrbtn_gap is %d, ctrbtn_square is %d", ctrcent->ctrbtn_gap, ctrcent->ctrbtn_square);
                }
                lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                lv_obj_set_style_pad_hor(obj, ctrcent->ctrbtn_gap, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_pad_top(obj, ctrcent->ctrbtn_gap, LV_PART_MAIN | LV_STATE_DEFAULT);
                lu_layout_info_t app_layout_info =
                {
                    .grid_size = {4, 32},
                    .obj_size = {ctrcent->ctrbtn_square, ctrcent->ctrbtn_square},
                    .obj_gap = {ctrcent->ctrbtn_gap, ctrcent->ctrbtn_gap},
                };
                ctrcent->ctrbtn_layout = lu_layout_create(obj, &app_layout_info);
                if(!ctrcent->ctrbtn_layout)
                {
                    lu_ctrcent_delete(&ctrcent);
                    return NULL;
                }
                lv_obj_set_style_grid_column_align(obj, LV_GRID_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                // lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                lv_obj_add_event_cb(obj, ctrbtn_area_contain_event_cb, LV_EVENT_PRESSED, ctrcent);
                lv_obj_add_event_cb(obj, ctrbtn_area_contain_event_cb, LV_EVENT_RELEASED, ctrcent);
            }
        }
        {
            lv_obj_t* obj = lu_widget_obj_init(ctrcent->contain);
            if(!obj)
            {
                lu_ctrcent_delete(&ctrcent);
                return NULL;
            }
            ctrcent->ctrbar_mask = obj;
            lv_obj_set_size(obj, LV_PCT(100), lu_ctrbar_get_height(ctrcent->ctrbar));
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_add_event_cb(obj, contain_event_cb, LV_EVENT_PRESSED, ctrcent);
            lv_obj_add_event_cb(obj, contain_event_cb, LV_EVENT_RELEASED, ctrcent);
            lv_obj_set_align(obj, LV_ALIGN_BOTTOM_MID);
        }
    }

    ctrcent->ctrbtn_list = lu_sll_create();
    if(!ctrcent->ctrbtn_list)
    {
        lu_ctrcent_delete(&ctrcent);
        return NULL;
    }
    {
        ctrcent->statbar_anim = lu_anim_create(lu_ctrbar_get_statbar_contain( ctrcent->ctrbar));
        if(!ctrcent->statbar_anim)
        {
            lu_ctrcent_delete(&ctrcent);
            return NULL;
        }
        lu_anim_param_t param =
        {
            .kp = 0.2,
            .kd = 0,
        };
        // lu_anim_set_value(ctrcent->statbar_anim, 0);
        lu_anim_set_param(ctrcent->statbar_anim, &param);
        lu_anim_set_get_cb(ctrcent->statbar_anim, lv_obj_get_y);
        lu_anim_set_set_cb(ctrcent->statbar_anim, lv_obj_set_y);
        lu_anim_start(ctrcent->statbar_anim);
    }
    {
        ctrcent->ctrbtn_area_contain_anim = lu_anim_create(ctrcent->ctrbtn_area_contain);
        if(!ctrcent->ctrbtn_area_contain_anim)
        {
            lu_ctrcent_delete(&ctrcent);
            return NULL;
        }
        lu_anim_param_t param =
        {
            .kp = 0.2,
            .kd = 0,
        };
        lu_anim_set_param(ctrcent->ctrbtn_area_contain_anim, &param);
        lu_anim_set_get_cb(ctrcent->ctrbtn_area_contain_anim, lv_obj_get_y);
        lu_anim_set_set_cb(ctrcent->ctrbtn_area_contain_anim, lv_obj_set_y);
        lu_anim_start(ctrcent->ctrbtn_area_contain_anim);
    }
    {
        ctrcent->bg_anim = lu_anim_create(ctrcent->bg);
        if(!ctrcent->bg_anim)
        {
            lu_ctrcent_delete(&ctrcent);
            return NULL;
        }
        lu_anim_param_t param =
        {
            .kp = 0.2,
            .kd = 0,
        };
        lu_anim_set_param(ctrcent->bg_anim, &param);
        lu_anim_set_get_cb(ctrcent->bg_anim, lu_anim_lv_get_bg_opa);
        lu_anim_set_set_cb(ctrcent->bg_anim, lu_anim_lv_set_bg_opa);
        lu_anim_start(ctrcent->bg_anim);
    }
    {
        ctrcent->contain_anim = lu_anim_create(ctrcent->contain);
        if(!ctrcent->contain_anim)
        {
            lu_ctrcent_delete(&ctrcent);
            return NULL;
        }
        lu_anim_param_t param =
        {
            .kp = 0.2,
            .kd = 0,
        };
        lu_anim_set_param(ctrcent->contain_anim, &param);
        lu_anim_set_get_cb(ctrcent->contain_anim, lv_obj_get_height);
        lu_anim_set_set_cb(ctrcent->contain_anim, lv_obj_set_height);
        lu_anim_start(ctrcent->contain_anim);
    }
    ctrcent->timer = lv_timer_create(ctrcent_timer_cb, LV_DEF_REFR_PERIOD, ctrcent);
    if(!ctrcent->timer)
    {
        lu_ctrcent_delete(&ctrcent);
        return NULL;
    }
    return ctrcent;
}

void lu_ctrcent_delete(lu_ctrcent_t** ctrcent)
{
    if(!ctrcent||!*ctrcent)
    {
        return;
    }
    if((*ctrcent)->timer)
    {
        lv_timer_delete((*ctrcent)->timer);
    }
    if((*ctrcent)->contain_anim)
    {
        lu_anim_delete(&(*ctrcent)->contain_anim);
    }
    if((*ctrcent)->bg_anim)
    {
        lu_anim_delete(&(*ctrcent)->bg_anim);
    }
    if((*ctrcent)->ctrbtn_area_contain_anim)
    {
        lu_anim_delete(&(*ctrcent)->ctrbtn_area_contain_anim);
    }
    if((*ctrcent)->statbar_anim)
    {
        lu_anim_delete(&(*ctrcent)->statbar_anim);
    }
    if((*ctrcent)->ctrbtn_list)
    {
        lu_sll_node_t* ctrbtn_list = lu_sll_get_head((*ctrcent)->ctrbtn_list);
        while(ctrbtn_list)
        {
            lu_ctrbtn_t* ctrbtn = lu_sll_get_data(ctrbtn_list);
            lu_ctrcent_remove_ctrbtn((*ctrcent), &ctrbtn);
            ctrbtn_list = lu_sll_get_next(ctrbtn_list);
        }
        lu_sll_delete(&(*ctrcent)->ctrbtn_list);
    }
    if((*ctrcent)->ctrbtn_layout)
    {
        lu_layout_delete(&(*ctrcent)->ctrbtn_layout);
    }
    if((*ctrcent)->contain)
    {
        lu_theme_remove_obj((*ctrcent)->lu_theme, (*ctrcent)->bg);
        lv_obj_delete((*ctrcent)->contain);
    }
    lu_free(*ctrcent);
    *ctrcent = NULL;
}

static void ctrbtn_event_cb(lv_event_t* event)
{
    lu_ctrbtn_t* ctrbtn = (lu_ctrbtn_t*)lv_event_get_user_data(event);
    if(ctrbtn->ctrbtn_cb)
    {
        // LU_LOG("ctrbtn_event_cb!=%p",ctrbtn->ctr_cb);
        ctrbtn->ctrbtn_cb(ctrbtn);
    }
}

void lu_ctrcent_add_ctrbtn_cb(lu_ctrbtn_t* ctrbtn, lu_ctrbtn_cb_t cb, void* user_data)
{
    if(!ctrbtn||!cb)
    {
        return;
    }
    ctrbtn->ctrbtn_cb = cb;
    ctrbtn->ctrbtn_user_data = user_data;
}

void* lu_ctrcent_get_ctrbtn_user_data(lu_ctrbtn_t* ctrbtn)
{
    if(!ctrbtn)
    {
        return NULL;
    }
    return ctrbtn->ctrbtn_user_data;
}

lu_ctrbtn_t* lu_ctrcent_add_ctrbtn(lu_ctrcent_t* ctrcent, const lu_ctrbtn_info_t* ctrbtn_info)
{
    if(!ctrcent||!ctrbtn_info)
    {
        return NULL;
    }
    if(!ctrbtn_info->ctrbtn_name&&!ctrbtn_info->ctrbtn_icon)
    {
        return NULL;
    }
    lu_ctrbtn_t* ctrbtn = (lu_ctrbtn_t*)lu_calloc(1, sizeof(lu_ctrbtn_t));
    if(!ctrbtn)
    {
        return NULL;
    }
    ctrbtn->ctrcent = ctrcent;
    // ctrbtn->ctrbtn_contain = ctrbtn_info->ctrbtn_contain;
    memcpy(&ctrbtn->ctrbtn_info, ctrbtn_info, sizeof(lu_ctrbtn_info_t));
    {
        lv_obj_t* obj = NULL;
        switch(ctrbtn_info->ctrbtn_size)
        {
            case LU_CTRBTN_SIZE_SMALL:
                obj = lu_widget_switch_init(ctrcent->ctrbtn_area);
                lv_obj_set_style_bg_opa(obj, LV_OPA_70, LV_PART_INDICATOR | LV_STATE_CHECKED);
                lv_obj_set_style_bg_opa(obj, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_event_cb(obj, ctrbtn_switch_event_cb, LV_EVENT_CLICKED, ctrbtn);
            break;
            case LU_CTRBTN_SIZE_MEDIUM:
                obj = lu_widget_slider_init(ctrcent->ctrbtn_area);
                lv_obj_set_style_bg_opa(obj, LV_OPA_70, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_opa(obj, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_event_cb(obj, ctrbtn_slider_event_cb, LV_EVENT_VALUE_CHANGED, ctrbtn);
            break;
            case LU_CTRBTN_SIZE_LARGE:
                obj = lu_widget_switch_init(ctrcent->ctrbtn_area);
                lv_obj_set_style_bg_opa(obj, LV_OPA_70, LV_PART_INDICATOR | LV_STATE_CHECKED);
                lv_obj_set_style_bg_opa(obj, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_event_cb(obj, ctrbtn_switch_event_cb, LV_EVENT_CLICKED, ctrbtn);
                break;
            default:
            break;
        }
        if(!obj)
        {
            lu_free(ctrbtn);
            return NULL;
        }
        ctrbtn->ctrbtn_item = obj;
        switch (ctrbtn_info->ctrbtn_size)
        {
            case LU_CTRBTN_SIZE_SMALL:
            {
                lv_point_t size = {1,1};
                lu_layout_set(ctrcent->ctrbtn_layout, obj, ctrbtn_info->ctrbtn_pos, size);
                break;
            }
            case LU_CTRBTN_SIZE_MEDIUM:
            {
                lv_point_t size = {1,2};
                lu_layout_set(ctrcent->ctrbtn_layout, obj, ctrbtn_info->ctrbtn_pos, size);
                break;
            }
            case LU_CTRBTN_SIZE_LARGE:
            {
                lv_point_t size = {2,2};
                lu_layout_set(ctrcent->ctrbtn_layout, obj, ctrbtn_info->ctrbtn_pos, size);
                break;
            }
            default:
            break;
        }
        // lu_theme_add_obj(ctrcent->lu_theme, obj, theme_info);
        lu_theme_info_t theme_info =
        {
            .self_type = LU_THEME_TYPE_THEME,
            .flag = LU_THEME_FLAG_SELF,
        };
        ctrbtn->ctrbtn_item_theme_obj = lu_theme_add_obj(ctrcent->lu_theme, obj, &theme_info);
        if(!ctrbtn->ctrbtn_item_theme_obj)
        {
            lv_obj_delete(ctrbtn->ctrbtn_item);
            lu_free(ctrbtn);
            return NULL;
        }
        // ctrbtn_status_update(ctrbtn);
        {
            lv_obj_t* obj = lu_widget_obj_init(ctrbtn->ctrbtn_item);
            if(!obj)
            {
                lu_theme_remove_obj(ctrcent->lu_theme, ctrbtn->ctrbtn_item);
                lv_obj_delete(ctrbtn->ctrbtn_item);
                lu_free(ctrbtn);
                return NULL;
            }
            ctrbtn->ctrbtn_item_mask = obj;
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            lv_obj_set_style_pad_all(obj, ctrcent->ctrbtn_gap, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_bg_opa(mask, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            // lv_obj_set_align(obj, LV_ALIGN_CENTER);
            lu_theme_info_t theme_info =
            {
                .child_type = LU_THEME_TYPE_INVMODE,
                .flag = LU_THEME_FLAG_CHILD,
            };
            ctrbtn->ctrbtn_item_mask_theme_obj = lu_theme_add_obj(ctrcent->lu_theme, obj, &theme_info);
            if(!ctrbtn->ctrbtn_item_mask_theme_obj)
            {
                lu_theme_remove_obj(ctrcent->lu_theme, ctrbtn->ctrbtn_item);
                lv_obj_delete(ctrbtn->ctrbtn_item);
                lv_obj_delete(ctrbtn->ctrbtn_item_mask);
                lu_free(ctrbtn);
                return NULL;
            }
            {
                if(ctrbtn_info->ctrbtn_icon)
                {
                    lv_obj_t * label = lu_widget_txt_init(obj,lu_font_get(lu_disp_get_dpi(ctrcent->lu_disp),LU_FONT_SIZE_HUGE,1));
                    if(!label)
                    {
                        lu_theme_remove_obj(ctrcent->lu_theme, ctrbtn->ctrbtn_item);
                        lu_theme_remove_obj(ctrcent->lu_theme, ctrbtn->ctrbtn_item_mask);
                        lv_obj_delete(ctrbtn->ctrbtn_item);
                        lv_obj_delete(ctrbtn->ctrbtn_item_mask);
                        lu_free(ctrbtn);
                        return NULL;
                    }
                    lv_label_set_text(label, ctrbtn_info->ctrbtn_icon);
                    // lu_theme_add_obj(ctrcent->lu_theme, label, LU_THEME_TYPE_MODE);
                    lv_obj_set_align(label, LV_ALIGN_CENTER);
                }
                if(ctrbtn_info->ctrbtn_name)
                {
                    if(ctrbtn_info->ctrbtn_size != LU_CTRBTN_SIZE_SMALL)
                    {
                        lv_obj_t* label = lu_widget_txt_init(obj,lu_font_get(lu_disp_get_dpi(ctrcent->lu_disp),LU_FONT_SIZE_NORMAL,1));
                        if(!label)
                        {
                            lu_theme_remove_obj(ctrcent->lu_theme, ctrbtn->ctrbtn_item);
                            lu_theme_remove_obj(ctrcent->lu_theme, ctrbtn->ctrbtn_item_mask);
                            lv_obj_delete(ctrbtn->ctrbtn_item);
                            lv_obj_delete(ctrbtn->ctrbtn_item_mask);
                            lu_free(ctrbtn);
                            return NULL;
                        }
                        lv_label_set_text(label, ctrbtn_info->ctrbtn_name);
                        // lu_theme_add_obj(ctrcent->lu_theme, label, LU_THEME_TYPE_MODE);
                        lv_obj_set_align(label, LV_ALIGN_BOTTOM_LEFT);
                    }
                }
            }
            lu_theme_update_obj(ctrcent->lu_theme, ctrbtn->ctrbtn_item_mask_theme_obj);
        }
    }
    lu_sll_append(ctrcent->ctrbtn_list, ctrbtn);
    return ctrbtn;
}

void lu_ctrcent_remove_ctrbtn(lu_ctrcent_t* ctrcent, lu_ctrbtn_t** ctrbtn)
{
    if(!ctrcent||!*ctrbtn)
    {
        return;
    }
    if(!(*ctrbtn)->ctrbtn_item)
    {
        return;
    }
    lu_theme_remove_obj(ctrcent->lu_theme, (*ctrbtn)->ctrbtn_item);
    lu_theme_remove_obj(ctrcent->lu_theme, (*ctrbtn)->ctrbtn_item_mask);
    lv_obj_delete((*ctrbtn)->ctrbtn_item);
    // if((*ctrbtn)->ctrbtn_item)
    // {
    // }
    // lv_obj_t* ctrbtn_item = (*ctrbtn)->ctrbtn_item;
    // lv_obj_t* obj = lv_obj_get_child(ctrbtn_item, 0);
    // while(lv_obj_get_child(obj, 0))
    // {
    //     lv_obj_t* child = lv_obj_get_child(obj, 0);
    //     lu_theme_remove_obj(ctrcent->lu_theme, child);
    //     lv_obj_delete(child);
    // }
    lu_free(*ctrbtn);
    *ctrbtn = NULL;
    return;
}

float lu_ctrcent_get_ctrbtn_status(lu_ctrbtn_t* ctrbtn)
{
    if(!ctrbtn)
    {
        return 0;
    }
    return ctrbtn->ctrbtn_status;
}

static void ctrbtn_status_update(lu_ctrbtn_t* ctrbtn)
{
    switch(ctrbtn->ctrbtn_info.ctrbtn_size)
    {
        case LU_CTRBTN_SIZE_SMALL:
            if(ctrbtn->ctrbtn_status)
            {
                // lv_obj_set_style_bg_color(ctrbtn->ctrbtn_item, lu_theme_get_color(ctrbtn->ctrcent->lu_theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_state(ctrbtn->ctrbtn_item, LV_STATE_CHECKED);
            }else
            {
                // lv_obj_set_style_bg_color(ctrbtn->ctrbtn_item, lu_color_smart_invert(lu_theme_get_color(ctrbtn->ctrcent->lu_theme)), LV_PART_MAIN | LV_STATE_DEFAULT);
                // lv_obj_set_style_bg_color(ctrbtn->ctrbtn_item, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_remove_state(ctrbtn->ctrbtn_item, LV_STATE_CHECKED);
            }
        break;
        case LU_CTRBTN_SIZE_MEDIUM:
            // lv_obj_set_style_bg_color(ctrbtn->ctrbtn_item, lu_theme_get_color(ctrbtn->ctrcent->lu_theme), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_slider_set_value(ctrbtn->ctrbtn_item,(int32_t)ctrbtn->ctrbtn_status,LV_ANIM_ON);
        break;
        case LU_CTRBTN_SIZE_LARGE:
            if(ctrbtn->ctrbtn_status)
            {
                lv_obj_add_state(ctrbtn->ctrbtn_item, LV_STATE_CHECKED);
                // lv_obj_set_style_bg_color(ctrbtn->ctrbtn_item, lu_theme_get_color(ctrbtn->ctrcent->lu_theme), LV_PART_MAIN | LV_STATE_DEFAULT);
            }else
            {
                lv_obj_remove_state(ctrbtn->ctrbtn_item, LV_STATE_CHECKED);
                // lv_obj_set_style_bg_color(ctrbtn->ctrbtn_item, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        break;
        default:
        break;
    }
}

void lu_ctrcent_set_ctrbtn_status(lu_ctrbtn_t* ctrbtn, float status)
{
    if(!ctrbtn)
    {
        return;
    }
    ctrbtn->ctrbtn_status = status;
    ctrbtn_status_update(ctrbtn);
}
