#include <lcom/lcf.h>

#include "sprite.h"

#include "graph.h"
#include "utils.h"
#include "fast_math.h"
#include <math.h>

struct sprite{
    int16_t x, y;
    uint16_t w, h;
    int16_t u0, v0;
    double theta;
    double scale;
    uint8_t *map;
};

sprite_t* (sprite_ctor)(const char **xpm, int u0, int v0){
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
    ret->scale = 1;
    return ret;
}
void (sprite_dtor)(sprite_t *p){
    if(p == NULL) return;
    free(p->map);
    free(p);
}

void (sprite_set_pos)   (sprite_t *p, int16_t x , int16_t y ){ p->x = x; p->y = y; }
void (sprite_set_center)(sprite_t *p, int16_t u0, int16_t v0){ p->u0 = u0; p->v0 = v0; }
void (sprite_set_angle) (sprite_t *p, double angle          ){ p->theta = angle; }
void (sprite_set_scale) (sprite_t *p, double scale          ){ p->scale = scale; }
int16_t  (sprite_get_x)(const sprite_t *p){ return p->x; }
int16_t  (sprite_get_y)(const sprite_t *p){ return p->y; }
uint16_t (sprite_get_w)(const sprite_t *p){ return p->w; }
uint16_t (sprite_get_h)(const sprite_t *p){ return p->h; }

void (sprite_src2pic)(const sprite_t *p, int16_t x, int16_t y, int16_t *u, int16_t *v){
    double s = fm_sin(p->theta);
    double c = fm_cos(p->theta);
    double dx = (x - p->x)/p->scale;
    double dy = (y - p->y)/p->scale;
    int16_t du = dx*c - dy*s + 0.5;
    int16_t dv = dx*s + dy*c + 0.5;
    *u = du + p->u0;
    *v = dv + p->v0;
}

void (sprite_pic2src)(const sprite_t *p, int16_t u, int16_t v, int16_t *x, int16_t *y){
    double s = fm_sin(p->theta);
    double c = fm_cos(p->theta);
    int16_t du = u - p->u0;
    int16_t dv = v - p->v0;
    double dx =  du*c + dv*s;
    double dy = -du*s + dv*c;
    *x = dx*p->scale + 0.5 + p->x;
    *y = dy*p->scale + 0.5 + p->y;
}

void (sprite_draw)(const sprite_t *p){
    int16_t xmin, xmax, ymin, ymax; {
        int16_t x, y;
        sprite_pic2src(p, 0   , 0   , &x, &y);
        xmin = x; xmax = x; ymin = y; ymax = y;
        sprite_pic2src(p, p->w, 0   , &x, &y);
        xmin = min(x, xmin); xmax = max(x, xmax); ymin = min(y, ymin); ymax = max(y, ymax);
        sprite_pic2src(p, 0   , p->h, &x, &y);
        xmin = min(x, xmin); xmax = max(x, xmax); ymin = min(y, ymin); ymax = max(y, ymax);
        sprite_pic2src(p, p->w, p->h, &x, &y);
        xmin = min(x, xmin); xmax = max(x, xmax); ymin = min(y, ymin); ymax = max(y, ymax);
        xmin = max(xmin-2, 0); xmax = min(xmax+2, graph_get_XRes());
        ymin = max(ymin-2, 0); ymax = min(ymax+2, graph_get_YRes());
    }
    int16_t u, v;
    for(int16_t y = ymin; y < ymax; ++y){
        for(int16_t x = xmin; x < xmax; ++x){
    //const int16_t diag = p->scale*sqrt(p->w*p->w + p->h*p->h)+2;
    //for(int16_t y = max(0,p->y-diag); y < min(p->y+diag,graph_get_YRes()); ++y){
    //    for(int16_t x = max(0,p->x-diag); x < min(p->x+diag,graph_get_XRes()); ++x){
            sprite_src2pic(p, x, y, &u, &v);
            if(0 <= u && u < p->w && 0 <= v && v < p->h){
                uint32_t c = *(uint32_t*)(p->map + (v*p->w + u)*4);
                graph_set_pixel_alpha(x, y, GET_COLOR(c), GET_ALP(c));
            }
        }
    }
}
