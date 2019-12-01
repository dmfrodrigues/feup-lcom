#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

struct sprite;
typedef struct sprite sprite_t;

sprite_t* (sprite_ctor)(const char **xpm, int u0, int v0);
void      (sprite_dtor)(sprite_t *p);

void (sprite_set_pos)   (sprite_t *p, int16_t x, int16_t y);
void (sprite_set_center)(sprite_t *p, int16_t u0, int16_t v0);
void (sprite_set_angle) (sprite_t *p, double angle);
void (sprite_set_scale) (sprite_t *p, double scale);

int16_t  (sprite_get_x)(const sprite_t *p);
int16_t  (sprite_get_y)(const sprite_t *p);
uint16_t (sprite_get_w)(const sprite_t *p);
uint16_t (sprite_get_h)(const sprite_t *p);

void (sprite_draw)(const sprite_t *p);

#endif //SPRITE_H_INCLUDED
