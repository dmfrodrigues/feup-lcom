#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <lcom/lcf.h>
#include <stdint.h>

int (get_permission)(unsigned int base_addr, unsigned int size);

int (get_permissions_first_mbyte)(void);

int (vbe_get_mode_information)(uint16_t mode, vbe_mode_info_t *vmi_p);

int (map_vram)(unsigned int vram_base, unsigned int vram_size);

int (free_memory)(void);

/**
 * @brief
 * @param mode
 * @return
 */
int (set_graphics_mode)(uint16_t mode);

#endif /* end of include guard: GRAPHICS_H_INCLUDED */
