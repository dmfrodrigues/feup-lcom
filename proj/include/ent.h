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
void (gunner_set_pos)  (gunner_t *p, double x, double y);
void (gunner_set_angle)(gunner_t *p, double angle      );
double  (gunner_get_x)       (const gunner_t *p);
double  (gunner_get_y)       (const gunner_t *p);
int16_t (gunner_get_x_screen)(const gunner_t *p);
int16_t (gunner_get_y_screen)(const gunner_t *p);
void (gunner_draw)(gunner_t *p);

struct bullet;
typedef struct bullet bullet_t;
bullet_t* (bullet_ctor)(basic_sprite_t *b, double x, double y, double vx, double vy);
void      (bullet_dtor)(bullet_t *p);
double  (bullet_get_x)       (const bullet_t *p);
double  (bullet_get_y)       (const bullet_t *p);
int16_t (bullet_get_x_screen)(const bullet_t *p);
int16_t (bullet_get_y_screen)(const bullet_t *p);
void (bullet_update_movement)(bullet_t *p);
void (bullet_draw)(bullet_t *p);

struct map;
typedef struct map map_t;
map_t* (map_ctor)(const char **background, const char **collide);
void   (map_dtor)(map_t *p);
int    (map_collides_point)(const map_t *p, double x, double y);
int    (map_collides_gunner)(const map_t *p, const gunner_t *gunner);
int    (map_collides_bullet)(const map_t *p, const bullet_t *bullet);
void   (map_draw)(map_t *p);

#endif //ENT_H_INCLUDED
