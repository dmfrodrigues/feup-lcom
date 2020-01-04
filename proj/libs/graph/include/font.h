#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

struct font;
/**
 * @brief Font. Represents a set of glyphs that can be drawn for showing text.
 */
typedef struct font font_t;
/**
 * @brief Construct font.
 *
 * The font is constructed from a provided path. That path is required to
 * contain glyphs with the name "ascii???.xpm2", files with format XPM2 where
 * instead of having "???" we have 3-digit ASCII codes, where the glyph with
 * that name map to that XPM2 file.
 * @param   s   Path to directory
 * @return      Pointer to font
 */
font_t* (font_ctor)(const char *s);
/**
 * @brief Destruct font.
 * @param   p   Pointer to font to be destroyed
 */
void    (font_dtor)(font_t *p);

/**
 * @brief Initialize font stuff.
 *
 * More specifically, this involves loading the default font, which is currently consolas.
 * @return  SUCCESS if initialization was successful, false otherwise
 */
int (font_init)(void);
/**
 * @brief Get default font.
 *
 * Currently, the default font is Consolas.
 * @return  Pointer to default font
 */
const font_t* font_get_default(void);
/**
 * @brief Get Consolas font.
 * @return Pointer to Consolas font
 */
const font_t* font_get_consolas(void);
/**
 * @brief Free resources used on initialization of fonts.
 */
void (font_free)(void);

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

struct text;
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

#endif //FONT_H_INCLUDED
