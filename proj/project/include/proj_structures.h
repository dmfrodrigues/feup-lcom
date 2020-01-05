#ifndef PROJ_STRUCTURES_H_INCLUDED
#define PROJ_STRUCTURES_H_INCLUDED

/**
 * @defgroup proj_structures proj_structures
 * @brief Project structures.
 *
 * @{
 */

#include <stdint.h>
#include "ent.h"

/**
 * @}
 */

/**
 * @defgroup keys_t keys_t
 * @ingroup proj_structures
 * @brief Keys pressed module.
 *
 * @{
 */

/**
 * @brief Keys pressed.
 */
typedef struct {
    /// @brief W is pressed when 1
    uint8_t w_pressed       : 1;
    /// @brief A is pressed when 1
    uint8_t a_pressed       : 1;
    /// @brief S is pressed when 1
    uint8_t s_pressed       : 1;
    /// @brief D is pressed when 1
    uint8_t d_pressed       : 1;
    /// @brief Ctrl is pressed when 1
    uint8_t ctrl_pressed    : 1;
    /// @brief Plus (+) is pressed when 1
    uint8_t plus_pressed    : 1;
    /// @brief Minus (-) is pressed when 1
    uint8_t minus_pressed   : 1;
    /// @brief Mouse left button is pressed when 1
    uint8_t lb_pressed      : 1;
} keys_t;

/**
 * @}
 */

/**
* @defgroup host_info_t host_info_t
* @ingroup proj_structures
* @brief Host info module.
*
* @{
*/

/**
 * @brief Information to transmit from host to remote.
 */
typedef struct {
    // host
    /// @brief Host player X-position
    int16_t    host_x;
    /// @brief Host player Y-postition
    int16_t    host_y;
    /// @brief Host player angle
    float      host_angle;
    /// @brief Host player health
    int16_t    host_health;
    /// @brief Host player current health
    int16_t    host_current_health;

    // remote
    /// @brief Remote player X-position
    int16_t    remote_x;
    /// @brief Remote player Y-postition
    int16_t    remote_y;
    /// @brief Remote player angle
    float      remote_angle;
    /// @brief Remote player health
    int16_t    remote_health;
    /// @brief Remote player current health
    int16_t    remote_current_health;
    /*
    // bullets
    /// @brief Number of bullets
    uint8_t    no_bullets;
    /// @brief X-position of the bullets
    int16_t   *bullets_x;
    /// @brief Y-position of the bullets
    int16_t   *bullets_y;
    /// @brief X-speed of the bullets
    int16_t   *bullets_vx;
    /// @brief Y-speed of the bullets
    int16_t   *bullets_vy;
    /// @brief Who shot each bullet
    bool      *bullets_shooter; // false for host, true for remote
    */
} host_info_t;
/**
 * @brief Construct host information.
 * @param   host    Pointer to host gunner
 * @param   remote  Pointer to remote gunner
 * @return  Pointer to constructed host information, or NULL if failed
 */
host_info_t* host_info_ctor(gunner_t *host, gunner_t *remote);
/**
 * @brief Destruct host information.
 * @param   p   Pointer to host information to be destructed
 */
void host_info_dtor(host_info_t *p);

/**
 * @}
 */

/**
* @defgroup remote_info_t remote_info_t
* @ingroup proj_structures
* @brief Remote info module.
*
* @{
*/

/**
 * @brief Information to transmit from remote to host.
 */
typedef struct {
    /// @brief Remote keys that are pressed
    keys_t  remote_keys_pressed;
    /// @brief Remote player angle
    double  remote_angle;
} remote_info_t;
/**
 * @brief Construct remote information.
 * @return  Pointer to constructed remote information, or NULL if failed
 */
remote_info_t* remote_info_ctor(void);
/**
 * @brief Destruct remote information.
 * @param   p   Pointer to remote information to be destructed
 */
void remote_info_dtor(remote_info_t *p);

/**
 * @}
 */

/**
* @defgroup bullet_info_t bullet_info_t
* @ingroup proj_structures
* @brief Bullet event module.
*
* @{
*/


/**
 * @brief Bullet event to transmit from remote to host.
 */
typedef struct {
    /// @brief New bullet signal from remote
    bool   new_bullet;
} bullet_info_t;
/**
 * @brief Construct bullet event.
 * @return  Pointer to constructed bullet event, or NULL if failed
 */
bullet_info_t* bullet_info_ctor(void);
/**
 * @brief Destruct bullet event.
 * @param   p   Pointer to bullet event to be destructed
 */
void bullet_info_dtor(bullet_info_t *p);

/**
 * @}
 */

#endif /* end of include guard: PROJ_STRUCTURES_H_INCLUDED */
