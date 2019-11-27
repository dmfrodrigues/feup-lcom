#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <lcom/lcf.h>
#include <stdint.h>

#define GET_ALP(n)          (0xFF & ((n) >> 24))
#define GET_RED(n)          (0xFF & ((n) >> 16))
#define GET_GRE(n)          (0xFF & ((n) >>  8))
#define GET_BLU(n)          (0xFF & (n      ))
#define SET_ALP(n)          (((n)&0xFF) << 24)
#define SET_RED(n)          (((n)&0xFF) << 16)
#define SET_GRE(n)          (((n)&0xFF) <<  8)
#define SET_BLU(n)          (((n)&0xFF)      )
#define SET_RGB(r,g,b)      (             SET_RED(r) | SET_GRE(g) | SET_BLU(b))
#define SET_ARGB(a,r,g,b)   (SET_ALP(a) | SET_RED(r) | SET_GRE(g) | SET_BLU(b))
#define FAR2PHYS(n)         ((((n)>>12) & 0xFFFFFFF0) + ((n) & 0x0000FFFF))

typedef struct __attribute__((packed)) {

    char        VbeSignature[4]     ;
    uint16_t    VbeVersion          ;
    uint32_t    OemStringPtr        ;
    uint8_t     Capabilities[4]     ;
    uint32_t    VideoModePtr        ;
    uint16_t    TotalMemory         ;

    uint16_t    OemSoftwareRev      ;
    uint32_t    OemVendorNamePtr    ;
    uint32_t    OemProductNamePtr   ;
    uint32_t    OemProductRevPtr    ;
    char        Reserved[222]       ;

    char        OemData[256]        ;
} VbeInfoBlock;

int (get_permission)(unsigned int base_addr, unsigned int size);

//int (get_permissions_first_mbyte)(void);

int (vbe_get_mode_information)(uint16_t mode);

int (vbe_get_controller_information)(vg_vbe_contr_info_t *info_p);

phys_bytes (graph_get_phys_addr)    (void);
unsigned   (graph_get_vram_size)    (void);
uint16_t   (graph_get_XRes)         (void);
uint16_t   (graph_get_YRes)         (void);
uint16_t   (graph_get_bits_pixel)   (void);
uint16_t   (graph_get_bytes_pixel)  (void);
uint16_t   (graph_get_RedMaskSize)  (void);
uint16_t   (graph_get_GreenMaskSize)(void);
uint16_t   (graph_get_BlueMaskSize) (void);

int (graph_map_vram)(void);

int (graph_free_memory_map)(void);

int (graph_set_pixel)      (uint16_t x, uint16_t y, uint32_t color);
int (graph_set_pixel_alpha)(uint16_t x, uint16_t y, uint32_t color, uint8_t alpha);

/**
 * @brief
 * @param mode
 * @return
 */
int (graph_set_mode)(uint16_t mode);

int (graph_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (graph_draw_rectangle)(uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint32_t color);

int (graph_paint_screen)(uint32_t color);

int (graph_clear_screen)(void);

int (graph_draw)(void);

#endif /* end of include guard: GRAPHICS_H_INCLUDED */
