#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

struct rectangle;
typedef struct rectangle rectangle_t;

rectangle_t* (rectangle_ctor)(int16_t x, int16_t y, uint16_t w, uint16_t h);
void         (rectangle_dtor)(rectangle_t *p);

#endif //RECTANGLE_H_INCLUDED
