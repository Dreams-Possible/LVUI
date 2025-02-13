//include
#include"lvui_manager.h"

//main
void main()
{
    //lvgl init
    lv_init();

    //lvui ui init
    lvui_ui_init(240,320,2.8);

    while(1)
    {
        lv_tick_inc(10);
        lv_task_handler();
        delay(10);
    }
}
