#ifndef ENT_H_INCLUDED
#define ENT_H_INCLUDED

#include "sprite.h"

void (ent_set_scale) (double n);
void (ent_set_origin)(double x, double y);

double (ent_get_scale)  (void);
double (ent_get_XLength)(void);
double (ent_get_YLength)(void);

struct gunner;
typedef struct gunner gunner_t;
gunner_t* (gunner_ctor)(basic_sprite_t *dude, basic_sprite_t *weapon);
void      (gunner_dtor)(gunner_t *p);
void (gunner_set_pos)  (gunner_t *p, int16_t x, int16_t y);
void (gunner_set_angle)(gunner_t *p, double angle        );
double  (gunner_get_x)       (const gunner_t *p);
double  (gunner_get_y)       (const gunner_t *p);
int16_t (gunner_get_x_screen)(const gunner_t *p);
int16_t (gunner_get_y_screen)(const gunner_t *p);
void (gunner_draw)(gunner_t *p);

struct bullet;
typedef struct bullet bullet_t;
bullet_t* (bullet_ctor)(basic_sprite_t *b);

#endif //ENT_H_INCLUDED
