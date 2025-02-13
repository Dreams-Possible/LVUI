#include"lvui_ui.h"
#ifdef LVUI_UI_H


void shortcutlist_test1(uint8_t*state)
{
    printf("shortcutlist_test1\n");
}

void shortcutlist_test2(uint8_t*state)
{
    printf("shortcutlist_test2\n");
}

void noticelist_test1(uint8_t*state)
{
    printf("noticelist_test1\n");
}

void noticelist_test2(uint8_t*state)
{
    printf("noticelist_test2\n");
}

//lvui ui init
void lvui_ui_init(uint16_t hor,uint16_t ver,float size)
{
    //init lvui manager
    lvui_manager_init(hor,ver,size);

    //lvui init appexample
    lvui_appexample_init();
    lv_obj_t*app1=lvui_appexample_load();

    //lvui init appexample setting
    lvui_appexample_setting_init();
    lv_obj_t*app2=lvui_appexample_setting_load();

    applist_add("test1",app1);
    applist_add("test2",app2);

    shortcutlist_add("test1",shortcutlist_test1);
    shortcutlist_add("test2",shortcutlist_test2);

    noticelist_add("test1",noticelist_test1);
    noticelist_add("test2",noticelist_test1);
}

#endif//#ifndef LVUI_UI_H




