#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/misc/lu_log.h"
#include "lvui/plat/lu_mem.h"
#include "lvui/misc/lu_sll.h"
#include "lu_color.h"
#include "src/core/lv_obj.h"
#include "src/misc/lv_color.h"
#include "src/misc/lv_types.h"
#include "lu_theme.h"

typedef struct lu_theme_obj_t
{
    lv_obj_t* obj;
    lu_theme_info_t info;
}lu_theme_obj_t;
typedef struct lu_theme_t
{
    lu_sll_t* obj_list;
    lv_color_t theme_color;
    lu_theme_mode_t theme_mode;
}lu_theme_t;

lu_theme_t* lu_theme_create();
void lu_theme_delete(lu_theme_t* theme);
lu_theme_obj_t* lu_theme_add_obj(lu_theme_t* theme, lv_obj_t* obj, const lu_theme_info_t* info);
void lu_theme_remove_obj(lu_theme_t* theme, lv_obj_t* obj);
void lu_theme_set_color(lu_theme_t* theme, lv_color_t color);
lv_color_t lu_theme_get_color(lu_theme_t* theme);
void lu_theme_set_mode(lu_theme_t* theme, lu_theme_mode_t mode);
lu_theme_mode_t lu_theme_get_mode(lu_theme_t* theme);
lv_color_t lu_theme_get_bright_color(lu_theme_t* theme);
lv_color_t lu_theme_get_dark_color(lu_theme_t* theme);
static void obj_theme_update(lu_theme_t* theme, lv_obj_t* obj, lu_theme_type_t theme_type);
static void theme_update(lu_theme_t* theme, lu_theme_obj_t* lu_theme_obj);
static void theme_apply(lu_theme_t* theme);
void lu_theme_update_obj(lu_theme_t* theme, lu_theme_obj_t* theme_obj);

lu_theme_t* lu_theme_create()
{
    lu_theme_t* theme= (lu_theme_t*)lu_calloc(1,sizeof(lu_theme_t));
    if(!theme)
    {
        return NULL;
    }
    theme->obj_list=lu_sll_create();
    if(!theme->obj_list)
    {
        lu_free(theme);
        return NULL;
    }
    return theme;
}

void lu_theme_delete(lu_theme_t* theme)
{
    if(theme)
    {
        lu_sll_delete(&theme->obj_list);
        lu_free(theme);
    }
}

lu_theme_obj_t* lu_theme_add_obj(lu_theme_t* theme, lv_obj_t* obj, const lu_theme_info_t* info)
{
    if(theme&&obj&&info)
    {
        lu_theme_obj_t* lu_theme_obj=(lu_theme_obj_t*)lu_calloc(1,sizeof(lu_theme_obj_t));
        if(!lu_theme_obj)
        {
            return NULL;
        }
        lu_theme_obj->obj=obj;
        memcpy(&lu_theme_obj->info, info, sizeof(lu_theme_info_t));
        lu_sll_node_t* node=lu_sll_append(theme->obj_list, lu_theme_obj);
        if(!node)
        {
            lu_free(lu_theme_obj);
            return NULL;
        }
        theme_update(theme, lu_theme_obj);
        return lu_theme_obj;
    }
    return NULL;
}

void lu_theme_remove_obj(lu_theme_t* theme, lv_obj_t* obj)
{
    if(theme)
    {
        lu_sll_node_t* node=lu_sll_get_head(theme->obj_list);
        while(node)
        {
            lu_theme_obj_t* lu_theme_obj=(lu_theme_obj_t*)lu_sll_get_data(node);
            if(lu_theme_obj->obj==obj)
            {
                int32_t index=lu_sll_get_index(theme->obj_list, node);
                lu_sll_remove(theme->obj_list, index);
                break;
            }
            node = lu_sll_get_next(node);
        }
    }
}

void lu_theme_set_color(lu_theme_t* theme, lv_color_t color)
{
    if(theme)
    {
        theme->theme_color=color;
        theme_apply(theme);
    }
}

lv_color_t lu_theme_get_color(lu_theme_t* theme)
{
    if(theme)
    {
        return theme->theme_color;
    }
    return lv_color_hex(0xff000000);
}

void lu_theme_set_mode(lu_theme_t* theme, lu_theme_mode_t mode)
{
    if(theme)
    {
        theme->theme_mode=mode;
        theme_apply(theme);
    }
}

lu_theme_mode_t lu_theme_get_mode(lu_theme_t* theme)
{
    if(theme)
    {
        return theme->theme_mode;
    }
    return LU_THEME_MODE_BRIGHT;
}

lv_color_t lu_theme_get_bright_color(lu_theme_t* theme)
{
    return lv_color_white();
}

lv_color_t lu_theme_get_dark_color(lu_theme_t* theme)
{
    return lv_color_black();
}

static void obj_theme_update(lu_theme_t* theme, lv_obj_t* obj, lu_theme_type_t theme_type)
{
    switch (theme_type)
    {
        case LU_THEME_TYPE_MODE:
            switch (theme->theme_mode)
            {
                case LU_THEME_MODE_BRIGHT:
                    if(lv_obj_check_type(obj, &lv_obj_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }
                    if(lv_obj_check_type(obj, &lv_label_class))
                    {
                        lv_obj_set_style_text_color(obj, lu_theme_get_bright_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_button_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_switch_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_slider_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_image_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }
                break;
                case LU_THEME_MODE_DARK:
                    if(lv_obj_check_type(obj, &lv_obj_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_label_class))
                    {
                        lv_obj_set_style_text_color(obj, lu_theme_get_dark_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_button_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_switch_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_slider_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_image_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }
                break;
                default:
                break;
            }
        break;
        case LU_THEME_TYPE_INVMODE:
            switch (theme->theme_mode)
            {
                case LU_THEME_MODE_BRIGHT:
                    if(lv_obj_check_type(obj, &lv_obj_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_label_class))
                    {
                        lv_obj_set_style_text_color(obj, lu_theme_get_dark_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_button_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else   
                    if(lv_obj_check_type(obj, &lv_switch_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_slider_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_image_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }
                break;
                case LU_THEME_MODE_DARK:
                    if(lv_obj_check_type(obj, &lv_obj_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_label_class))
                    {
                        lv_obj_set_style_text_color(obj, lu_theme_get_bright_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_button_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else   
                    if(lv_obj_check_type(obj, &lv_switch_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_slider_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_image_class))
                    {
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }
                break;
                default:
                break;
            }
        break;
        case LU_THEME_TYPE_THEME:
            switch (theme->theme_mode)
            {
                case LU_THEME_MODE_BRIGHT:
                    if(lv_obj_check_type(obj, &lv_obj_class))
                    {
                        lv_obj_set_style_bg_color(obj, theme->theme_color, LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_label_class))
                    {
                        lv_obj_set_style_text_color(obj, theme->theme_color, LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_button_class))
                    {
                        lv_obj_set_style_bg_color(obj, theme->theme_color, LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else   
                    if(lv_obj_check_type(obj, &lv_switch_class))
                    {
                        lv_obj_set_style_bg_color(obj, theme->theme_color, LV_PART_INDICATOR | LV_STATE_CHECKED);
                        // lv_obj_set_style_bg_color(obj, lv_color_lighten(theme->theme_color, LV_OPA_70), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_slider_class))
                    {
                        lv_obj_set_style_bg_color(obj, theme->theme_color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                        lv_obj_set_style_bg_color(obj, lu_theme_get_bright_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_image_class))
                    {
                        lv_obj_set_style_bg_color(obj, theme->theme_color, LV_PART_MAIN | LV_STATE_DEFAULT);
                    }
                break;
                case LU_THEME_MODE_DARK:
                    if(lv_obj_check_type(obj, &lv_obj_class))
                    {
                        lv_obj_set_style_bg_color(obj, lv_color_darken(theme->theme_color, 32), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_label_class))
                    {
                        lv_obj_set_style_text_color(obj, lv_color_darken(theme->theme_color, 32), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_button_class))
                    {
                        lv_obj_set_style_bg_color(obj, lv_color_darken(theme->theme_color, 32), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_switch_class))
                    {
                        lv_obj_set_style_bg_color(obj, theme->theme_color, LV_PART_INDICATOR | LV_STATE_CHECKED);
                        // lv_obj_set_style_bg_color(obj, lv_color_lighten(theme->theme_color, 64), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_slider_class))
                    {
                        lv_obj_set_style_bg_color(obj, theme->theme_color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                        lv_obj_set_style_bg_color(obj, lu_theme_get_dark_color(theme), LV_PART_MAIN | LV_STATE_DEFAULT);
                    }else
                    if(lv_obj_check_type(obj, &lv_image_class))
                    {
                        lv_obj_set_style_bg_color(obj, lv_color_darken(theme->theme_color, 32), LV_PART_MAIN | LV_STATE_DEFAULT);
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

static void theme_update(lu_theme_t* theme, lu_theme_obj_t* theme_obj)
{
    if(!theme||!theme_obj)
    {
        return;
    }
    switch (theme_obj->info.flag)
    {
        case LU_THEME_FLAG_SELF:
            obj_theme_update(theme, theme_obj->obj, theme_obj->info.self_type);
        break;
        case LU_THEME_FLAG_CHILD:
            for(int32_t i=0;i<lv_obj_get_child_count(theme_obj->obj);++i)
            {
                obj_theme_update(theme, lv_obj_get_child(theme_obj->obj, i), theme_obj->info.child_type);
            }
        break;
        case LU_THEME_FLAG_ALL:
            obj_theme_update(theme, theme_obj->obj, theme_obj->info.self_type);
            for(int32_t i=0;i<lv_obj_get_child_count(theme_obj->obj);++i)
            {
                obj_theme_update(theme, lv_obj_get_child(theme_obj->obj, i), theme_obj->info.child_type);
            }
        break;
        default:
        break;
    }
}

static void theme_apply(lu_theme_t* theme)
{
    if(theme)
    {
        lu_sll_node_t* node=lu_sll_get_head(theme->obj_list);
        while(node)
        {
            lu_theme_obj_t* theme_obj=(lu_theme_obj_t*)lu_sll_get_data(node);
            theme_update(theme, theme_obj);
            node=lu_sll_get_next(node);
        }
    }
}

void lu_theme_update_obj(lu_theme_t* theme, lu_theme_obj_t* theme_obj)
{
    if(!theme||!theme_obj)
    {
        return;
    }
    theme_update(theme, theme_obj);
}
