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

#include "lu_ctrbar.h"
#include "lu_statbar.h"

typedef struct lu_ctrbar_t
{
    lu_disp_t* lu_disp;
    lu_theme_t* lu_theme;
    lu_touch_t* lu_touch;
    lv_obj_t* contain;
    lv_obj_t* statbar;
    lu_statbar_t* lu_statbar;
}lu_ctrbar_t;

int32_t lu_ctrbar_get_height(lu_ctrbar_t* ctrbar);
lu_ctrbar_t* lu_ctrbar_create(lu_disp_t* lu_disp, lu_theme_t* lu_theme, lu_touch_t* lu_touch, lv_obj_t* parent);
void lu_ctrbar_delete(lu_ctrbar_t** ctrbar);
lv_obj_t* lu_ctrbar_get_contain(lu_ctrbar_t* ctrbar);
lv_obj_t* lu_ctrbar_get_statbar_contain(lu_ctrbar_t* ctrbar);
lu_statbar_t* lu_ctrbar_get_statbar(lu_ctrbar_t* ctrbar);

int32_t lu_ctrbar_get_height(lu_ctrbar_t* ctrbar)
{
    if(!ctrbar)
    {
        return 0;
    }
    return lu_disp_dp2px(lu_disp_get_dpi(ctrbar->lu_disp), 30);
}

lu_ctrbar_t* lu_ctrbar_create(lu_disp_t* lu_disp, lu_theme_t* lu_theme, lu_touch_t* lu_touch, lv_obj_t* parent)
{
    if(!lu_disp||!lu_theme||!lu_touch||!parent)
    {
        return NULL;
    }
    lu_ctrbar_t* ctrbar = (lu_ctrbar_t*)lu_calloc(1, sizeof(lu_ctrbar_t));
    if(!ctrbar)
    {
        return NULL;
    }
    ctrbar->lu_disp = lu_disp;
    ctrbar->lu_theme = lu_theme;
    ctrbar->lu_touch = lu_touch;
    lv_obj_t* obj = lu_widget_obj_init(parent);
    if(!obj)
    {
        lu_ctrbar_delete(&ctrbar);
        return NULL;
    }
    ctrbar->contain = obj;
    // lv_obj_set_size(obj, lu_disp_get_res(lu_disp).x,  lu_ctrbar_get_height(ctrbar));
    lv_obj_set_size(obj, LV_PCT(100),  LV_SIZE_CONTENT);

    // lv_obj_set_style_bg_opa(obj, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {        
        lv_obj_t* obj = lu_widget_obj_init(ctrbar->contain);
        if(!obj)
        {
            lu_ctrbar_delete(&ctrbar);
            return NULL;
        }
        ctrbar->statbar = obj;
        lv_obj_set_size(obj, LV_PCT(100),  lu_ctrbar_get_height(ctrbar));
        // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

        // lv_obj_set_style_bg_opa(obj, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN | LV_STATE_DEFAULT);
        
    }
    ctrbar->lu_statbar = lu_statbar_create(lu_disp, lu_theme, lu_touch, ctrbar->statbar);
    if(!ctrbar->lu_statbar)
    {
        lu_ctrbar_delete(&ctrbar);
        return NULL;
    }
    return ctrbar;
}

void lu_ctrbar_delete(lu_ctrbar_t** ctrbar)
{
    if(!ctrbar||!*ctrbar)
    {
        return;
    }
    if((*ctrbar)->lu_statbar)
    {
        lu_statbar_delete(&((*ctrbar)->lu_statbar));
    }
    if((*ctrbar)->contain)
    {
        lv_obj_delete((*ctrbar)->contain);
    }
    lu_free(*ctrbar);
    *ctrbar = NULL;
}

lv_obj_t* lu_ctrbar_get_contain(lu_ctrbar_t* ctrbar)
{
    if(!ctrbar)
    {
        return NULL;
    }
    return ctrbar->contain;
}

lv_obj_t* lu_ctrbar_get_statbar_contain(lu_ctrbar_t* ctrbar)
{
    if(!ctrbar)
    {
        return NULL;
    }
    return ctrbar->statbar;
}

lu_statbar_t* lu_ctrbar_get_statbar(lu_ctrbar_t* ctrbar)
{
    if(!ctrbar)
    {
        return NULL;
    }
    return ctrbar->lu_statbar;
}
