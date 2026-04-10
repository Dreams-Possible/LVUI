#ifndef LU_LOG_H
#define LU_LOG_H

#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"

void lu_log(char* fmt, ...);

#define LU_LOG LV_LOG_USER

#endif