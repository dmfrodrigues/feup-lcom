#include <lcom/lcf.h>

#include "proj_func.h"

#include "interrupts_func.h"
#include "libs.h"
#include "proj_macros.h"
#include "ent.h"

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
    if (keyboard_get_size() == 1) {
        switch(keyboard_get_scancode()[0]) {
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
        case PLUS_MAKE_CODE     : key_presses.plus_pressed  = 1;    update_scale();        break;
        case PLUS_BREAK_CODE    : key_presses.plus_pressed  = 0;    update_scale();        break;
        case MINUS_MAKE_CODE    : key_presses.minus_pressed = 1;    update_scale();        break;
        case MINUS_BREAK_CODE   : key_presses.minus_pressed = 0;    update_scale();        break;
        }
    }
}
keys_t* (get_key_presses)(void) {
    return &key_presses;
}

static int16_t mouse_x = 0, mouse_y = 0;
void (update_mouse)(struct packet *p) {
    mouse_x = max_16(0, mouse_x + p->delta_x);
    mouse_x = min_16(mouse_x, (int16_t)graph_get_XRes() - 1);

    mouse_y = max_16(0, mouse_y - p->delta_y);
    mouse_y = min_16(mouse_y, (int16_t)graph_get_YRes() - 1);

    key_presses.lb_pressed = p->lb;
}
int16_t* get_mouse_X(void) { return &mouse_x; }
int16_t* get_mouse_Y(void) { return &mouse_y; }
double get_mouse_angle(gunner_t *p) {
    return atan2(gunner_get_y_screen(p) - mouse_y, mouse_x - gunner_get_x_screen(p));
}

void update_movement(map_t *map, gunner_t *p, keys_t *keys, list_t *shooter_list) {
    /** Player movement */{
        int ver_mov = keys->s_pressed - keys->w_pressed;
        int hor_mov = keys->d_pressed - keys->a_pressed;
        double x = gunner_get_x(p);
        double y = gunner_get_y(p);

        gunner_set_pos(p, x + SHOOTER_SPEED * hor_mov, y);
        if (map_collides_gunner(map, p)) gunner_set_pos(p, x, y);
        else {
            list_node_t *it = list_begin(shooter_list);
            while (it != list_end(shooter_list)) {
                gunner_t *p2 = *(gunner_t**)list_node_val(it);
                if (p != p2 && gunner_collides_gunner(p, p2)) {
                    gunner_set_pos(p, x, y);
                    break;
                }
                it = list_node_next(it);
            }
        }
        x = gunner_get_x(p);
        gunner_set_pos(p, x, y + SHOOTER_SPEED * ver_mov);
        if (map_collides_gunner(map, p)) gunner_set_pos(p, x, y);
        else {
            list_node_t *it = list_begin(shooter_list);
            while (it != list_end(shooter_list)) {
                gunner_t *p2 = *(gunner_t**)list_node_val(it);
                if (p != p2 && gunner_collides_gunner(p, p2)) {
                    gunner_set_pos(p, x, y);
                    break;
                }
                it = list_node_next(it);
            }
        }
    }
    /** Followers movement */ {
        list_node_t *it = list_begin(shooter_list);
        while(it != list_end(shooter_list)){
            gunner_t *g = *(gunner_t**)list_node_val(it);
            if(gunner_get_type(g) & GUNNER_FOLLOW){
                double theta = 0.0;
                map_where_to_follow(map, gunner_get_x(g), gunner_get_y(g), &theta);
                double c = fm_cos(theta), s = fm_sin(theta);
                double dx = ZOMBIE_SPEED*c, dy = -ZOMBIE_SPEED*s;
                double x = gunner_get_x(g);
                double y = gunner_get_y(g);
                gunner_set_pos(g, x+dx, y+dy);
                if (map_collides_gunner(map, g)){
                    gunner_set_pos(g, x, y);
                } else {
                    list_node_t *it2 = list_begin(shooter_list);
                    while (it2 != list_end(shooter_list)) {
                        gunner_t *p2 = *(gunner_t**)list_node_val(it2);
                        if (g != p2 && gunner_collides_gunner(g, p2)) {
                            gunner_set_pos(g, x, y);
                            break;
                        }
                        it2 = list_node_next(it2);
                    }
                }
                gunner_set_angle(g, theta-M_PI_2);
            }
            it = list_node_next(it);
        }
    }
}

void (get_random_spawn)(const map_t *map, gunner_t *p, list_t *l) {
    uint16_t w = map_get_width(map), h = map_get_height(map);
    double x, y;

    while(true){
        x = rand() % w;
        y = rand() % h;
        gunner_set_pos(p, x, y);
        if(map_collides_gunner(map, p)) continue;
        int collides = false;
        list_node_t *it = list_begin(l);
        while(it != list_end(l)){
            if(gunner_collides_gunner(p, *list_node_val(it))){
                collides = true;
                break;
            }
            it = list_node_next(it);
        }
        if(!collides) return;
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

void (shoot_bullet)(const gunner_t *shooter, list_t *bullet_list, const basic_sprite_t *bsp_bullet) {
    double angle = gunner_get_angle(shooter);
    double vx = -BULLET_SPEED * fm_sin(angle);
    double vy = -BULLET_SPEED * fm_cos(angle);
    bullet_t *bullet = bullet_ctor(shooter, bsp_bullet, gunner_get_x(shooter), gunner_get_y(shooter), vx, vy);
    list_insert(bullet_list, list_end(bullet_list), bullet);
}

void (update_game_state)(const map_t *map, list_t *shooter_list, list_t *bullet_list) {
    /// BULLETS
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
            if(gunner_collides_bullet(shooter, bullet) &&
               gunner_get_team(shooter) != gunner_get_team(bullet_get_shooter(bullet))) {
                list_node_t *aux = list_node_next(bullet_it);
                /// Change health
                gunner_set_curr_health(shooter, gunner_get_curr_health(shooter) - bullet_get_damage(bullet));
                if (gunner_get_curr_health(shooter) <= 0) {
                    gunner_dtor(list_erase(shooter_list, shooter_it));
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
    /// MELEE
    list_node_t *it1 = list_begin(shooter_list);
    while(it1 != list_end(shooter_list)){
        gunner_t *s1 = *list_node_val(it1);
        if(!(gunner_get_type(s1) & GUNNER_MELEE)){ it1 = list_node_next(it1); continue; }
        list_node_t *it2 = list_begin(shooter_list);
        while(it2 != list_end(shooter_list)){
            gunner_t *s2 = *list_node_val(it2);
            if(gunner_get_team(s1) != gunner_get_team(s2) &&
               gunner_distance(s1, s2) < MELEE_RANGE)
                gunner_set_curr_health(s2, gunner_get_curr_health(s2) - MELEE_DAMAGE);
            if(gunner_get_curr_health(s2) <= 0){
                list_node_t *aux = list_node_next(it2);
                gunner_dtor(list_erase(shooter_list, it2));
                it2 = aux;
            }else it2 = list_node_next(it2);
        }
        it1 = list_node_next(it1);
    }
}

text_timer_t* (text_timer_ctor)(const font_t *fnt){
    if(fnt == NULL) return NULL;
    text_timer_t *ret = malloc(sizeof(timer_t));
    if (ret == NULL) return NULL;
    ret->time = 0;
    ret->text = text_ctor(fnt, "000s");
    text_set_color(ret->text, TEXT_COLOR);
    return ret;
}
void (text_timer_update)(text_timer_t *p){
    if (p->time >= 999) return;
    p->time++;
    char buffer[100];
    sprintf(buffer, "%03ds", p->time);
    text_set_string(p->text, buffer);
}
void (text_timer_reset)(text_timer_t *p){
    text_set_string(p->text, "000s");
}
void (text_timer_dtor)(text_timer_t *p){
    if (p == NULL) return;
    text_dtor(p->text);
    free(p);
}

host_info_t* host_info_ctor(gunner_t *host, gunner_t *remote) {
    host_info_t *ret = (host_info_t*)malloc(sizeof(host_info_t));
    if (ret == NULL) return ret;

    ret->host_x               = (int16_t)gunner_get_x          (host);
    ret->host_y               = (int16_t)gunner_get_y          (host);
    ret->host_angle           = (float)  gunner_get_angle      (host);
    ret->host_health          = (int16_t)gunner_get_health     (host);
    ret->host_current_health  = (int16_t)gunner_get_curr_health(host);

    // remote
    ret->remote_x               = (int16_t)gunner_get_x          (remote);
    ret->remote_y               = (int16_t)gunner_get_y          (remote);
    ret->remote_angle           = (float)  gunner_get_angle      (remote);
    ret->remote_health          = (int16_t)gunner_get_health     (remote);
    ret->remote_current_health  = (int16_t)gunner_get_curr_health(remote);

    //ret->no_bullets = 0;

    return ret;
}
void host_info_dtor(host_info_t *p) {
    if (p==NULL) return;
    /*
    if ((p->bullets_x) != NULL){ free(p->bullets_x); p->bullets_x = NULL; }

    if ((p->bullets_y) != NULL){ free(p->bullets_y); p->bullets_y = NULL; }

    if ((p->bullets_vx) != NULL){ free(p->bullets_vx); p->bullets_vx = NULL; }

    if ((p->bullets_vy) != NULL){ free(p->bullets_vy); p->bullets_vy = NULL; }

    if ((p->bullets_shooter) != NULL){ free(p->bullets_shooter); p->bullets_shooter = NULL; }
    */
    free(p);
}

remote_info_t* remote_info_ctor(void) {
    remote_info_t *ret = (remote_info_t*)malloc(sizeof(remote_info_t));
    if (ret == NULL) return ret;

    memset(&(ret->remote_keys_pressed), 0, sizeof(keys_t));

    ret->remote_angle = 0;

    return ret;
}
void remote_info_dtor(remote_info_t *p) {
    if (p==NULL) return;
    free(p);
}

bullet_info_t* bullet_info_ctor(void) {
    bullet_info_t *ret = (bullet_info_t*)malloc(sizeof(bullet_info_t));
    if (ret == NULL) return ret;

    ret->new_bullet = false;

    return ret;
}
void bullet_info_dtor(bullet_info_t *p) {
    if (p == NULL) return;
    free(p);
}

void build_host_structure(host_info_t *p, gunner_t *host, gunner_t *remote) {
    // host
    p->host_x               = (int16_t)gunner_get_x          (host);
    p->host_y               = (int16_t)gunner_get_y          (host);
    p->host_angle           = (float)  gunner_get_angle      (host);
    p->host_health          = (int16_t)gunner_get_health     (host);
    p->host_current_health  = (int16_t)gunner_get_curr_health(host);

    // remote
    p->remote_x               = (int16_t)gunner_get_x          (remote);
    p->remote_y               = (int16_t)gunner_get_y          (remote);
    p->remote_angle           = (float)  gunner_get_angle      (remote);
    p->remote_health          = (int16_t)gunner_get_health     (remote);
    p->remote_current_health  = (int16_t)gunner_get_curr_health(remote);
    /*
    // bullets
    size_t sz = list_size(bullet_list);
    p->no_bullets = sz;
    p->bullets_x        = (int16_t*)realloc(p->bullets_x         , sz * sizeof(int16_t));
    p->bullets_y        = (int16_t*)realloc(p->bullets_y         , sz * sizeof(int16_t));
    p->bullets_vx       = (int16_t*)realloc(p->bullets_vx        , sz * sizeof(int16_t));
    p->bullets_vy       = (int16_t*)realloc(p->bullets_vy        , sz * sizeof(int16_t));
    p->bullets_shooter  = (bool*)   realloc(p->bullets_shooter   , sz * sizeof(bool ));

    list_node_t *it = list_begin(bullet_list);
    size_t i = 0;
    while (it != list_end(bullet_list)) {
        bullet_t *bullet = *list_node_val(it);
        p->bullets_x        [i] = (int16_t)bullet_get_x                      (bullet);
        p->bullets_y        [i] = (int16_t)bullet_get_y                      (bullet);
        p->bullets_vx       [i] = (int16_t)bullet_get_vx                     (bullet);
        p->bullets_vy       [i] = (int16_t)bullet_get_vy                     (bullet);
        p->bullets_shooter  [i] = gunner_get_team(bullet_get_shooter(bullet)) != gunner_get_team(host);
        it = list_node_next(it);
        i++;
    }
    */
}
void build_remote_structure(remote_info_t *p, keys_t *keys, double angle) {
    memcpy(&(p->remote_keys_pressed), keys, sizeof(keys_t));
    p->remote_angle = angle;
}
