#include"lvui_display.h"
#ifdef LVUI_DISPLAY_H

//display
typedef struct display_t
{
    uint16_t hor;
    uint16_t ver;
    float size;//inch
    float dpi;
    uint8_t level;//pixel number level
}display_t;
display_t display;

//lvui display change
uint16_t lvui_display_change(uint16_t dp);
//lvui display init
void lvui_display_init(uint16_t hor,uint16_t ver,float size);
//lvui display level
uint8_t lvui_display_level();

//lvui display change
uint16_t lvui_display_change(uint16_t dp)
{
    float px=dp*display.dpi/160;
    return (uint16_t)px;
}

//lvui display init
void lvui_display_init(uint16_t hor,uint16_t ver,float size)
{
    display.hor=hor;
    display.ver=ver;
    display.size=size;
    display.dpi=sqrt(pow(hor,2)+pow(ver,2))/size;
    //just for test
    // display.dpi=165.63;
    printf("display.dpi=%f\n",display.dpi);
    uint32_t volume=hor*ver;
    if(volume<=320*480)
    {
        //small
        display.level=LVUI_DISPLAY_LEVEL_SMALL;
        printf("LVUI_DISPLAY_LEVEL_SMALL\n");
    }else
    if(volume<=720*480)
    {
        //nomal
        display.level=LVUI_DISPLAY_LEVEL_NOMALL;
        printf("LVUI_DISPLAY_LEVEL_NOMALL\n");
    }else
    {
        //big
        display.level=LVUI_DISPLAY_LEVEL_BIG;
        printf("LVUI_DISPLAY_LEVEL_BIG\n");
    }
}

//lvui display level
uint8_t lvui_display_level()
{
    return display.level;
}

#endif//#ifndef LVUI_DISPLAY_H
