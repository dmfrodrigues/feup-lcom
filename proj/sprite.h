#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

struct sprite;
typedef struct sprite sprite_t;

sprite_t* sprite_ctor(const char **xpm, int u0, int v0);
void      sprite_dtor(sprite_t *p);

void sprite_set_x     (sprite_t *p, int x);
void sprite_set_y     (sprite_t *p, int y);
void sprite_set_pos   (sprite_t *p, int x, int y);
void sprite_set_angle (sprite_t *p, double angle);
void sprite_set_center(sprite_t *p, int u0, int v0);

int sprite_get_w(const sprite_t *p);
int sprite_get_h(const sprite_t *p);

void sprite_draw(const sprite_t *p);

#endif //SPRITE_H_INCLUDED
