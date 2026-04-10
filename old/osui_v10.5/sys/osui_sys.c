#include"osui_sys.h"

#include<string.h>

//系统时间类
typedef struct sys_time_t
{

    uint8_t hour;
    uint8_t minute;
} sys_time_t;
//系统日期类
typedef struct sys_date_t
{
    uint8_t month;
    uint8_t day;
} sys_date_t;
//系统数据类
typedef struct sys_data_t
{
    char time[8];
    char date[16];
    char year[8];
    char batt_icon[8];
    char batt_str[8];
    char icon[16];
    char locker_icon[32];
    char stat_batt[32];
} sys_data_t;
//系统状态类
typedef struct sys_stat_t
{
    sys_time_t time;
    sys_date_t date;
    uint16_t year;
    uint8_t batt;
    // bool mult_stat;
    bool wifi_stat;
    bool bt_stat;
    sys_data_t data;
} sys_stat_t;
static sys_stat_t sys_stat = {0};

//月份字典
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
// static const char *month_short[12] = {
//     "Jan",  // 1月
//     "Feb",  // 2月
//     "Mar",  // 3月
//     "Apr",  // 4月
//     "May",  // 5月
//     "Jun",  // 6月
//     "Jul",  // 7月
//     "Aug",  // 8月
//     "Sep",  // 9月
//     "Oct",  // 10月
//     "Nov",  // 11月
//     "Dec"   // 12月
// };

// //星期字典
// static const char *weekday_full[] = {
//     "Sunday", "Monday", "Tuesday", "Wednesday",
//     "Thursday", "Friday", "Saturday"
// };
// static const char *weekday_short[] = {
//     "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
// };

//系统设置时间
void osui_sys_set_time(uint8_t hour, uint8_t minute);
//系统设置日期
void osui_sys_set_date(uint8_t month, uint8_t day);
//系统设置年份
void osui_sys_set_year(uint16_t year);
//系统设置电量
void osui_sys_set_batt(uint8_t batt);
//系统设置WIFI状态
void osui_sys_set_wifi_stat(bool stat);
//系统设置蓝牙状态
void osui_sys_set_bt_stat(bool stat);
//更新数据
static void update_data();
//更新UI
void osui_ui_locker_set_icon(const char* icon);
void osui_ui_locker_set_time(const char* time);
void osui_ui_locker_set_date(const char* date);
void osui_ui_locker_set_year(const char* year);
void osui_ui_statbar_set_icon(const char* icon);
void osui_ui_statbar_set_time(const char* time);
void osui_ui_statbar_set_batt(const char* batt);
void osui_ui_desktop_set_time(const char* time);
void osui_ui_desktop_set_date(const char* date);
static void update_ui();
//系统刷新
void osui_sys_refresh();
//系统初始化
void osui_sys_init();

//系统设置时间
void osui_sys_set_time(uint8_t hour, uint8_t minute)
{
    sys_stat.time.hour = hour;
    sys_stat.time.minute = minute;
}

//系统设置日期
void osui_sys_set_date(uint8_t month, uint8_t day)
{
    sys_stat.date.month = month;
    sys_stat.date.day = day;
}

//系统设置年份
void osui_sys_set_year(uint16_t year)
{
    sys_stat.year = year;
}

//系统设置电量
void osui_sys_set_batt(uint8_t batt)
{
    sys_stat.batt = batt;
}

//系统设置WIFI状态
void osui_sys_set_wifi_stat(bool stat)
{
    sys_stat.wifi_stat = stat;
}

//系统设置蓝牙状态
void osui_sys_set_bt_stat(bool stat)
{
    sys_stat.bt_stat = stat;
}

//更新数据
static void update_data()
{
    //时间
    memset(sys_stat.data.time, 0, sizeof(sys_stat.data.time));
    snprintf(sys_stat.data.time, sizeof(sys_stat.data.time), "%d:%d", sys_stat.time.hour, sys_stat.time.minute);
    //日期
    memset(sys_stat.data.date, 0, sizeof(sys_stat.data.date));
    snprintf(sys_stat.data.date, sizeof(sys_stat.data.date), "%s %d", month_full[sys_stat.date.month-1], sys_stat.date.day);
    //年份
    memset(sys_stat.data.year, 0, sizeof(sys_stat.data.year));
    snprintf(sys_stat.data.year, sizeof(sys_stat.data.year), "%d", sys_stat.year);
    //电量
    memset(sys_stat.data.batt_icon, 0, sizeof(sys_stat.data.batt_icon));
    uint8_t batt_lev=sys_stat.batt;
    if(batt_lev<10)
    {
        snprintf(sys_stat.data.batt_icon, sizeof(sys_stat.data.batt_icon), LV_SYMBOL_BATTERY_EMPTY);
    }else
    if(batt_lev<30)
    {
        snprintf(sys_stat.data.batt_icon, sizeof(sys_stat.data.batt_icon), LV_SYMBOL_BATTERY_1);
    }else
    if(batt_lev<55)
    {
        snprintf(sys_stat.data.batt_icon, sizeof(sys_stat.data.batt_icon), LV_SYMBOL_BATTERY_2);
    }else
    if(batt_lev<80)
    {
        snprintf(sys_stat.data.batt_icon, sizeof(sys_stat.data.batt_icon), LV_SYMBOL_BATTERY_3);
    }else
    if(batt_lev<100)
    {
        snprintf(sys_stat.data.batt_icon, sizeof(sys_stat.data.batt_icon), LV_SYMBOL_BATTERY_FULL);
    }
    snprintf(sys_stat.data.batt_str, sizeof(sys_stat.data.batt_str), "%d%%", batt_lev);
    //图标
    memset(sys_stat.data.icon, 0, sizeof(sys_stat.data.icon));
    // if(sys_stat.mult_stat)
    // {
    //     snprintf(sys_stat.data.icon+strlen(sys_stat.data.icon), sizeof(sys_stat.data.icon)-strlen(sys_stat.data.icon), LV_SYMBOL_MUTE);
    // }
    if(!strlen(sys_stat.data.icon))
    {
        snprintf(sys_stat.data.icon, sizeof(sys_stat.data.icon), LV_SYMBOL_HOME " " );
    }
    if(sys_stat.wifi_stat)
    {
        snprintf(sys_stat.data.icon+strlen(sys_stat.data.icon), sizeof(sys_stat.data.icon)-strlen(sys_stat.data.icon), LV_SYMBOL_WIFI " " );
    }
    if(sys_stat.bt_stat)
    {
        snprintf(sys_stat.data.icon+strlen(sys_stat.data.icon), sizeof(sys_stat.data.icon)-strlen(sys_stat.data.icon), LV_SYMBOL_BLUETOOTH " ");
    }
    // if(!strlen(sys_stat.data.icon))
    // {
    //     snprintf(sys_stat.data.icon, sizeof(sys_stat.data.icon), LV_SYMBOL_HOME);
    // }
}

//更新UI
static void update_ui()
{
    //锁屏
    memset(sys_stat.data.locker_icon, 0, sizeof(sys_stat.data.locker_icon));
    snprintf(sys_stat.data.locker_icon, sizeof(sys_stat.data.locker_icon), "%s %s", sys_stat.data.icon, sys_stat.data.batt_icon);
    osui_ui_locker_set_icon(sys_stat.data.locker_icon);
    osui_ui_locker_set_time(sys_stat.data.time);
    osui_ui_locker_set_date(sys_stat.data.date);
    osui_ui_locker_set_year(sys_stat.data.year);
    //状态栏
    osui_ui_statbar_set_icon(sys_stat.data.icon);
    osui_ui_statbar_set_time(sys_stat.data.time);
    memset(sys_stat.data.stat_batt, 0, sizeof(sys_stat.data.stat_batt));
    snprintf(sys_stat.data.stat_batt, sizeof(sys_stat.data.stat_batt), "%s %s", sys_stat.data.batt_str, sys_stat.data.batt_icon);
    osui_ui_statbar_set_batt(sys_stat.data.stat_batt);
    //桌面
    osui_ui_desktop_set_time(sys_stat.data.time);
    osui_ui_desktop_set_date(sys_stat.data.date);
}

//系统刷新
void osui_sys_refresh()
{
    update_data();
    update_ui();
}

//系统初始化
void osui_sys_init()
{
    sys_stat.time.hour=10;
    sys_stat.time.minute=31;
    sys_stat.date.month=2;
    sys_stat.date.day=2;
    sys_stat.year=2027;
    sys_stat.batt=99;
    sys_stat.wifi_stat=false;
    sys_stat.bt_stat=false;
    osui_sys_refresh();
}
