#include <lcom/lcf.h>

#include "ent.h"

#include "graph.h"

static double scale = 1.0;
static int16_t x_origin = 0;
static int16_t y_origin = 0;

void (ent_set_scale) (double n){ scale = n; }
void (ent_set_origin)(double x, double y){ x_origin = x; y_origin = y; }

double (ent_get_XLength)(void){ return graph_get_XRes()/scale; }
double (ent_get_YLength)(void){ return graph_get_YRes()/scale; }

struct ent{
    double x, y; //real position in meters
    sprite_t *dude;
    sprite_t *weapon;
};

ent_t* (ent_ctor)(basic_sprite_t *dude, basic_sprite_t *weapon){
    ent_t *ret = malloc(sizeof(ent_t));
    if(ret == NULL) return NULL;
    ret->x = 0.0;
    ret->y = 0.0;
    ret->dude   = sprite_ctor(dude  );
    ret->weapon = sprite_ctor(weapon);
    if(ret->dude == NULL || ret->weapon == NULL){
        ent_dtor(ret);
        return NULL;
    } else return ret;
}
void (ent_dtor)(ent_t *p){
    if(p == NULL) return;
    sprite_dtor(p->dude);
    sprite_dtor(p->weapon);
    free(p);
}

void (ent_set_pos)  (ent_t *p, int16_t x, int16_t y){ p->x = x; p->y = y; }
void (ent_set_angle)(ent_t *p, double angle        ){
    sprite_set_angle(p->dude  , angle);
    sprite_set_angle(p->weapon, angle);
}

double  (ent_get_x)       (const ent_t *p){ return p->x; }
double  (ent_get_y)       (const ent_t *p){ return p->y; }
int16_t (ent_get_x_screen)(const ent_t *p){ return (p->x-x_origin)*scale; }
int16_t (ent_get_y_screen)(const ent_t *p){ return (p->y-y_origin)*scale; }

void (ent_draw)(ent_t *p){
    const int16_t x_screen = ent_get_x_screen(p);
    const int16_t y_screen = ent_get_y_screen(p);
    sprite_set_pos  (p->dude  , x_screen, y_screen);
    sprite_set_pos  (p->weapon, x_screen, y_screen);
    sprite_set_scale(p->dude  , scale);
    sprite_set_scale(p->weapon, scale);
    sprite_draw     (p->weapon);
    sprite_draw     (p->dude  );
}
