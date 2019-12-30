#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

// Graphics modes
#define INDEXED_1024_768        0x105
#define DIRECT_640_480_888      0x110
#define DIRECT_800_600_888      0x115
#define DIRECT_1024_768_888     0x118
#define DIRECT_1280_1024_565    0x11A
#define DIRECT_1280_1024_888    0x11B
#define LINEAR_FRAME_BUFFER_MD  BIT(14)

// Colors in RBG (8 bit)
#define GRAPH_BLACK               0x000000
#define GRAPH_WHITE               0xFFFFFF

// Alpha
#define ALPHA_THRESHOLD     0x7F

/// MACROS
#define GET_ALP(n)          (0xFF & ((n) >> 24))
#define GET_RED(n)          (0xFF & ((n) >> 16))
#define GET_GRE(n)          (0xFF & ((n) >>  8))
#define GET_BLU(n)          (0xFF & (n      ))
#define GET_COLOR(n)        (0xFFFFFF & (n))
#define SET_ALP(n)          (((n)&0xFF) << 24)
#define SET_RED(n)          (((n)&0xFF) << 16)
#define SET_GRE(n)          (((n)&0xFF) <<  8)
#define SET_BLU(n)          (((n)&0xFF)      )
#define SET_RGB(r,g,b)      (SET_RED(r) | SET_GRE(g) | SET_BLU(b))

/// PUBLIC GET
uint16_t   (graph_get_XRes)         (void);
uint16_t   (graph_get_YRes)         (void);
uint16_t   (graph_get_bytes_pixel)  (void);

/// INIT
int (graph_init)(uint16_t mode);

/// CLEANUP
int (graph_cleanup)(void);

/// PIXEL DRAWING
int (graph_set_pixel)             (uint16_t x, uint16_t y, uint32_t color);
void (graph_set_pixel_pos)         (unsigned pos          , uint32_t color);

/// SCREEN
int (graph_clear_screen)(void);

/// DRAW
int (graph_draw)(void);

#endif /* end of include guard: GRAPH_H_INCLUDED */