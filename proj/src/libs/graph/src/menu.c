#include <lcom/lcf.h>

#include "menu.h"

#include "graph.h"
#include "rectangle.h"
#include "proj_func.h"
#include "proj_macros.h"
#include "errors.h"

static const int W  = 400;
static const int H  = 60;
static const int W1 = 40;
static const int H1 = 40;
static const int H2 = 20;

struct menu {
    const font_t *fnt;
    size_t sz;
    rectangle_t **r;
    text_t      **t;
    rectangle_t *frame;
};

static int menu_recalculate(menu_t *menu){
    const uint16_t Wtotal = 2*W1+W;
    const uint16_t Htotal = (uint16_t)(2*H1+menu->sz*H+(menu->sz-1)*H2);
    int16_t x = graph_get_XRes()/2-Wtotal/2;
    int16_t y = graph_get_YRes()/2-Htotal/2;
    rectangle_set_pos (menu->frame, x, y);
    rectangle_set_size(menu->frame, Wtotal, Htotal);
    x += W1; y += H1;
    for(size_t i = 0; i < menu->sz; ++i, y+=H+H2){
        rectangle_set_pos(menu->r[i], x, y);
        text_set_pos     (menu->t[i], x+W/2, y+H/2);
    }
    return SUCCESS;
}

menu_t* (menu_ctor)(const font_t *fnt){
    if(fnt == NULL) return NULL;
    menu_t *ret = (menu_t*)malloc(sizeof(menu_t));
    if (ret == NULL) return NULL;
    ret->fnt = fnt;
    ret->sz = 0;
    ret->r = NULL;
    ret->t = NULL;

    ret->frame = rectangle_ctor(0, 0, 0, 0);
    rectangle_set_fill_color(ret->frame, GRAPH_BLACK);
    rectangle_set_outline_width(ret->frame, 6);
    rectangle_set_outline_color(ret->frame, GRAPH_WHITE);
    rectangle_set_fill_trans(ret->frame, GRAPH_TRANSPARENT);

    menu_recalculate(ret);

    return ret;
}

int (menu_add_item)(menu_t *menu, const char *s){
    rectangle_t *r = rectangle_ctor(0, 0, W, H);
    text_t      *t = text_ctor(menu->fnt, s);
    if(r == NULL || t == NULL){
        rectangle_dtor(r);
        text_dtor(t);
        return NULL_PTR;
    }
    rectangle_set_fill_color   (r, GRAPH_BLACK);
    rectangle_set_outline_width(r, 2);
    rectangle_set_outline_color(r, GRAPH_WHITE);

    text_set_valign(t, text_valign_center);
    text_set_halign(t, text_halign_center);
    text_set_color (t, TEXT_COLOR);

    ++menu->sz;
    menu->r = realloc(menu->r, menu->sz*sizeof(rectangle_t*));
    menu->t = realloc(menu->t, menu->sz*sizeof(text_t     *));
    menu->r[menu->sz-1] = r;
    menu->t[menu->sz-1] = t;

    return menu_recalculate(menu);
}

int (menu_update_state)(menu_t *menu, int click) {
    if(!click) return -1;
    for(size_t i = 0; i < menu->sz; ++i)
        if(rectangle_collide_point(menu->r[i], *get_mouse_X(), *get_mouse_Y()))
            return (int)i;
    return -1;
}

void (menu_draw)(menu_t *menu) {
    rectangle_draw(menu->frame);
    int x = *get_mouse_X(), y = *get_mouse_Y();
    for(size_t i = 0; i < menu->sz; ++i){
        if(rectangle_collide_point(menu->r[i], x, y))
            rectangle_set_fill_color(menu->r[i], HIGHLIGHT_COLOR);
        rectangle_draw(menu->r[i]);
        text_draw     (menu->t[i]);
    }
    for(size_t i = 0; i < menu->sz; ++i)
        if(rectangle_collide_point(menu->r[i], x, y))
            rectangle_set_fill_color(menu->r[i], GRAPH_BLACK);
}

void (menu_dtor)(menu_t *p){
    for(size_t i = 0; i < p->sz; ++i){
        rectangle_dtor(p->r[i]);
        text_dtor     (p->t[i]);
    }
    rectangle_dtor(p->frame);
    free(p);
}
