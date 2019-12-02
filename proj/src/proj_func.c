#include <lcom/lcf.h>

#include "proj_func.h"

#include "interrupts_func.h"
#include "graph.h"
#include "keyboard.h"
#include "errors.h"
#include "proj_macros.h"
#include "utils.h"
#include "ent.h"

#include "kbc_macros.h"

#include <math.h>

int cleanup(void) {
    int r = SUCCESS;
    if ((r = unsubscribe_all()))
        printf("%s: failed to unsubscribe drivers.\n", __func__);
    if ((r = graph_cleanup()))
        printf("%s: graph cleanup failed\n", __func__);

    return r;
}

static int hor_mov = REST, ver_mov = REST;
static keys_t key_presses;

void update_key_presses(void) {
    if (sz == 1) {
        switch(scancode[0]) {
        case W_MAKE_CODE        : key_presses.w_pressed     = 1;        break;
        case W_BREAK_CODE       : key_presses.w_pressed     = 0;        break;
        case A_MAKE_CODE        : key_presses.a_pressed     = 1;        break;
        case A_BREAK_CODE       : key_presses.a_pressed     = 0;        break;
        case S_MAKE_CODE        : key_presses.s_pressed     = 1;        break;
        case S_BREAK_CODE       : key_presses.s_pressed     = 0;        break;
        case D_MAKE_CODE        : key_presses.d_pressed     = 1;        break;
        case D_BREAK_CODE       : key_presses.d_pressed     = 0;        break;
        case CTRL_MAKE_CODE     : key_presses.ctrl_pressed  = 1;        break;
        case CTRL_BREAK_CODE    : key_presses.ctrl_pressed  = 0;        break;
        case PLUS_MAKE_CODE     : key_presses.plus_pressed  = 1;        break;
        case PLUS_BREAK_CODE    : key_presses.plus_pressed  = 0;        break;
        case MINUS_MAKE_CODE    : key_presses.minus_pressed = 1;        break;
        case MINUS_BREAK_CODE   : key_presses.minus_pressed = 0;        break;
        }
    }
    ver_mov = key_presses.s_pressed - key_presses.w_pressed;
    hor_mov = key_presses.d_pressed - key_presses.a_pressed;
}

void update_movement(gunner_t *p) {
    static const int speed = 5;
    gunner_set_pos(p, gunner_get_x(p) + speed * hor_mov, gunner_get_y(p) + speed * ver_mov);
}

void update_scale(void) {
    static uint8_t last_plus = 0, last_minus = 0;
    if (key_presses.ctrl_pressed) {
        if (key_presses.plus_pressed && !last_plus) {
            double scale = ent_get_scale();
            scale *= 1.1;
            if (scale <= MAX_SCALE) ent_set_scale(scale);
        }
        else if (key_presses.minus_pressed && !last_minus) {
            double scale = ent_get_scale();
            scale /= 1.1;
            if (scale >= MIN_SCALE) ent_set_scale(scale);
        }

        printf("SCALE: %d\n", (int)(ent_get_scale()*1000));
    }

    last_plus = key_presses.plus_pressed;
    last_minus = key_presses.minus_pressed;
}

static int32_t mouse_x = 0, mouse_y = 0;

void update_mouse_position(struct packet *p) {
    mouse_x = max(0, mouse_x + p->delta_x);
    mouse_x = min(mouse_x, graph_get_XRes() - 1);

    mouse_y = max(0, mouse_y - p->delta_y);
    mouse_y = min(mouse_y, graph_get_YRes() - 1);
}

int32_t get_mouse_X(void) { return mouse_x; }

int32_t get_mouse_Y(void) { return mouse_y; }

double get_mouse_angle(gunner_t *p) {
    return atan2(gunner_get_y_screen(p) - mouse_y, mouse_x - gunner_get_x_screen(p));
}

int get_hor_movement(void) { return hor_mov; }

int get_ver_movement(void) { return ver_mov; }
