#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

/**
 * @defgroup    rectangle_t rectangle_t
 * @ingroup graph
 * @brief   Rectangle module.
 *
 * @{
 */

typedef struct rectangle rectangle_t;

rectangle_t* (rectangle_ctor)(int16_t x, int16_t y, uint16_t w, uint16_t h);
void         (rectangle_dtor)(rectangle_t *p);

void (rectangle_set_pos)          (rectangle_t *p,  int16_t x,  int16_t y);
void (rectangle_set_size)         (rectangle_t *p, uint16_t w, uint16_t h);
void (rectangle_set_fill_color)   (rectangle_t *p, uint32_t color);
void (rectangle_set_fill_trans)   (rectangle_t *p, uint8_t  alpha);
void (rectangle_set_outline_color)(rectangle_t *p, uint32_t color);
void (rectangle_set_outline_width)(rectangle_t *p,  int16_t width);

int16_t  (rectangle_get_x)(const rectangle_t *p);
int16_t  (rectangle_get_y)(const rectangle_t *p);
uint16_t (rectangle_get_w)(const rectangle_t *p);
uint16_t (rectangle_get_h)(const rectangle_t *p);
int      (rectangle_collide_point)(const rectangle_t *p, int x, int y);

void (rectangle_draw)(const rectangle_t *p);

/**
 * @}
 */

#endif //RECTANGLE_H_INCLUDED
