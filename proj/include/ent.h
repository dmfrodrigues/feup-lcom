#ifndef ENT_H_INCLUDED
#define ENT_H_INCLUDED

#include "basic_sprite.h"
#include "list.h"

void (ent_set_scale) (double n);
void (ent_set_origin)(double x, double y);

double (ent_get_scale)  (void);
double (ent_get_XLength)(void);
double (ent_get_YLength)(void);

#define GUNNER_MELEE    BIT(0)
#define GUNNER_RANGED   BIT(1)
#define GUNNER_PLAYER   BIT(2)
#define GUNNER_FOLLOW   BIT(3)

struct gunner;
typedef struct gunner gunner_t;
gunner_t* (gunner_ctor)(basic_sprite_t *dude, basic_sprite_t *weapon, uint16_t type, int team);
void      (gunner_dtor)(gunner_t *p);
void (gunner_set_pos)               (gunner_t *p, double x, double y);
void (gunner_set_spawn)             (gunner_t *p, double x, double y);
void (gunner_set_angle)             (gunner_t *p, double angle      );
void (gunner_set_health)            (gunner_t *p, double health);
void (gunner_set_curr_health)       (gunner_t *p, double health);
double  (gunner_get_x)              (const gunner_t *p);
double  (gunner_get_y)              (const gunner_t *p);
double  (gunner_get_spawn_x)        (const gunner_t *p);
double  (gunner_get_spawn_y)        (const gunner_t *p);
double  (gunner_get_angle)          (const gunner_t *p);
double  (gunner_get_health)         (const gunner_t *p);
double  (gunner_get_curr_health)    (const gunner_t *p);
int16_t (gunner_get_x_screen)       (const gunner_t *p);
int16_t (gunner_get_y_screen)       (const gunner_t *p);
uint16_t (gunner_get_type)          (const gunner_t *p);
int     (gunner_get_team)           (const gunner_t *p);
void (gunner_draw)(gunner_t *p);
void (gunner_draw_health)(const gunner_t *p);

double (gunner_distance)(const gunner_t *p1, const gunner_t *p2);

struct bullet;
typedef struct bullet bullet_t;
bullet_t* (bullet_ctor)(const gunner_t *shooter, const basic_sprite_t *b, double x, double y, double vx, double vy);
void      (bullet_dtor)(bullet_t *p);
double  (bullet_get_x)       (const bullet_t *p);
double  (bullet_get_y)       (const bullet_t *p);
double  (bullet_get_vx)      (const bullet_t *p);
double  (bullet_get_vy)      (const bullet_t *p);
int16_t (bullet_get_x_screen)(const bullet_t *p);
int16_t (bullet_get_y_screen)(const bullet_t *p);
double  (bullet_get_damage)  (const bullet_t *p);
void    (bullet_set_damage)  (bullet_t *p, double damage);
const gunner_t* (bullet_get_shooter)(const bullet_t *p);
void (bullet_update_movement)(bullet_t *p);
void (bullet_update_movement_list)(list_t *bullet_list);
void (bullet_draw)(bullet_t *p);

void (gunner_draw_list)(list_t *shooter_list);
void (bullet_draw_list)(list_t *bullet_list);
void (gunner_draw_health)(const gunner_t *p);

struct map;
typedef struct map map_t;
map_t* (map_ctor)(const char *const *background, const char *const *collide);
void   (map_dtor)(map_t *p);
int    (map_collides_point)(const map_t *p, double x, double y);
int    (map_collides_gunner)(const map_t *p, const gunner_t *gunner);
int    (map_collides_bullet)(const map_t *p, const bullet_t *bullet);
uint16_t (map_get_width)   (const map_t *p);
uint16_t (map_get_height)  (const map_t *p);
int (map_make_dijkstra)(map_t *p, double x_, double y_);
int (map_where_to_follow)(const map_t *p, double x, double y, double *theta);
int (gunner_collides_bullet)(const gunner_t *shooter, const bullet_t *bull);
double (distance_gunners)(const gunner_t *shooter1, const gunner_t *shooter2);
int (gunner_collides_gunner)(const gunner_t *shooter1, const gunner_t *shooter2);
void   (map_draw)(map_t *p);

#endif //ENT_H_INCLUDED
