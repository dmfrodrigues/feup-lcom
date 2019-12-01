#ifndef ENT_H_INCLUDED
#define ENT_H_INCLUDED

#include "sprite.h"

void (ent_set_scale) (double n);
void (ent_set_origin)(double x, double y);

double (ent_get_scale)  (void);
double (ent_get_XLength)(void);
double (ent_get_YLength)(void);

struct ent;
typedef struct ent ent_t;

ent_t* (ent_ctor)(basic_sprite_t *dude, basic_sprite_t *weapon);
void   (ent_dtor)(ent_t *p);

void (ent_set_pos)  (ent_t *p, int16_t x, int16_t y);
void (ent_set_angle)(ent_t *p, double angle        );

double  (ent_get_x)       (const ent_t *p);
double  (ent_get_y)       (const ent_t *p);
int16_t (ent_get_x_screen)(const ent_t *p);
int16_t (ent_get_y_screen)(const ent_t *p);

void (ent_draw)(ent_t *p);

#endif //ENT_H_INCLUDED
