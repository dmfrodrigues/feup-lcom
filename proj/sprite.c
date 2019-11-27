#include <lcom/lcf.h>

#include "sprite.h"

#include "graphics.h"
#include <math.h>

struct sprite{
    int x, y;
    int w, h;
    int u0, v0;
    double theta;
    uint8_t *map;
};

sprite_t* sprite_ctor(const char **xpm, int u0, int v0){
    sprite_t *ret = (sprite_t*)malloc(sizeof(sprite_t));
    if(ret == NULL) return NULL;
    enum xpm_image_type type = XPM_8_8_8_8;
    xpm_image_t img;
    ret->map = xpm_load((xpm_map_t)xpm, type, &img);
    if(ret->map == NULL){
        free(ret);
        return NULL;
    }
    ret->x = 0;
    ret->y = 0;
    ret->w = img.width;
    ret->h = img.height;
    ret->u0 = u0;
    ret->v0 = u0;
    ret->theta = 0;
    return ret;
}
void sprite_dtor(sprite_t *p){
    if(p == NULL) return;
    if(p->map) free(p->map);
    free(p);
}

void sprite_set_x(sprite_t *p, int x){ p->x = x; }
void sprite_set_y(sprite_t *p, int y){ p->y = y; }
void sprite_set_pos(sprite_t *p, int x, int y){ sprite_set_x(p, x); sprite_set_y(p, y); }
void sprite_set_angle(sprite_t *p, double angle){ p->theta = angle; }
void sprite_set_center(sprite_t *p, int u0, int v0){ p->u0 = u0; p->v0 = v0; }

int sprite_get_w(const sprite_t *p){ return p->w; }
int sprite_get_h(const sprite_t *p){ return p->h; }

void sprite_src2pic(const sprite_t *p, int x, int y, int *u, int *v){
    int dx = x - p->x;
    int dy = y - p->y;
    //int du = dx*cos(p->theta) - dy*sin(p->theta);
    //int dv = dx*sin(p->theta) + dy*cos(p->theta);
    //*u = du + p->u0;
    //*v = dv + p->v0;
    //*u = dx*cos(p->theta) - dy*sin(p->theta) + p->u0;
    //*v = dx*sin(p->theta) + dy*cos(p->theta) + p->v0;
    double s = sin(p->theta);
    double c = cos(p->theta);
    *u = dx*c - dy*s + p->u0;
    *v = dx*s + dy*c + p->v0;
}

void sprite_draw(const sprite_t *p){
    const int diag = sqrt(p->w*p->w + p->h*p->h)+2;
    int u, v;
    for(int y = p->y - diag; y < p->y + diag; ++y){
        for(int x = p->x - diag; x < p->x + diag; ++x){
            if (p->x + x < get_XRes() && p->y + y < get_YRes()){
                sprite_src2pic(p, x, y, &u, &v);
                if(0 <= u && u < p->w && 0 <= v && v < p->h){
                    uint8_t *m = p->map + (v*p->w + u)*4;
                    uint32_t color = SET_RGB(*(m+2), *(m+1), *(m));
                    set_pixel_alpha(p->x + x, p->y + y, color, *(m+3));
                }
            }
        }
    }
}
