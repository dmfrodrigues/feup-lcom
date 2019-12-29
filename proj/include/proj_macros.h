#ifndef PROJ_MACROS_H_INCLUDED
#define PROJ_MACROS_H_INCLUDED

// WASD Movement Keys
#define W_MAKE_CODE         0x11    /** @brief W Make Code */
#define W_BREAK_CODE        0x91    /** @brief W Break Code */
#define A_MAKE_CODE         0x1E    /** @brief A Make Code */
#define A_BREAK_CODE        0x9E    /** @brief A Break Code */
#define S_MAKE_CODE         0x1F    /** @brief S Make Code */
#define S_BREAK_CODE        0x9F    /** @brief S Break Code */
#define D_MAKE_CODE         0x20    /** @brief D Make Code */
#define D_BREAK_CODE        0xA0    /** @brief D Break Code */
#define CTRL_MAKE_CODE      0x1D    /** @brief CTRL Make Code */
#define CTRL_BREAK_CODE     0x9D    /** @brief CTRL Break Code */
#define PLUS_MAKE_CODE      0x1A    /** @brief Plus (+) Make Code */
#define PLUS_BREAK_CODE     0x9A    /** @brief Plus (+) Break Code */
#define MINUS_MAKE_CODE     0x35    /** @brief Minus (-) Make Code */
#define MINUS_BREAK_CODE    0xB5    /** @brief Minus (-) Break Code */
#define ENTER_MAKE_CODE     0x1C
#define ENTER_BREAK_CODE    0x9C

// Movement Directions
#define UP      -1  /** @brief Moving to the top side of screen */
#define DOWN    1   /** @brief Moving to the bottom side of screen */
#define LEFT    -1  /** @brief Moving to the left side of screen */
#define RIGHT   1   /** @brief Moving to the right side of screen */
#define REST    0   /** @brief Not moving */

// Movement Constants
#define SHOOTER_SPEED   5   /** @brief Shooter speed */
#define BULLET_SPEED    8   /** @brief Bullet speet */

// Game States
#define MENU            0
#define GAME            1
#define TEST            2
#define EXIT            3

// Extra Keys
#define ESC_MAKE_CODE   0x01    /** @brief ESC Make Code */
#define ESC_BREAK_CODE  0x81    /** @brief ESC Break Code */

// Refresh Rate
#define REFRESH_RATE    60      /** @brief Screen refresh rate */

//Graphics mode
#define GRAPH_MODE      DIRECT_1024_768_888 /** @brief Graphic mode used */
#define MIN_SCALE       0.2                  /** @brief Minimum zoom */
#define DEFAULT_SCALE   0.5                  /** @brief Default zoom */
#define MAX_SCALE       10.0                 /** @brief Maximum zoom */

// Colors
#define TEXT_COLOR              0x888888
#define HIGHLIGHT_COLOR         0xA0A0A0
#define DELIMITER_COLOR         0x404040

#endif /* end of include guard: PROJ_MACROS_H_INCLUDED */
