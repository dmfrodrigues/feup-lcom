#ifndef PROJ_MACROS_H_INCLUDED
#define PROJ_MACROS_H_INCLUDED

/**
 * @defgroup proj_macros proj_macros
 * @ingroup proj
 * @brief Project macros.
 *
 * @{
 */

// Movement Directions
/** @brief Moving to the top side of screen */
#define UP      -1
/** @brief Moving to the bottom side of screen */
#define DOWN    1
/** @brief Moving to the left side of screen */
#define LEFT    -1
/** @brief Moving to the right side of screen */
#define RIGHT   1
/** @brief Not moving */
#define REST    0

// Movement Constants
/** @brief Shooter speed */
#define SHOOTER_SPEED   5
/** @brief Bullet speet */
#define BULLET_SPEED    8

// Refresh Rate
/** @brief Screen refresh rate */
#define REFRESH_RATE    60

//Graphics mode
/** @brief Graphic mode used */
#define GRAPH_MODE      DIRECT_1024_768_888
/** @brief Minimum zoom */
#define MIN_SCALE       0.2
/** @brief Default zoom */
#define DEFAULT_SCALE   0.5
/** @brief Maximum zoom */
#define MAX_SCALE       10.0

// Colors
/** @brief Text color */
#define TEXT_COLOR              0xFFFFFF
/** @brief Highlight color */
#define HIGHLIGHT_COLOR         0x404040
/** @brief Delimiter color */
#define DELIMITER_COLOR         0x404040

/** @brief Melee attacks range */
#define MELEE_RANGE    80
/** @brief Melee attack damage (for zombies). */
#define MELEE_DAMAGE   1
/** @brief Zombie speed */
#define ZOMBIE_SPEED    1.0

/**
 * @}
 */

#endif /* end of include guard: PROJ_MACROS_H_INCLUDED */
