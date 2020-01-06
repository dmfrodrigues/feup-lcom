#ifndef ENT_H_INCLUDED
#define ENT_H_INCLUDED

/**
 * @defgroup ent ent
 * @ingroup proj
 * @brief Entities module
 *
 * @{
 */

#include "libs.h"

/**
 * @brief Set scale.
 * @param   n   Scale
 */
void (ent_set_scale) (double n);
/**
 * @brief Set origin of screen (upper-left corner) in game referential coordinates.
 * @param   x   X-position of the upper-left corner of the screen
 * @param   y   Y-position of the upper-left corner of the screen
 */
void (ent_set_origin)(double x, double y);
/**
 * @brief Get scale.
 * @return  Scale
 */
double (ent_get_scale)  (void);
/**
 * @brief Get X-length of the screen in game referencial coordinates.
 * @return X-length
 */
double (ent_get_XLength)(void);
/**
 * @brief Get Y-length of the screen in game referencial coordinates.
 * @return Y-length
 */
double (ent_get_YLength)(void);

/**
 * @}
 */

/**
 * @defgroup gunner_t gunner_t
 * @ingroup ent
 * @brief Gunner module.
 *
 * @{
 */

/// @brief Identifier of a melee gunner
#define GUNNER_MELEE    BIT(0)
/// @brief Identifier of a ranged gunner
#define GUNNER_RANGED   BIT(1)
/// @brief Identifier of a player-controlled gunner
#define GUNNER_PLAYER   BIT(2)
/// @brief Identifier of an automatic follower gunner
#define GUNNER_FOLLOW   BIT(3)

/**
 * @brief Gunner.
 */
typedef struct gunner gunner_t;
/**
 * @brief Construct gunner.
 * @param   dude    Basic sprite representing the gunner
 * @param   weapon  Basic sprite representing the weapon of the gunner
 * @param   type    Type of gunner. Can be one of the identifier macros and also the OR-merge of any combination of them
 * @param   team    Team of the gunner. 1 for player 1, 2 for player 2, 3 for opponents and 0 for no team (temporary gunner)
 */
gunner_t* (gunner_ctor)(basic_sprite_t *dude, basic_sprite_t *weapon, uint16_t type, int team);
/**
 * @brief Destruct gunner.
 * @param   p   Pointer to gunner to be destructed
 */
void      (gunner_dtor)(gunner_t *p);
/**
 * @brief   Set gunner position.
 * @param   p   Pointer to gunner
 * @param   x   X-position of the gunner in game coordinates
 * @param   y   Y-position of the gunner in game coordinates
 */
void (gunner_set_pos)               (gunner_t *p, double x, double y);
/**
 * @brief   Set gunner spawn position.
 * @param   p   Pointer to gunner
 * @param   x   Spawn X-position of the gunner in game coordinates
 * @param   y   Spawn Y-position of the gunner in game coordinates
 */
void (gunner_set_spawn)             (gunner_t *p, double x, double y);
/**
 * @brief   Set gunner angle.
 * @param   p       Pointer to gunner
 * @param   angle   Angle in radians
 */
void (gunner_set_angle)             (gunner_t *p, double angle);
/**
 * @brief   Set gunner health (total, max health).
 * @param   p       Pointer to gunner
 * @param   health  Health
 */
void (gunner_set_health)            (gunner_t *p, double health);
/**
 * @brief   Set gunner current health.
 * @param   p       Pointer to gunner
 * @param   health  Current health
 */
void (gunner_set_curr_health)       (gunner_t *p, double health);
/**
 * @brief   Get gunner X-position.
 * @param   p   Pointer to gunner
 * @return  X-position of the gunner
 */
double  (gunner_get_x)              (const gunner_t *p);
/**
 * @brief   Get gunner Y-position.
 * @param   p   Pointer to gunner
 * @return  Y-position of the gunner
 */
double  (gunner_get_y)              (const gunner_t *p);
/**
 * @brief   Get gunner spawn X-position.
 * @param   p   Pointer to gunner
 * @return  Spawn X-position of the gunner
 */
double  (gunner_get_spawn_x)        (const gunner_t *p);
/**
 * @brief   Get gunner spawn Y-position.
 * @param   p   Pointer to gunner
 * @return  Spawn Y-position of the gunner
 */
double  (gunner_get_spawn_y)        (const gunner_t *p);
/**
 * @brief   Get gunner angle.
 * @param   p   Pointer to gunner
 * @return  Angle of the gunner
 */
double  (gunner_get_angle)          (const gunner_t *p);
/**
 * @brief   Get gunner max health.
 * @param   p   Pointer to gunner
 * @return  Max health of the gunner
 */
double  (gunner_get_health)         (const gunner_t *p);
/**
 * @brief   Get gunner current health.
 * @param   p   Pointer to gunner
 * @return  Current health of the gunner
 */
double  (gunner_get_curr_health)    (const gunner_t *p);
/**
 * @brief   Get gunner X-position in screen coordinates.
 * @param   p   Pointer to gunner
 * @return  X-position of the gunner in screen coordinates
 */
int16_t (gunner_get_x_screen)       (const gunner_t *p);
/**
 * @brief   Get gunner Y-position in screen coordinates.
 * @param   p   Pointer to gunner
 * @return  Y-position of the gunner in screen coordinates
 */
int16_t (gunner_get_y_screen)       (const gunner_t *p);
/**
 * @brief   Get gunner type.
 * @param   p   Pointer to gunner
 * @return  Type of gunner
 */
uint16_t (gunner_get_type)          (const gunner_t *p);
/**
 * @brief   Get gunner team.
 * @param   p   Pointer to gunner
 * @return  Team of gunner
 */
int     (gunner_get_team)           (const gunner_t *p);
/**
 * @brief   Draw gunner on screen buffer.
 * @param   p   Pointer to gunner
 */
void (gunner_draw)(gunner_t *p);
/**
 * @brief   Draw gunner health bar on screen buffer.
 * @param   p   Pointer to gunner
 */
void (gunner_draw_health)(const gunner_t *p);
/**
 * @brief   Draw list of gunners on screen buffer.
 * @param   shooter_list    List of gunners
 */
void (gunner_draw_list)(list_t *shooter_list);
/**
 * @brief Get distance between two gunners.
 * @param   p1  Gunner 1
 * @param   p2  Gunner 2
 * @return  Euclidian distance between the two gunners
 */
double (gunner_distance)(const gunner_t *p1, const gunner_t *p2);
/**
 * @brief Check if two gunners are colliding.
 * @param   p1  Gunner 1
 * @param   p2  Gunner 2
 * @return  true if the two gunners are colliding, false otherwise
 */
int (gunner_collides_gunner)(const gunner_t *p1, const gunner_t *p2);

/**
 * @}
 */

/**
 * @defgroup bullet_t bullet_t
 * @ingroup ent
 * @brief Bullet module.
 *
 * @{
 */

/**
 * @brief Bullet.
 */
typedef struct bullet bullet_t;
/**
 * @brief Construct bullet.
 * @param   shooter Pointer to gunner that shot that bullet
 * @param   b       Basic sprite of the bullet
 * @param   x       X-position
 * @param   y       Y-position
 * @param   vx      X-speed
 * @param   vy      Y-speed
 * @return Pointer to constructed bullet, or NULL if failed
 */
bullet_t* (bullet_ctor)(const gunner_t *shooter, const basic_sprite_t *b, double x, double y, double vx, double vy);
/**
 * @brief Destruct bullet.
 * @param   p   Pointer to bullet to be destructed
 */
void      (bullet_dtor)(bullet_t *p);
/**
 * @brief   Get bullet X-position.
 * @param   p   Pointer to bullet
 * @return  X-position of the bullet
 */
double  (bullet_get_x)       (const bullet_t *p);
/**
 * @brief   Get bullet Y-position.
 * @param   p   Pointer to bullet
 * @return  Y-position of the bullet
 */
double  (bullet_get_y)       (const bullet_t *p);
/**
 * @brief   Get bullet X-speed.
 * @param   p   Pointer to bullet
 * @return  X-speed of the bullet
 */
double  (bullet_get_vx)      (const bullet_t *p);
/**
 * @brief   Get bullet Y-speed.
 * @param   p   Pointer to bullet
 * @return  Y-speed of the bullet
 */
double  (bullet_get_vy)      (const bullet_t *p);
/**
 * @brief   Get bullet X-position in screen coordinates.
 * @param   p   Pointer to bullet
 * @return  X-position of the bullet in screen coordinates
 */
int16_t (bullet_get_x_screen)(const bullet_t *p);
/**
 * @brief   Get bullet Y-position in screen coordinates.
 * @param   p   Pointer to bullet
 * @return  Y-position of the bullet in screen coordinates
 */
int16_t (bullet_get_y_screen)(const bullet_t *p);
/**
 * @brief   Get bullet damage.
 * @param   p   Pointer to bullet
 * @return  Damage
 */
double  (bullet_get_damage)  (const bullet_t *p);
/**
 * @brief   Set bullet damage.
 * @param   p   Pointer to bullet
 * @param   damage  Damage
 */
void    (bullet_set_damage)  (bullet_t *p, double damage);
/**
 * @brief Get gunner that shot the bullet.
 * @param   p   Pointer to bullet
 * @return  Pointer to gunner that shot the bullet
 */
const gunner_t* (bullet_get_shooter)(const bullet_t *p);
/**
 * @brief Update bullet position (i.e., advance it according to its speed).
 * @param   p   Pointer to bullet
 */
void (bullet_update_movement)(bullet_t *p);
/**
 * @brief Update movement of bullets in a list.
 * @param   bullet_list     Pointer to bullet list
 */
void (bullet_update_movement_list)(list_t *bullet_list);
/**
 * @brief Draw bullet on screen buffer.
 * @param   p   Pointer to bullet
 */
void (bullet_draw)(bullet_t *p);
/**
 * @brief Draw bullets in list on screen buffer.
 * @param   bullet_list     Pointer to bullet list
 */
void (bullet_draw_list)(list_t *bullet_list);

/**
 * @}
 */

/**
 * @defgroup map_t map_t
 * @ingroup ent
 * @brief Bullet module.
 *
 * @{
 */

/**
 * @brief Map.
 */
typedef struct map map_t;
/**
 * @brief Construct map.
 * @param   background  XPM describing map as it should be rendered
 * @param   collide     XPM with transparency describing what parts of the map will cause collision
 * @return  Pointer to constructed map, or NULL if failed
 */
map_t* (map_ctor)(const char *const *background, const char *const *collide);
/**
 * @brief Destruct map.
 * @param   p   Pointer to map to be destructed
 */
void   (map_dtor)(map_t *p);
/**
 * @brief Get map width.
 * @param   p   Pointer to map
 * @return      Width of map
 */
uint16_t (map_get_width)   (const map_t *p);
/**
 * @brief Get map height.
 * @param   p   Pointer to map
 * @return      Height of map
 */
uint16_t (map_get_height)  (const map_t *p);
/**
 * @brief Use Dijkstra's path-finding algorithm.
 *
 * Allows a follower to know where to move so it gets closer to a given target.
 *
 * This function is implemented in a clever way that reduces the time of execution.
 * On the other hand, it might not always give the shortest path, but it is
 * guaranteeed to provide a decent, valid path to the target.
 *
 * @param   p   Pointer to map
 * @param   x_  X-position of the target
 * @param   y_  Y-position of the target
 * @return  SUCCESS if operation is successful, false otherwise
 */
int (map_make_dijkstra)(map_t *p, double x_, double y_);
/**
 * @brief Get the direction a follower should move to get closer to the target.
 * @param   p       Pointer to map
 * @param   x       X-position of the follower
 * @param   y       Y-position of the follower
 * @param   theta   Pointer to angle that will be updated
 * @return SUCCESS if operation was successful, other value otherwise
 */
int (map_where_to_follow)(const map_t *p, double x, double y, double *theta);
/**
 * @brief Draw map on screen buffer.
 * @param   p   Pointer to map
 */
void   (map_draw)(map_t *p);
/**
 * @brief Evaluate if point collides with map.
 * @param   p   Pointer to map
 * @param   x   X-position of the point
 * @param   y   Y-position of the point
 * @return true if the point collides with the map, false otherwise
 */
int    (map_collides_point)(const map_t *p, double x, double y);

/**
 * @}
 */

/**
 * @brief Evaluate if gunner is colliding with map.
 * @param   p       Pointer to map
 * @param   gunner  Pointer to gunner
 * @return true if the gunner collides with the map, false otherwise
 */
int    (map_collides_gunner)(const map_t *p, const gunner_t *gunner);
/**
 * @brief Evaluate if bullet is colliding with map.
 * @param   p       Pointer to map
 * @param   bullet  Pointer to bullet
 * @return true if the bullet collides with the map, false otherwise
 */
int    (map_collides_bullet)(const map_t *p, const bullet_t *bullet);
/**
 * @brief Evaluate if bullet is colliding with gunner.
 * @param   shooter     Pointer to gunner
 * @param   bull        Pointer to bullet
 * @return true if the bullet collides with the gunner, false otherwise
 */
int (gunner_collides_bullet)(const gunner_t *shooter, const bullet_t *bull);

#endif //ENT_H_INCLUDED
