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

struct sprite;
typedef struct sprite sprite_t;

sprite_t* (sprite_ctor)(const basic_sprite_t *bsp);
/*
 * /!\ WARNING: Entity destructor does not destruct the basic_sprite passed as
 * constructor arguments, since it is assumed the same basic_sprite may be used to
 * draw several sprites. It is thus YOUR responsibility to delete basic_sprite's.
 * @param   p   Pointer to sprite_t to be destructed
 */
void      (sprite_dtor)(sprite_t *p);

void (sprite_set_pos)   (sprite_t *p, int16_t x, int16_t y);
void (sprite_set_center)(sprite_t *p, int16_t u0, int16_t v0);
void (sprite_set_angle) (sprite_t *p, double angle);
void (sprite_set_scale) (sprite_t *p, double scale);

int16_t  (sprite_get_x)(const sprite_t *p);
int16_t  (sprite_get_y)(const sprite_t *p);
double   (sprite_get_angle)(const sprite_t *p);
uint16_t (sprite_get_w)(const sprite_t *p);
uint16_t (sprite_get_h)(const sprite_t *p);

void (sprite_draw)(const sprite_t *p);

/**
 * @}
 */

#endif //SPRITE_H_INCLUDED
