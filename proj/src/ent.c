#include <lcom/lcf.h>

#include "ent.h"

#include "graph.h"
#include "sprite.h"
#include "utils.h"
#include <math.h>

static double scale = 1.0;
static int16_t x_origin = 0;
static int16_t y_origin = 0;

void (ent_set_scale) (double n){ scale = n; }
void (ent_set_origin)(double x, double y){ x_origin = x; y_origin = y; }
double (ent_get_scale)  (void){ return scale; }
double (ent_get_XLength)(void){ return graph_get_XRes()/scale; }
double (ent_get_YLength)(void){ return graph_get_YRes()/scale; }

struct gunner{
    double x, y; //real position
    sprite_t *dude;
    sprite_t *weapon;
};
gunner_t* (gunner_ctor)(basic_sprite_t *dude, basic_sprite_t *weapon){
    gunner_t *ret = malloc(sizeof(gunner_t));
    if(ret == NULL) return NULL;
    ret->x = 0.0;
    ret->y = 0.0;
    ret->dude   = sprite_ctor(dude  );
    ret->weapon = sprite_ctor(weapon);
    if(ret->dude == NULL || ret->weapon == NULL){
        gunner_dtor(ret);
        return NULL;
    } else return ret;
}
void (gunner_dtor)(gunner_t *p){
    if(p == NULL) return;
    sprite_dtor(p->dude);
    sprite_dtor(p->weapon);
    free(p);
}
void (gunner_set_pos)  (gunner_t *p, double x, double y){ p->x = x; p->y = y; }
void (gunner_set_angle)(gunner_t *p, double angle      ){
    sprite_set_angle(p->dude  , angle);
    sprite_set_angle(p->weapon, angle);
}
double  (gunner_get_x)       (const gunner_t *p){ return p->x; }
double  (gunner_get_y)       (const gunner_t *p){ return p->y; }
int16_t (gunner_get_x_screen)(const gunner_t *p){ return (p->x-x_origin)*scale; }
int16_t (gunner_get_y_screen)(const gunner_t *p){ return (p->y-y_origin)*scale; }
void (gunner_draw)(gunner_t *p){
    const int16_t x_screen = gunner_get_x_screen(p);
    const int16_t y_screen = gunner_get_y_screen(p);
    sprite_set_pos  (p->dude  , x_screen, y_screen);
    sprite_set_pos  (p->weapon, x_screen, y_screen);
    sprite_set_scale(p->dude  , scale);
    sprite_set_scale(p->weapon, scale);
    sprite_draw     (p->weapon);
    sprite_draw     (p->dude  );
}

struct bullet{
    double x, y; //real position
    double vx, vy;
    sprite_t *b;
};
bullet_t* (bullet_ctor)(basic_sprite_t *b){
    bullet_t *ret = malloc(sizeof(bullet_t));
    if(ret == NULL) return NULL;
    ret-> x = 0.0;
    ret-> y = 0.0;
    ret->vx = 0.0;
    ret->vy = 0.0;
    ret->b = sprite_ctor(b);
    if(ret->b == NULL){
        bullet_dtor(ret);
        return NULL;
    }else return ret;
}
void (bullet_dtor)(bullet_t *p){
    if(p == NULL) return;
    sprite_dtor(p->b);
    free(p);
}
void (bullet_set_pos)  (bullet_t *p, double x, double y){ p->x = x; p->y = y; }
void (bullet_set_angle)(bullet_t *p, double angle      ){ sprite_set_angle(p->b, angle); }
double  (bullet_get_x)       (const bullet_t *p){ return p->x; }
double  (bullet_get_y)       (const bullet_t *p){ return p->y; }
int16_t (bullet_get_x_screen)(const bullet_t *p){ return (p->x-x_origin)*scale; }
int16_t (bullet_get_y_screen)(const bullet_t *p){ return (p->y-y_origin)*scale; }
void (bullet_update_movement)(bullet_t *p){
    p->x += p->vx;
    p->y += p->vy;
}
void (bullet_draw)(bullet_t *p){
    const int16_t x_screen = bullet_get_x_screen(p);
    const int16_t y_screen = bullet_get_y_screen(p);
    sprite_set_pos  (p->b, x_screen, y_screen);
    sprite_set_scale(p->b, scale);
    sprite_draw     (p->b);
}

struct map{
    basic_sprite_t *bsp_background;
    sprite_t *background;
    uint8_t *collide;
};
map_t* (map_ctor)(const char **background, const char **collide){
    map_t *ret = malloc(sizeof(map_t));
    if(ret == NULL) return NULL;

    ret->bsp_background = NULL;
    ret->background     = NULL;
    ret->collide        = NULL;

    ret->bsp_background = basic_sprite_ctor(background, 0, 0);
    ret->background     = sprite_ctor(ret->bsp_background);
    if(ret->bsp_background == NULL ||
        ret->background     == NULL){ map_dtor(ret); return NULL; }

    basic_sprite_t *bsp_collide = basic_sprite_ctor(collide, 0, 0);
    if(bsp_collide == NULL){ map_dtor(ret); return NULL; }
    const uint16_t W = basic_sprite_get_w(bsp_collide);
    const uint16_t H = basic_sprite_get_h(bsp_collide);
    ret->collide = malloc(W*H*sizeof(uint8_t));
    if(ret->collide == NULL){ map_dtor(ret); return NULL; }
    const uint8_t *m = basic_sprite_get_map(bsp_collide);
    for(unsigned i = 0; i < W*H; ++i){
        ret->collide[i] = (m[4*i+3] < ALPHA_THRESHOLD ? 1 : 0);
    }
    basic_sprite_dtor(bsp_collide);

    return ret;
}
void (map_dtor)(map_t *p){
    if(p == NULL) return;
    sprite_dtor(p->background);
    basic_sprite_dtor(p->bsp_background);
    free(p->collide);
    free(p);
}
int16_t (map_get_x_screen)(const map_t *p){ return (-x_origin)*scale; }
int16_t (map_get_y_screen)(const map_t *p){ return (-y_origin)*scale; }
int (map_collides_point)(const map_t *p, double x, double y){
    const uint16_t w = sprite_get_w(p->background), h = sprite_get_h(p->background);
    int16_t x_ = x, y_ = y;
    if(x_ < 0 || w <= x_ || y_ < 0 || h <= y_) return 0;
    uint32_t pos = x_ + y_*w;
    return p->collide[pos];
}

int (map_collides_gunner)(const map_t *p, const gunner_t *shooter) {
    double radius = sprite_get_w(shooter->dude)/2.0;
    double shooter_x = gunner_get_x(shooter);
    double shooter_y = gunner_get_y(shooter);
    for (double x = -radius; x < radius; x += 1) {
        double y1 = sqrt(radius*radius - x*x);
        double y2 = -y1;
        if (map_collides_point(p, shooter_x + x, shooter_y + y1) || map_collides_point(p, shooter_x + x, shooter_y + y2)) return 1;
    }
    return 0;
}

int (map_collides_bullet)(const map_t *p, const bullet_t *bull){
    double radius = max(sprite_get_w(bull->b), sprite_get_h(bull->b))/2.0;
    double bullet_x = bullet_get_x(bull);
    double bullet_y = bullet_get_y(bull);
    for (double x = -radius; x < radius; x += 1){
        double y1 = sqrt(radius*radius - x*x);
        double y2 = -y1;
        if (map_collides_point(p, bullet_x + x, bullet_y + y1) || map_collides_point(p, bullet_x + x, bullet_y + y2)) return 1;
    }
    return 0;
}

void   (map_draw)(map_t *p){
    const int16_t x_screen = map_get_x_screen(p);
    const int16_t y_screen = map_get_y_screen(p);
    sprite_set_pos  (p->background, x_screen, y_screen);
    sprite_set_scale(p->background, scale);
    sprite_draw     (p->background);
}
