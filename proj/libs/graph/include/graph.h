#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

/**
 * @defgroup    graph   graph
 * @brief Graph module.
 *
 * Provides functions, classes and methods for graphic drawing.
 *
 * @{
 */

// Graphics modes
#define INDEXED_1024_768        0x105
#define DIRECT_640_480_888      0x110
#define DIRECT_800_600_888      0x115
#define DIRECT_1024_768_888     0x118
#define DIRECT_1280_1024_565    0x11A
#define DIRECT_1280_1024_888    0x11B

// Colors in RBG (8 bit)
#define GRAPH_BLACK             0x000000
#define GRAPH_WHITE             0xFFFFFF
#define GRAPH_TRANSPARENT       0x00
#define GRAPH_RED               0xFF0000

// Alpha
#define ALPHA_THRESHOLD     0x7F

/**
 * @brief Initialize graphics.
 * @param   mode    Graphics mode to use
 * @return  SUCCESS if operation was successful, other value otherwise
 */
int (graph_init)(uint16_t mode);

/**
 * @brief Cleanup graphics.
 * @return  SUCCESS if operation was successful, other value otherwise
 */
int (graph_cleanup)(void);

/**
 * @brief Get screen X-resolution.
 * @return Screen X-resolution
 */
uint16_t   (graph_get_XRes)         (void);
/**
 * @brief Get screen Y-resolution.
 * @return Screen Y-resolution
 */
uint16_t   (graph_get_YRes)         (void);
/**
 * @brief Get number of bytes per pixel.
 *
 * That is, the number of bytes needed to encode the color of a pixel.
 * @return Number of bytes per pixel
 */
uint16_t   (graph_get_bytes_pixel)  (void);

/**
 * @brief Draw pixel with color.
 * @param   x       X-position of pixel to draw
 * @param   y       Y-position of pixel to draw
 * @param   color   Color that will be used to draw the pixel
 * @return  SUCCESS if operation was successful, other value otherwise
 */
void (graph_set_pixel)             (uint16_t x, uint16_t y, uint32_t color);
/**
 * @brief Draw pixel with color using position.
 *
 * Position is calculated from y*WIDTH+x.
 * @param   pos     Position
 * @param   color   Color that will be used to draw the pixel
 */
void (graph_set_pixel_pos)         (unsigned pos          , uint32_t color);

/**
 * @brief Clear screen, painting it black.
 * @return  SUCCESS if operation was successful, other value otherwise
 */
int (graph_clear_screen)(void);

/**
 * @brief Draw everything on the buffer to the screen.
 * @return  SUCCESS if operation was successful, other value otherwise
 */
int (graph_draw)(void);

/**
 * @}
 */

#endif /* end of include guard: GRAPH_H_INCLUDED */
