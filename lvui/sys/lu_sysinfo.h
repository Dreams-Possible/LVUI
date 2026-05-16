#ifndef LU_SYSINFO_H
#define LU_SYSINFO_H

#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"

typedef struct lu_sysinfo_t lu_sysinfo_t;

lu_sysinfo_t* lu_sysinfo_create();
void lu_sysinfo_delete(lu_sysinfo_t** sys);
void lu_sysinfo_set_year(lu_sysinfo_t* sys, uint16_t year);
void lu_sysinfo_get_year(lu_sysinfo_t* sys, uint16_t* year);
char* lu_sysinfo_get_year_str(lu_sysinfo_t* sys);
void lu_sysinfo_set_month(lu_sysinfo_t* sys, uint8_t month);
void lu_sysinfo_get_month(lu_sysinfo_t* sys, uint8_t* month);
const char* lu_sysinfo_get_month_str_full(lu_sysinfo_t* sys);
const char* lu_sysinfo_get_month_str_short(lu_sysinfo_t* sys);
void lu_sysinfo_set_day(lu_sysinfo_t* sys, uint8_t day);
void lu_sysinfo_get_day(lu_sysinfo_t* sys, uint8_t* day);
char* lu_sysinfo_get_day_str(lu_sysinfo_t* sys);
void lu_sysinfo_set_hour(lu_sysinfo_t* sys, uint8_t hour);
void lu_sysinfo_get_hour(lu_sysinfo_t* sys, uint8_t* hour);
char* lu_sysinfo_get_hour_str(lu_sysinfo_t* sys);
void lu_sysinfo_set_minute(lu_sysinfo_t* sys, uint8_t minute);
void lu_sysinfo_get_minute(lu_sysinfo_t* sys, uint8_t* minute);
char* lu_sysinfo_get_minute_str(lu_sysinfo_t* sys);
void lu_sysinfo_set_second(lu_sysinfo_t* sys, uint8_t second);
void lu_sysinfo_get_second(lu_sysinfo_t* sys, uint8_t* second);
char* lu_sysinfo_get_second_str(lu_sysinfo_t* sys);
void lu_sysinfo_set_time(lu_sysinfo_t* sys, uint64_t time);
void lu_sysinfo_set_timezone(lu_sysinfo_t* sys, int8_t timezone);
int8_t lu_sysinfo_get_timezone(lu_sysinfo_t* sys);
const char* lu_sysinfo_get_timezone_str(lu_sysinfo_t* sys);

#endif
