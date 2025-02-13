#if 1
#ifndef LVAP_FILEBROWSER_H
#define LVAP_FILEBROWSER_H

//include
#include"lvui_manager.h"

//lvui init filebrowser
void lvui_filebrowser_init();
//lvui load filebrowser
lv_obj_t*lvui_filebrowser_load();
//filebrowser add
void filebrowser_add(char*text);
//filebrowser clean
void filebrowser_clean();
//filebrowser title
void filebrowser_title(char*text);

#endif//#ifndef LVAP_FILEBROWSER_H
#endif//#if 1
