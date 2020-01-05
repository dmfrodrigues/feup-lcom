#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

/**
 * @defgroup sprite_t sprite_t
 * @ingroup graph
 * @brief Sprite module.
 *
 * A sprite allows to draw a basic_sprite_t, while also providing some transformations for that basic_sprite_t.
 *
 * @{
 */

#include "basic_sprite.h"

/**
 * @brief Basic sprite.
 */
typedef struct sprite sprite_t;
/**
 * @brief Construct sprite.
 * @param   bsp Basic sprite from which to build the sprite
 * @return      Pointer to constructed sprite, or NULL if failed
 */
sprite_t* (sprite_ctor)(const basic_sprite_t *bsp);
/**
 * @brief Destruct sprite.
 *
 *
 * @attention Destructor does not destruct the basic sprite passed as
 * constructor argument, since it is assumed the same basic sprite may be used to
 * draw several sprites. It is thus YOUR responsibility to delete basic sprites.
 * @param   p   Pointer to sprite_t to be destructed
 */
void      (sprite_dtor)(sprite_t *p);
/**
 * @brief Set sprite position.
 * @param   p   Pointer to sprite
 * @param   x   X-position in map
 * @param   y   Y-position in map
 */
void (sprite_set_pos)   (sprite_t *p, int16_t x, int16_t y);
/**
 * @brief Set sprite angle.
 *
 * If not provided, will be drawn with angle 0, corresponding to the same
 * orientation as the original image.
 * @param   p       Pointer to sprite
 * @param   angle   Angle in radians, counterclockwise positive and 0 on positive x-axis
 */
void (sprite_set_angle) (sprite_t *p, double angle);
/**
 * @brief Set sprite scale.
 *
 * Default scale is 1, corresponding to original image scale.
 * @param   p       Pointer to sprite
 * @param   scale   Scale, greater than 0
 */
void (sprite_set_scale) (sprite_t *p, double scale);
/**
 * @brief Get sprite angle.
 * @param   p   Pointer to sprite
 * @return      Sprite angle
 */
double   (sprite_get_angle)(const sprite_t *p);
/**
 * @brief   Get sprite width (same as the underlying basic sprite).
 * @param   p   Pointer to sprite
 * @return      Sprite width
 */
uint16_t (sprite_get_w)(const sprite_t *p);
/**
 * @brief   Get sprite height (same as the underlying basic sprite).
 * @param   p   Pointer to sprite
 * @return      Sprite height
 */
uint16_t (sprite_get_h)(const sprite_t *p);
/**
 * @brief Draw sprite in buffer.
 * @param   p   Pointer to sprite
 */
void (sprite_draw)(const sprite_t *p);

/**
 * @}
 */

#endif //SPRITE_H_INCLUDED
