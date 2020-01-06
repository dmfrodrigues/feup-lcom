#ifndef PROJ_H_INCLUDED
#define PROJ_H_INCLUDED

/**
 * @defgroup proj proj
 * @brief Project module.
 *
 * @{
 */

#include "libs.h"

#include "ent.h"

/// @brief Crosshair
basic_sprite_t       *bsp_crosshair;
/// @brief Shooter
basic_sprite_t       *bsp_shooter  ;
/// @brief Zombie
basic_sprite_t       *bsp_zombie   ;
/// @brief Pistol
basic_sprite_t       *bsp_pistol   ;
/// @brief Nothing (just a transparent basic_sprite_t)
basic_sprite_t       *bsp_nothing  ;
/// @brief Bullet
basic_sprite_t       *bsp_bullet   ;
/// @brief Map 1
map_t                *map1         ;
/// @brief Crosshair sprite
sprite_t             *sp_crosshair ;

/**
 * @}
 */

#endif //PROJ_H_INCLUDED
