#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

/**
 * @defgroup    text_t  text_t
 * @ingroup font_t
 * @brief Text module.
 *
 * Allows to draw text in screen.
 * @{
 */

#include "font.h"

/**
 * @brief Enumeration of vertical alignment schemes.
 */
typedef enum{
    text_valign_top    = -1,
    text_valign_center =  0,
    text_valign_bottom =  1
} text_valign;
/**
 * @brief Enumeration of horizontal alignment schemes.
 */
typedef enum{
    text_halign_left   = -1,
    text_halign_center =  0,
    text_halign_right  =  1
} text_halign;

/**
 * @brief Drawable text.
 */
typedef struct text text_t;
/**
 * @brief Construct text.
 * @param   fnt Font to draw the text
 * @param   txt String to be drawn
 * @return      Pointer to constructed text
 */
text_t* (text_ctor)(const font_t *fnt, const char *txt);
/**
 * @brief Destruct text.
 * @param   p   Pointer to text to be destroyed
 */
void    (text_dtor)(text_t *p);
/**
 * @brief Set text string.
 * @param   p   Pointer to text
 * @param   txt String to be set
 */
void (text_set_string)(text_t *p, const char *txt);
/**
 * @brief Set text position.
 * @param   p   Pointer to text
 * @param   x   X-position of the text
 * @param   y   Y-position of the text
 */
void (text_set_pos)   (text_t *p, int16_t x, int16_t y);
/**
 * @brief Set text size (letter size, in pixels).
 * @param   p       Pointer to text
 * @param   size    Letter size of the text
 */
void (text_set_size)  (text_t *p, uint16_t size);
/**
 * @brief Set text color.
 * @param   p       Pointer to text
 * @param   color   Text color
 */
void (text_set_color) (text_t *p, uint32_t color);
/**
 * @brief Set text vertical alignment.
 * @param   p       Pointer to text
 * @param   valign  Vertical alignment of the text
 */
void (text_set_valign)(text_t *p, text_valign valign);
/**
 * @brief Set text horizontal alignment.
 * @param   p       Pointer to text
 * @param   halign  Horizontal alignment of the text
 */
void (text_set_halign)(text_t *p, text_halign halign);
/**
 * @brief Get string of the text.
 * @param   p       Pointer to text
 * @return  Pointer to string
 */
const char* (text_get_string)(const text_t *p);
/**
 * @brief Get text x-position.
 * @param   p   Pointer to text
 * @return      X-position of text
 */
int16_t (text_get_x)  (const text_t *p);
/**
 * @brief Get text y-position.
 * @param   p   Pointer to text
 * @return      Y-position of text
 */
int16_t (text_get_y)  (const text_t *p);
/**
 * @brief Draw text on the screen buffer.
 * @param   p   Pointer to text
 * @return      SUCCESS if operation was successful, false otherwise
 */
int     (text_draw)   (const text_t *p);

/**
 * @}
 */

#endif //TEXT_H_INCLUDED
