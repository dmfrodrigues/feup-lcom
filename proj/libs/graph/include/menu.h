#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "font.h"

typedef struct menu menu_t;

menu_t* (menu_ctor)(const font_t *fnt);

int (menu_update_state)(menu_t *menu, int click);

void (menu_draw)(menu_t *menu);

void (menu_dtor)(menu_t *p);

#endif //MENU_H_INCLUDED
