#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

/// MACROS
//#define GET_ALP(n)          (0xFF & ((n) >> 24))
#define GET_RED(n)          (0xFF & ((n) >> 16))
#define GET_GRE(n)          (0xFF & ((n) >>  8))
#define GET_BLU(n)          (0xFF & (n      ))
//#define SET_ALP(n)          (((n)&0xFF) << 24)
#define SET_RED(n)          (((n)&0xFF) << 16)
#define SET_GRE(n)          (((n)&0xFF) <<  8)
#define SET_BLU(n)          (((n)&0xFF)      )
#define SET_RGB(r,g,b)      (SET_RED(r) | SET_GRE(g) | SET_BLU(b))

/// PUBLIC GET
uint16_t   (graph_get_XRes)         (void);
uint16_t   (graph_get_YRes)         (void);

/// INIT
int (graph_init)(uint16_t mode);

/// CLEANUP
int (graph_cleanup)(void);

/// PIXEL DRAWING
int (graph_set_pixel)      (uint16_t x, uint16_t y, uint32_t color);
int (graph_set_pixel_alpha)(uint16_t x, uint16_t y, uint32_t color, uint8_t alpha);

/// SCREEN
int (graph_clear_screen)(void);

/// DRAW
int (graph_draw)(void);



/// RECTANGLE


int (graph_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (graph_draw_rectangle)(uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint32_t color);

#endif /* end of include guard: GRAPH_H_INCLUDED */
