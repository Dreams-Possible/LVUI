#ifndef __OSUI_UI_FRAME_H__
#define __OSUI_UI_FRAME_H__

//OSUI基础
#include"osui/core/osui_base.h"

//UI框架初始化
void osui_ui_frame_init();
//UI框架获取桌面
lv_obj_t **osui_uiframe_get();
//UI框架载入
void osui_ui_frame_load();

#endif
