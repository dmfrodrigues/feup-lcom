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

/**
 * @brief Menu.
 */
typedef struct menu menu_t;
/**
 * @brief Construct menu.
 * @param   fnt Font to use when rendering menu options text
 * @return      Pointer to constructed menu, or NULL if failed.
 */
menu_t* (menu_ctor)(const font_t *fnt);
/**
 * @brief Destruct menu.
 * @param   p   Pointer to menu to destruct
 */
void (menu_dtor)(menu_t *p);
/**
 * @brief Add item to menu.
 * @param   menu    Pointer to menu
 * @param   s       String to be used as text for the new item
 * @return  SUCCESS if operation was successful, other value otherwise
 */
int (menu_add_item)(menu_t *menu, const char *s);
/**
 * @brief Update menu state.
 *
 * This function allows to check if the mouse is hovering over an item, and knowing
 * if an item was clicked.
 * @param   menu    Pointer to menu
 * @param   click   0 if mouse right button is clicked, other value otherwise
 * @return  selected option if clicked, -1 otherwise
 */
int (menu_update_state)(menu_t *menu, int click);
/**
 * @brief Draw menu on screen buffer.
 * @param   menu    Pointer to menu to be drawn
 */
void (menu_draw)(menu_t *menu);

/**
 * @}
 */

#endif //MENU_H_INCLUDED
