#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <lcom/lcf.h>
#include <stdint.h>

int (get_permission)(unsigned int base_addr, unsigned int size);

int (get_permissions_first_mbyte)(void);

int (vbe_get_mode_information)(uint16_t mode);

phys_bytes get_phys_addr(void);

unsigned int get_vram_size(void);

uint16_t get_XRes(void);

uint16_t get_YRes(void);

uint16_t get_bits_pixel(void);

uint16_t get_bytes_pixel(void);

int (map_vram)(void);

int (free_memory)(void);

int (set_pixel)(uint16_t row, uint16_t col, uint32_t color);

/**
 * @brief
 * @param mode
 * @return
 */
int (set_graphics_mode)(uint16_t mode);

#endif /* end of include guard: GRAPHICS_H_INCLUDED */
