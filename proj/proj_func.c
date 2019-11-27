#include <lcom/lcf.h>

#include "proj_func.h"

#include "interrupts_func.h"
#include "graph.h"
#include "keyboard.h"
#include "errors.h"
#include "proj_macros.h"
#include "utils.h"

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

void update_movement(void) {
    static int w_pressed = 0, a_pressed = 0, s_pressed = 0, d_pressed = 0;
    if (sz == 1) {
        kbd_print_scancode(!(scancode[0] & BREAK_CODE_BIT), 1, scancode);
        switch(scancode[0]) {
        case W_MAKE_CODE  : w_pressed = 1;      break;
        case W_BREAK_CODE : w_pressed = 0;      break;
        case A_MAKE_CODE  : a_pressed = 1;      break;
        case A_BREAK_CODE : a_pressed = 0;      break;
        case S_MAKE_CODE  : s_pressed = 1;      break;
        case S_BREAK_CODE : s_pressed = 0;      break;
        case D_MAKE_CODE  : d_pressed = 1;      break;
        case D_BREAK_CODE : d_pressed = 0;      break;
        }
    }
    ver_mov = s_pressed - w_pressed;
    hor_mov = d_pressed - a_pressed;
}

double get_mouse_angle(sprite_t *p, int32_t mouse_x, int32_t mouse_y) {
    return atan2(sprite_get_y(p) - mouse_y, mouse_x - sprite_get_x(p));
}

int get_hor_movement(void) { return hor_mov; }

int get_ver_movement(void) { return ver_mov; }
