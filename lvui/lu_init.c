#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/misc/lu_log.h"
#include "lvui/plat/lu_mem.h"

#include "lvui/frame/input/lu_touch.h"

bool lu_init();

bool lu_init()
{
    return lu_touch_init();    
}