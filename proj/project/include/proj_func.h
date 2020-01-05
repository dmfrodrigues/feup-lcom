#ifndef PROJ_FUNC_H_INCLUDED
#define PROJ_FUNC_H_INCLUDED

/**
 * @defgroup proj_func proj_func
 * @ingroup proj
 * @brief Project helper functions.
 *
 * @{
 */

#include "libs.h"
#include "proj_structures.h"

//#include <stdint.h>

/**
 * @brief Cleans up all memory, unsubscribes interrupts.
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int cleanup(void);

/**
 * @brief Update key presses.
 */
void update_key_presses(void);
/**
 * @brief Get key presses.
 * @return  keys_t containing key presses
 */
keys_t* (get_key_presses)(void);

/**
 * @brief Update mouse.
 */
void update_mouse(struct packet *p);
/**
 * @brief Get pointer to mouse X-position.
 * @return  Pointer to mouse X-position.
 */
int16_t* get_mouse_X(void);
/**
 * @brief Get pointer to mouse Y-position.
 * @return  Pointer to mouse Y-position.
 */
int16_t* get_mouse_Y(void);
/**
 * @brief Get cursor angle relative to a gunner in the screen.
 * @param   p   Pointer to gunner
 * @return      Angle of the mouse relative to the gunner
 */
double get_mouse_angle(gunner_t *p);

/**
 * @brief Updates movement variables.
 */
void update_movement(map_t *map, gunner_t *p, keys_t *keys, list_t *shooter_list);

/**
 * @brief Get random spawn (actually, position) for new gunner.
 *
 * The new position cannot collide with the map, nor with any of the gunners
 * already in the list.
 * @param   map     Pointer to map
 * @param   p       Pointer to new gunner
 * @param   l       List of gunners that already exist
 */
void (get_random_spawn)(const map_t *map, gunner_t *p, list_t *l);

/**
 * @brief Update scale if the keys were pressed to change scale.
 */
void update_scale(void);

/**
 * @brief Shoot a bullet.
 * @param   shooter     Pointer to gunner that shot the bullet
 * @param   bullet_list List of the bullets in the map
 * @param   bsp_bullet  Basic sprite that will be used to render the bullet
 */
void (shoot_bullet)(const gunner_t *shooter, list_t *bullet_list, const basic_sprite_t *bsp_bullet);
/**
 * @brief Update game state.
 *
 * This includes moving bullets accordingly, processing damages, collisions of
 * bullets with walls, melee damage for the gunners that have autonomous melee.
 * @param   map             Pointer to map
 * @param   shooter_list    List of shooters
 * @param   bullet_list     List of bullets in the map
 */
void (update_game_state)(const map_t *map, list_t *shooter_list, list_t *bullet_list);

/**
 * @brief Wrapper to easen filling of the host_info_t.
 * @param   p       Host info to fill
 * @param   host    Host gunner
 * @param   remote  Remote gunner
 */
void build_host_structure(host_info_t *p, gunner_t *host, gunner_t *remote);
/**
 * @brief Wrapper to easen filling of the remote_info_t.
 * @param   p       Remote info to fill
 * @param   keys    Key presses of remote
 * @param   angle   Mouse angle of remote
 */
void build_remote_structure(remote_info_t *p, keys_t *keys, double angle);



/**
 * @}
 */

/**
 * @defgroup text_timer_t text_timer_t
 * @ingroup proj_func
 * @brief Text timer.
 *
 * @{
 */

/**
 * @brief Text timer.
 */
typedef struct {
    /// @brief Time since construction.
    int time;
    /// @brief Text.
    text_t *text;
} text_timer_t;
/**
 * @brief Construct timer
 * @param   fnt Font used to render timer text
 * @return      Pointer to constructed timer, or NULL if failed
 */
text_timer_t* (text_timer_ctor)(const font_t *fnt);
/**
 * @brief Destruct timer.
 * @param   p   Pointer to timer to be destructed
 */
void (text_timer_dtor)(text_timer_t *p);
/**
 * @brief   Update timer, by incrementing the number of seconds.
 * @param   p   Pointer to timer
 */
void (text_timer_update)(text_timer_t *p);
/**
 * @brief   Reset timer to 0.
 * @param   p   Pointer to timer
 */
void (text_timer_reset)(text_timer_t *p);

/**
 * @}
 */

#endif /* end of include guard: PROJ_FUNC_H_INCLUDED */
