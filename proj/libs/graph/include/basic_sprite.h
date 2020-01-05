#ifndef BASIC_SPRITE_H_INCLUDED
#define BASIC_SPRITE_H_INCLUDED

/**
 * @defgroup basic_sprite_t basic_sprite_t
 * @ingroup graph
 * @brief Basic sprite module.
 *
 * A basic sprite allows to represent an image file.
 *
 * @{
 */

/**
 * @brief Basic sprite.
 */
typedef struct basic_sprite basic_sprite_t;

/**
 * @brief Construct basic sprite.
 * @param   xpm     XPM array to build the basic sprite from
 * @param   u0      X-center of the XPM.
 * @param   v0      Y-center of the XPM.
 * @return  Pointer to created basic_sprite_t, or NULL if fails
 */
basic_sprite_t* (basic_sprite_ctor)(const char *const *xpm, int16_t u0, int16_t v0);
/**
 * @brief Destruct basic sprite.
 * @param   p   Pointer to basic sprite to destruct.
 */
void            (basic_sprite_dtor)(basic_sprite_t *p);
/**
 * @brief Get basic sprite map (pixels).
 * @param   p   Pointer to basic sprite
 * @return  Pixel map
 */
const uint8_t* (basic_sprite_get_map)(const basic_sprite_t *p);
/**
 * @brief Get width.
 * @param   p   Pointer to basic sprite
 * @return      Width
 */
uint16_t       (basic_sprite_get_w)  (const basic_sprite_t *p);
/**
 * @brief Get height.
 * @param   p   Pointer to basic sprite
 * @return      Height
 */
uint16_t       (basic_sprite_get_h)  (const basic_sprite_t *p);
/**
 * @brief Get X-center of the basic sprite.
 * @param   p   Pointer to basic sprite
 * @return      X-center
 */
int16_t        (basic_sprite_get_u0) (const basic_sprite_t *p);
/**
 * @brief Get Y-center of the basic sprite.
 * @param   p   Pointer to basic sprite
 * @return      Y-center
 */
int16_t        (basic_sprite_get_v0) (const basic_sprite_t *p);

/**
 * @}
 */

#endif //BASIC_SPRITE_H_INCLUDED
