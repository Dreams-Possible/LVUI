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

#include "lu_desktop.h"

typedef struct lu_desktop_t
{
    lu_disp_t* lu_disp;
    lu_theme_t* lu_theme;
    lu_touch_t* lu_touch;
    lv_obj_t* contain;
}lu_desktop_t;

lu_desktop_t* lu_desktop_create(lu_disp_t* lu_disp, lu_theme_t* lu_theme, lu_touch_t* lu_touch, lv_obj_t* parent);
void lu_desktop_delete(lu_desktop_t** desktop);
lv_obj_t* lu_desktop_get_contain(lu_desktop_t* desktop);

lu_desktop_t* lu_desktop_create(lu_disp_t* lu_disp, lu_theme_t* lu_theme, lu_touch_t* lu_touch, lv_obj_t* parent)
{
    if(!lu_disp||!lu_theme||!lu_touch||!parent)
    {
        return NULL;
    }
    lu_desktop_t* desktop = (lu_desktop_t*)lu_calloc(1, sizeof(lu_desktop_t));
    if(!desktop)
    {
        return NULL;
    }
    desktop->lu_disp = lu_disp;
    desktop->lu_theme = lu_theme;
    desktop->lu_touch = lu_touch;
    lv_obj_t* obj = lu_widget_obj_init(parent);
    if(!obj)
    {
        lu_desktop_delete(&desktop);
        return NULL;
    }
    desktop->contain = obj;
    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
    // lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    return desktop;
}

void lu_desktop_delete(lu_desktop_t** desktop)
{
    if(!desktop||!*desktop)
    {
        return;
    }
    if((*desktop)->contain)
    {
        lv_obj_delete((*desktop)->contain);
    }
    lu_free(*desktop);
    *desktop = NULL;
}

lv_obj_t* lu_desktop_get_contain(lu_desktop_t* desktop)
{
    return desktop->contain;
}
