#ifndef PROJ_MACROS_H_INCLUDED
#define PROJ_MACROS_H_INCLUDED

#include "graph_macros.h"

// WASD Movement Keys
#define W_MAKE_CODE     0x11    /** @brief W Make Code */
#define W_BREAK_CODE    0x91    /** @brief W Break Code */
#define A_MAKE_CODE     0x1E    /** @brief A Make Code */
#define A_BREAK_CODE    0x9E    /** @brief A Break Code */
#define S_MAKE_CODE     0x1F    /** @brief S Make Code */
#define S_BREAK_CODE    0x9F    /** @brief S Break Code */
#define D_MAKE_CODE     0x20    /** @brief D Make Code */
#define D_BREAK_CODE    0xA0    /** @brief D Break Code */

// Movement Directions
#define UP      -1  /** @brief Moving to the top side of screen */
#define DOWN    1   /** @brief Moving to the bottom side of screen */
#define LEFT    -1  /** @brief Moving to the left side of screen */
#define RIGHT   1   /** @brief Moving to the right side of screen */
#define REST    0   /** @brief Not moving */

// Extra Keys
#define ESC_MAKE_CODE   0x01    /** @brief ESC Make Code */
#define ESC_BREAK_CODE  0x81    /** @brief ESC Break Code */

// Refresh Rate
#define REFRESH_RATE    60      /** @brief Screen refresh rate */

//Graphics mode
#define GRAPH_MODE      DIRECT_1280_1024_888 /** @brief Graphic mode used */

#endif /* end of include guard: PROJ_MACROS_H_INCLUDED */
