#include <lcom/lcf.h>

#include "proj_func.h"

#include "interrupts_func.h"
#include "graph.h"
#include "keyboard.h"
#include "errors.h"
#include "proj_macros.h"
#include "utils.h"
#include "ent.h"
#include "fast_math.h"

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
}


void update_movement(const map_t *map, gunner_t *p, gunner_t *p2) {
    int ver_mov = key_presses.s_pressed - key_presses.w_pressed;
    int hor_mov = key_presses.d_pressed - key_presses.a_pressed;
    double x = gunner_get_x(p);
    double y = gunner_get_y(p);
    gunner_set_pos(p, x + SHOOTER_SPEED * hor_mov, y);
    if (map_collides_gunner(map, p) || gunner_collides_gunner(p, p2)) {
        gunner_set_pos(p, x, y);
    }
        x = gunner_get_x(p);
    gunner_set_pos(p, x, y + SHOOTER_SPEED * ver_mov);
    if (map_collides_gunner(map, p) || gunner_collides_gunner(p, p2)) {
        gunner_set_pos(p, x, y);
    }
}

void (shoot_bullet)(const gunner_t *shooter, list_t *bullet_list, const basic_sprite_t *bsp_bullet) {
    double angle = gunner_get_angle(shooter);
    double vx = -BULLET_SPEED * fm_sin(angle);
    double vy = -BULLET_SPEED * fm_cos(angle);
    bullet_t *bullet = bullet_ctor(shooter, bsp_bullet, gunner_get_x(shooter), gunner_get_y(shooter), vx, vy);
    list_insert(bullet_list, list_end(bullet_list), bullet);
}

void (update_game_state)(const map_t *map, list_t *shooter_list, list_t *bullet_list) {

    bullet_update_movement_list(bullet_list);

    list_node_t *bullet_it = list_begin(bullet_list);
    while (bullet_it != list_end(bullet_list)) {
        /// Collision with walls
        bullet_t *bullet = *(bullet_t**)list_node_val(bullet_it);
        if (map_collides_bullet(map, bullet)) {
            list_node_t *aux = list_node_next(bullet_it);
            /// Delete bullet
            bullet_dtor(list_erase(bullet_list, bullet_it));
            /// Advance iterator
            bullet_it = aux;
            continue;
        }
        /// Collision with shooters
        list_node_t *shooter_it = list_begin(shooter_list);
        int deleted_bullet = false;
        while(shooter_it != list_end(shooter_list)){
            gunner_t *shooter = *(gunner_t**)list_node_val(shooter_it);
            if(gunner_collides_bullet(shooter, bullet)) {
                list_node_t *aux = list_node_next(bullet_it);
                /// Change health
                gunner_set_curr_health(shooter, gunner_get_curr_health(shooter) - bullet_get_damage(bullet));
                if (gunner_get_curr_health(shooter) <= 0) {
                    shooter = list_erase(shooter_list, shooter_it);
                    gunner_dtor(shooter);
                }
                /// Delete bullet
                bullet_dtor(list_erase(bullet_list, bullet_it)); deleted_bullet = true;
                /// Advance iterator
                bullet_it = aux;
                break;
            } else shooter_it = list_node_next(shooter_it);
        }
        if(deleted_bullet) continue;
        /// Advance iterator if necessary
        bullet_it = list_node_next(bullet_it);
    }
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

        //printf("SCALE: %d\n", (int)(ent_get_scale()*1000));
    }

    last_plus = key_presses.plus_pressed;
    last_minus = key_presses.minus_pressed;
}

static int32_t mouse_x = 0, mouse_y = 0;

void (update_mouse)(struct packet *p) {
    mouse_x = max(0, mouse_x + p->delta_x);
    mouse_x = min(mouse_x, graph_get_XRes() - 1);

    mouse_y = max(0, mouse_y - p->delta_y);
    mouse_y = min(mouse_y, graph_get_YRes() - 1);

    key_presses.lb_pressed = p->lb;
}

keys_t* (get_key_presses)(void) {
    return &key_presses;
}

int32_t get_mouse_X(void) { return mouse_x; }

int32_t get_mouse_Y(void) { return mouse_y; }

double get_mouse_angle(gunner_t *p) {
    return atan2(gunner_get_y_screen(p) - mouse_y, mouse_x - gunner_get_x_screen(p));
}
