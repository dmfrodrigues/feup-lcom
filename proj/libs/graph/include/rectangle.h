#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

/**
 * @defgroup    rectangle_t rectangle_t
 * @ingroup graph
 * @brief   Rectangle module.
 *
 * @{
 */

/**
 * @brief Rectangle.
 */
typedef struct rectangle rectangle_t;
/**
 * @brief Construct rectangle.
 * @param   x   X-position of upper-left corner
 * @param   y   Y-position of upper-left corner
 * @param   w   Width
 * @param   h   Height
 * @return      Pointer to constructed rectangle, or NULL if failed
 */
rectangle_t* (rectangle_ctor)(int16_t x, int16_t y, uint16_t w, uint16_t h);
/**
 * @brief Destruct rectangle.
 * @param   p   Pointer to rectangle to be destructed
 */
void         (rectangle_dtor)(rectangle_t *p);
/**
 * @brief Set rectangle position.
 * @param   p   Pointer to rectangle
 * @param   x   X-position of the upper-left corner
 * @param   y   Y-position of the upper-left corner
 */
void (rectangle_set_pos)          (rectangle_t *p,  int16_t x,  int16_t y);
/**
 * @brief Set rectangle size.
 * @param   p   Pointer to rectangle
 * @param   w   Width
 * @param   h   Height
 */
void (rectangle_set_size)         (rectangle_t *p, uint16_t w, uint16_t h);
/**
 * @brief Set rectangle fill color.
 * @param   p       Pointer to rectangle
 * @param   color   Color to fill rectangle
 */
void (rectangle_set_fill_color)   (rectangle_t *p, uint32_t color);
/**
 * @brief Set rectangle fill transparency.
 * @param   p       Pointer to rectangle
 * @param   alpha   Value of the alpha-channel
 */
void (rectangle_set_fill_trans)   (rectangle_t *p, uint8_t  alpha);
/**
 * @brief Set rectangle outline color.
 * @param   p       Pointer to rectangle
 * @param   color   Color to outline rectangle
 */
void (rectangle_set_outline_color)(rectangle_t *p, uint32_t color);
/**
 * @brief Set rectangle outline width.
 * @param   p       Pointer to rectangle
 * @param   width   Width of the rectangle border/outline
 */
void (rectangle_set_outline_width)(rectangle_t *p,  int16_t width);
/**
 * @brief Get rectangle x-position.
 * @param   p   Pointer to rectangle
 * @return      Rectangle x-position
 */
int16_t  (rectangle_get_x)(const rectangle_t *p);
/**
 * @brief Get rectangle y-position.
 * @param   p   Pointer to rectangle
 * @return      Rectangle y-position
 */
int16_t  (rectangle_get_y)(const rectangle_t *p);
/**
 * @brief Get rectangle width.
 * @param   p   Pointer to rectangle
 * @return      Rectangle width
 */
uint16_t (rectangle_get_w)(const rectangle_t *p);
/**
 * @brief Get rectangle height.
 * @param   p   Pointer to rectangle
 * @return      Rectangle height
 */
uint16_t (rectangle_get_h)(const rectangle_t *p);
/**
 * @brief Know if point in screen coordinates lies inside rectangle.
 * @param   p   Pointer to rectangle
 * @param   x   X-position of the point
 * @param   y   Y-position of the point
 * @return      True if the point lies inside the rectangle, false otherwise.
 */
int      (rectangle_collide_point)(const rectangle_t *p, int x, int y);
/**
 * Draw rectangle in screen buffer.
 * @param   p   Pointer to rectangle to be drawn
 */
void (rectangle_draw)(const rectangle_t *p);

/**
 * @}
 */

#endif //RECTANGLE_H_INCLUDED
