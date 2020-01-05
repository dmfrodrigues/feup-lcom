#ifndef PROJ_FUNC_H_INCLUDED
#define PROJ_FUNC_H_INCLUDED

#include "ent.h"
#include "text.h"
#include "proj_structures.h"

#include <stdint.h>

/**
 * @brief Cleans up all memory, unsubscribes interrupts.
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int cleanup(void);

void update_key_presses(void);

/**
 * @brief Updates movement variables.
 */
void update_movement(map_t *map, gunner_t *p, keys_t *keys, list_t *shooter_list);

void update_mouse(struct packet *p);

keys_t* (get_key_presses)(void);

void (shoot_bullet)(const gunner_t *shooter, list_t *bullet_list, const basic_sprite_t *bsp_bullet);

void (update_game_state)(const map_t *map, list_t *shooter_list, list_t *bullet_list);

void (get_random_spawn)(const map_t *map, gunner_t *p, list_t *l);

void update_scale(void);

int16_t* get_mouse_X(void);

int16_t* get_mouse_Y(void);

void build_host_structure(host_info_t *p, gunner_t *host, gunner_t *remote, list_t *bullet_list);

void build_remote_structure(remote_info_t *p, keys_t *keys, double angle);

double get_mouse_angle(gunner_t *p);

typedef struct timer {
    int time;
    text_t *text;
} text_timer_t;

text_timer_t* (timer_ctor)(const font_t *fnt);

void (timer_update)(text_timer_t *p);

void (timer_reset)(text_timer_t *p);

void (timer_dtor)(text_timer_t *p);

#endif /* end of include guard: PROJ_FUNC_H_INCLUDED */
