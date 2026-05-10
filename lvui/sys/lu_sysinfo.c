#include "lvui/plat/lu_type.h"
#include "lvui/plat/lu_lvgl.h"
#include "lvui/plat/lu_mem.h"

#include "lvui/misc/lu_log.h"
#include "lvui/misc/lu_pdc.h"
#include "lvui/misc/lu_sll.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

static const char *month_full[12] = {
    "January",      // 1月
    "February",     // 2月
    "March",        // 3月
    "April",        // 4月
    "May",          // 5月
    "June",         // 6月
    "July",         // 7月
    "August",       // 8月
    "September",    // 9月
    "October",      // 10月
    "November",     // 11月
    "December"      // 12月
};
static const char *month_short[12] = {
    "Jan",  // 1月
    "Feb",  // 2月
    "Mar",  // 3月
    "Apr",  // 4月
    "May",  // 5月
    "Jun",  // 6月
    "Jul",  // 7月
    "Aug",  // 8月
    "Sep",  // 9月
    "Oct",  // 10月
    "Nov",  // 11月
    "Dec"   // 12月
};

typedef struct sys_time_t
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} sys_time_t;

typedef struct sys_data_t
{
    char year[8];
    char month[8];
    char day[8];
    char hour[8];
    char minute[8];
    char second[8];
} sys_data_t;

typedef struct lu_sysinfo_t
{
    sys_time_t time;
    sys_data_t data;
} lu_sysinfo_t;

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

lu_sysinfo_t* lu_sysinfo_create()
{
    lu_sysinfo_t* sys = (lu_sysinfo_t*)lu_calloc(1, sizeof(lu_sysinfo_t));
    if(!sys)
    {
        return NULL;
    }
    return sys;
}

void lu_sysinfo_delete(lu_sysinfo_t** sys)
{
    if(!sys||!*sys)
    {
        return;
    }
    lu_free(*sys);
    *sys = NULL;
}

void lu_sysinfo_set_year(lu_sysinfo_t* sys, uint16_t year)
{
    if(!sys)
    {
        return;
    }
    sys->time.year = year;
    memset(sys->data.year, 0, sizeof(sys->data.year));
    snprintf(sys->data.year, sizeof(sys->data.year), "%d", year);
}

void lu_sysinfo_get_year(lu_sysinfo_t* sys, uint16_t* year)
{
    if(!sys||!year)
    {
        return;
    }
    *year = sys->time.year;
}

char* lu_sysinfo_get_year_str(lu_sysinfo_t* sys)
{
    if(!sys)
    {
        return NULL;
    }
    return sys->data.year;
}

void lu_sysinfo_set_month(lu_sysinfo_t* sys, uint8_t month)
{
    if(!sys)
    {
        return;
    }
    sys->time.month = month;
    memset(sys->data.month, 0, sizeof(sys->data.month));
}


void lu_sysinfo_get_month(lu_sysinfo_t* sys, uint8_t* month)
{
    if(!sys||!month)
    {
        return;
    }
    *month = sys->time.month;
}

const char* lu_sysinfo_get_month_str_full(lu_sysinfo_t* sys)
{
    if(!sys)
    {
        return NULL;
    }
    return month_full[sys->time.month - 1];
}

const char* lu_sysinfo_get_month_str_short(lu_sysinfo_t* sys)
{
    if(!sys)
    {
        return NULL;
    }
    return month_short[sys->time.month - 1];
}
void lu_sysinfo_set_day(lu_sysinfo_t* sys, uint8_t day)
{
    if(!sys)
    {
        return;
    }
    sys->time.day = day;
    memset(sys->data.day, 0, sizeof(sys->data.day));
    snprintf(sys->data.day, sizeof(sys->data.day), "%d", sys->time.day);
}

void lu_sysinfo_get_day(lu_sysinfo_t* sys, uint8_t* day)
{
    if(!sys||!day)
    {
        return;
    }
    *day = sys->time.day;
}

char* lu_sysinfo_get_day_str(lu_sysinfo_t* sys)
{
    if(!sys)
    {
        return NULL;
    }
    return sys->data.day;
}

void lu_sysinfo_set_hour(lu_sysinfo_t* sys, uint8_t hour)
{
    if(!sys)
    {
        return;
    }
    sys->time.hour = hour;
    memset(sys->data.hour, 0, sizeof(sys->data.hour));
    snprintf(sys->data.hour, sizeof(sys->data.hour), "%.2d", sys->time.hour);
}

void lu_sysinfo_get_hour(lu_sysinfo_t* sys, uint8_t* hour)
{
    if(!sys||!hour)
    {
        return;
    }
    *hour = sys->time.hour;
}

char* lu_sysinfo_get_hour_str(lu_sysinfo_t* sys)
{
    if(!sys)
    {
        return NULL;
    }
    return sys->data.hour;
}

void lu_sysinfo_set_minute(lu_sysinfo_t* sys, uint8_t minute)
{
    if(!sys)
    {
        return;
    }
    sys->time.minute = minute;
    memset(sys->data.minute, 0, sizeof(sys->data.minute));
    snprintf(sys->data.minute, sizeof(sys->data.minute), "%.2d", sys->time.minute);
}

void lu_sysinfo_get_minute(lu_sysinfo_t* sys, uint8_t* minute)
{
    if(!sys||!minute)
    {
        return;
    }
    *minute = sys->time.minute;
}

char* lu_sysinfo_get_minute_str(lu_sysinfo_t* sys)
{
    if(!sys)
    {
        return NULL;
    }
    return sys->data.minute;
}

void lu_sysinfo_set_second(lu_sysinfo_t* sys, uint8_t second)
{
    if(!sys)
    {
        return;
    }
    sys->time.second = second;
    memset(sys->data.second, 0, sizeof(sys->data.second));
    snprintf(sys->data.second, sizeof(sys->data.second), "%.2d", sys->time.second);
}

void lu_sysinfo_get_second(lu_sysinfo_t* sys, uint8_t* second)
{
    if(!sys||!second)
    {
        return;
    }
    *second = sys->time.second;
}

char* lu_sysinfo_get_second_str(lu_sysinfo_t* sys)
{
    if(!sys)
    {
        return NULL;
    }
    return sys->data.second;
}

void lu_sysinfo_set_time(lu_sysinfo_t* sys, uint64_t time)
{
    if(!sys)
    {
        return;
    }
    time_t now = (time_t)time;
    struct tm *tinfo = gmtime(&now);
    if(!tinfo)
    {
        return;
    }
    lu_sysinfo_set_year(sys, tinfo->tm_year + 1900);
    lu_sysinfo_set_month(sys, tinfo->tm_mon + 1);
    lu_sysinfo_set_day(sys, tinfo->tm_mday);
    lu_sysinfo_set_hour(sys, tinfo->tm_hour+8);
    lu_sysinfo_set_minute(sys, tinfo->tm_min);
    lu_sysinfo_set_second(sys, tinfo->tm_sec);
}
