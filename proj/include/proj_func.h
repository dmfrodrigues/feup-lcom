#ifndef PROJ_FUNC_H_INCLUDED
#define PROJ_FUNC_H_INCLUDED

#include "ent.h"

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
void update_movement(const map_t *map, gunner_t *p);

typedef struct keys {
    uint8_t w_pressed       : 1;
    uint8_t a_pressed       : 1;
    uint8_t s_pressed       : 1;
    uint8_t d_pressed       : 1;
    uint8_t ctrl_pressed    : 1;
    uint8_t plus_pressed    : 1;
    uint8_t minus_pressed   : 1;
    uint8_t lb_pressed      : 1;
} keys_t;

void update_mouse(struct packet *p);

keys_t* (get_key_presses)(void);

void (shoot_bullet)(const gunner_t *shooter, list_t *bullet_list);

void (update_game_state)(const map_t *map, gunner_t *shooter, list_t *bullet_list);

void update_scale(void);

int32_t get_mouse_X(void);

int32_t get_mouse_Y(void);

/**
 * @brief
 * @param
 * @param
 * @param
 * @return Angle
 */
double get_mouse_angle(gunner_t *p);

/**
 * @brief Get horizontal movement direction.
 * @return Horizontal movement direction (-1 -> heading LEFT; 0 -> not moving horizontally; 1 -> heading RIGHT)
 */
int get_hor_movement(void);

/**
 * @brief Get vertical movement direction.
 * @return Vertical movement direction (-1 -> heading UP; 0 -> not moving vertically; 1 -> heading DOWN)
 */
int get_ver_movement(void);

#endif /* end of include guard: PROJ_FUNC_H_INCLUDED */
