#include <lcom/lcf.h>

#include "rectangle.h"

struct rectangle{
    int16_t x, y;
    uint16_t w, h;
    uint32_t fill_color;
    uint32_t outline_color;
    uint16_t outline_width;
};

rectangle_t* (rectangle_ctor)(int16_t x, int16_t y, uint16_t w, uint16_t h){
    rectangle_t *ret = (rectangle_t*)malloc(sizeof(rectangle_t));

    return ret;
}
void         (rectangle_dtor)(rectangle_t *p){
    if(p == NULL) return;
    free(p);
}
