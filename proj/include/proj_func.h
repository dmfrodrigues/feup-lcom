#ifndef PROJ_FUNC_H_INCLUDED
#define PROJ_FUNC_H_INCLUDED

#include "sprite.h"

/**
 * @brief Cleans up all memory, unsubscribes interrupts.
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int cleanup(void);

/**
 * @brief Updates movement variables.
 */
void update_movement(void);

void update_mouse_position(struct packet *p);

int32_t get_mouse_X(void);

int32_t get_mouse_Y(void);

/**
 * @brief
 * @param
 * @param
 * @param
 * @return Angle
 */
double get_mouse_angle(sprite_t *p);

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
