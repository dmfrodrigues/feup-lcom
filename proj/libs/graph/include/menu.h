#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

/**
 * @defgroup    menu_t    menu_t
 * @ingroup graph
 * @brief Menu module
 *
 * Allows to create flexible and responsive menus.
 *
 * @{
 */

#include "font.h"

typedef struct menu menu_t;

menu_t* (menu_ctor)(const font_t *fnt);

int (menu_add_item)(menu_t *menu, const char *s);

int (menu_update_state)(menu_t *menu, int click);

void (menu_draw)(menu_t *menu);

void (menu_dtor)(menu_t *p);

/**
 * @}
 */

#endif //MENU_H_INCLUDED
