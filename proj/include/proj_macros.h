#ifndef PROJ_MACROS_H_INCLUDED
#define PROJ_MACROS_H_INCLUDED

// Movement Directions
#define UP      -1  /** @brief Moving to the top side of screen */
#define DOWN    1   /** @brief Moving to the bottom side of screen */
#define LEFT    -1  /** @brief Moving to the left side of screen */
#define RIGHT   1   /** @brief Moving to the right side of screen */
#define REST    0   /** @brief Not moving */

// Movement Constants
#define SHOOTER_SPEED   5   /** @brief Shooter speed */
#define BULLET_SPEED    8   /** @brief Bullet speet */

// Refresh Rate
#define REFRESH_RATE    60      /** @brief Screen refresh rate */

//Graphics mode
#define GRAPH_MODE      DIRECT_1024_768_888 /** @brief Graphic mode used */
#define MIN_SCALE       0.2                  /** @brief Minimum zoom */
#define DEFAULT_SCALE   0.5                  /** @brief Default zoom */
#define MAX_SCALE       10.0                 /** @brief Maximum zoom */

// Colors
#define TEXT_COLOR              0xFFFFFF
#define HIGHLIGHT_COLOR         0x404040
#define DELIMITER_COLOR         0x404040

#define MELEE_RANGE    80
#define MELEE_DAMAGE   1

#endif /* end of include guard: PROJ_MACROS_H_INCLUDED */
