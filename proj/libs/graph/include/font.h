#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

/**
 * @defgroup    font_t    font_t
 * @ingroup graph
 * @brief   Font module.
 *
 * Represents a set of glyphs that can be drawn for showing text.
 *
 * @{
 */

/**
 * @brief Font.
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
 * @}
 */

#endif //FONT_H_INCLUDED
