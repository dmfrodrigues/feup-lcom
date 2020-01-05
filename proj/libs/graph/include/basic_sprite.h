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
struct basic_sprite;
typedef struct basic_sprite basic_sprite_t;

basic_sprite_t* (basic_sprite_ctor)(const char *const *xpm, int16_t u0, int16_t v0);
void            (basic_sprite_dtor)(basic_sprite_t *p);
const uint8_t* (basic_sprite_get_map)(const basic_sprite_t *p);
uint16_t       (basic_sprite_get_w)  (const basic_sprite_t *p);
uint16_t       (basic_sprite_get_h)  (const basic_sprite_t *p);
int16_t        (basic_sprite_get_u0) (const basic_sprite_t *p);
int16_t        (basic_sprite_get_v0) (const basic_sprite_t *p);

/**
 * @}
 */

#endif //BASIC_SPRITE_H_INCLUDED
