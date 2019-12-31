#include <lcom/lcf.h>

#include "menu.h"

#include "graph.h"
#include "rectangle.h"
#include "proj_func.h"
#include "proj_macros.h"

struct menu {
    size_t sz;
    rectangle_t **r;
    text_t      **t;
    rectangle_t *frame;
};

menu_t* (menu_ctor)(const font_t *fnt){
    if(fnt == NULL) return NULL;
    menu_t *ret = (menu_t*)malloc(sizeof(menu_t));
    if (ret == NULL) return NULL;
    ret->sz = 0;
    ret->r = NULL;
    ret->t = NULL;

    ret->frame = rectangle_ctor(0, 0, 800, 500);
    rectangle_set_fill_color(ret->frame, GRAPH_BLACK);
    rectangle_set_outline_width(ret->frame, 6);
    rectangle_set_outline_color(ret->frame, GRAPH_WHITE);
    rectangle_set_fill_trans(ret->frame, GRAPH_TRANSPARENT);


    ret->sz = 3;
    ret->r = realloc(ret->r, ret->sz*sizeof(rectangle_t*));
    ret->t = realloc(ret->t, ret->sz*sizeof(text_t     *));
    ret->r[0] = rectangle_ctor(0, 0, 400, 100);
    ret->r[1] = rectangle_ctor(0, 0, 400, 100);
    ret->r[2] = rectangle_ctor(0, 0, 400, 100);
    ret->t[0] = text_ctor(fnt, "Play");
    ret->t[1] = text_ctor(fnt, "Test");
    ret->t[2] = text_ctor(fnt, "Exit");

    if (ret->r[0] == NULL || ret->r[1] == NULL || ret->r[2] == NULL ||
        ret->t[0] == NULL || ret->t[1] == NULL || ret->t[2] == NULL ||
        ret->frame == NULL) return NULL;
    // VISUAL
    rectangle_set_fill_color(ret->r[0], GRAPH_BLACK);
    rectangle_set_outline_width(ret->r[0], 2);
    rectangle_set_outline_color(ret->r[0], GRAPH_WHITE);
    rectangle_set_fill_trans(ret->r[0], GRAPH_TRANSPARENT);

    rectangle_set_fill_color(ret->r[1], GRAPH_BLACK);
    rectangle_set_outline_width(ret->r[1], 2);
    rectangle_set_outline_color(ret->r[1], GRAPH_WHITE);
    rectangle_set_fill_trans(ret->r[1], GRAPH_TRANSPARENT);

    rectangle_set_fill_color(ret->r[2], GRAPH_BLACK);
    rectangle_set_outline_width(ret->r[2], 2);
    rectangle_set_outline_color(ret->r[2], GRAPH_WHITE);
    rectangle_set_fill_trans(ret->r[2], GRAPH_TRANSPARENT);

    text_set_valign(ret->t[0], text_valign_center);
    text_set_halign(ret->t[0], text_halign_center);
    text_set_color(ret->t[0], TEXT_COLOR);

    text_set_valign(ret->t[1], text_valign_center);
    text_set_halign(ret->t[1], text_halign_center);
    text_set_color(ret->t[1], TEXT_COLOR);

    text_set_valign(ret->t[2], text_valign_center);
    text_set_halign(ret->t[2], text_halign_center);
    text_set_color(ret->t[2], TEXT_COLOR);
    // POSITIONS
    rectangle_set_pos(ret->r[0],
                      graph_get_XRes()/2    - rectangle_get_w(ret->r[0])/2,
                      graph_get_YRes()*0.35 - rectangle_get_h(ret->r[0])/2);


    rectangle_set_pos(ret->r[1],
                    graph_get_XRes()/2    - rectangle_get_w(ret->r[1])/2,
                    graph_get_YRes()*0.5 - rectangle_get_h(ret->r[1])/2);


    rectangle_set_pos(ret->r[2],
                  graph_get_XRes()/2    - rectangle_get_w(ret->r[2])/2,
                  graph_get_YRes()*0.65 - rectangle_get_h(ret->r[2])/2);

    text_set_pos(ret->t[0], rectangle_get_x(ret->r[0])+rectangle_get_w(ret->r[0])/2,
                          rectangle_get_y(ret->r[0])+rectangle_get_h(ret->r[0])/2);

    text_set_pos(ret->t[1], rectangle_get_x(ret->r[1])+rectangle_get_w(ret->r[1])/2,
                          rectangle_get_y(ret->r[1])+rectangle_get_h(ret->r[1])/2);

    text_set_pos(ret->t[2], rectangle_get_x(ret->r[2])+rectangle_get_w(ret->r[2])/2,
                          rectangle_get_y(ret->r[2])+rectangle_get_h(ret->r[2])/2);

    rectangle_set_pos(ret->frame,
                  graph_get_XRes()/2    - rectangle_get_w(ret->frame)/2,
                  graph_get_YRes()*0.5 - rectangle_get_h(ret->frame)/2);
    return ret;
}

static int highlighted = -1;

int (menu_update_state)(menu_t *menu, int click) {

    if (rectangle_collide_point(menu->r[0], *get_mouse_X(), *get_mouse_Y())) {
        highlighted = GAME;
        if (click) return GAME;
    } else if (rectangle_collide_point(menu->r[1], *get_mouse_X(), *get_mouse_Y())) {
        highlighted = TEST;
        if (click) return TEST;
    } else if (rectangle_collide_point(menu->r[2], *get_mouse_X(), *get_mouse_Y())) {
        highlighted = EXIT;
        if (click) return EXIT;
    } else {
        highlighted = -1;
    }
    return MENU;
}

void (menu_draw)(menu_t *menu) {
    rectangle_draw(menu->frame);
    switch (highlighted) {
    case GAME:
        rectangle_set_fill_color(menu->r[0], HIGHLIGHT_COLOR);
        rectangle_draw(menu->r[0]);
        rectangle_draw(menu->r[1]);
        rectangle_draw(menu->r[2]);
        rectangle_set_fill_color(menu->r[0], GRAPH_BLACK);
        break;
    case TEST:
        rectangle_set_fill_color(menu->r[1], HIGHLIGHT_COLOR);
        rectangle_draw(menu->r[0]);
        rectangle_draw(menu->r[1]);
        rectangle_draw(menu->r[2]);
        rectangle_set_fill_color(menu->r[1], GRAPH_BLACK);
        break;
    case EXIT:
        rectangle_set_fill_color(menu->r[2], HIGHLIGHT_COLOR);
        rectangle_draw(menu->r[0]);
        rectangle_draw(menu->r[1]);
        rectangle_draw(menu->r[2]);
        rectangle_set_fill_color(menu->r[2], GRAPH_BLACK);
        break;
    default:
        rectangle_draw(menu->r[0]);
        rectangle_draw(menu->r[1]);
        rectangle_draw(menu->r[2]);
        break;
    }
    for(size_t i = 0; i < menu->sz; ++i)
        text_draw(menu->t[i]);
}

void (menu_dtor)(menu_t *p){
    for(size_t i = 0; i < p->sz; ++i){
        rectangle_dtor(p->r[i]);
        text_dtor     (p->t[i]);
    }
    rectangle_dtor(p->frame);
    free(p);
}
