#include "graphics.h"
#include "graphics_macros.h"
#include "errors.h"

#include <lcom/lcf.h>

#include <stdio.h>

static void *video_mem; /** @brief Frame-buffer VM address */
//static vbe_mode_info_t vbe_mem_info;
static mmap_t mem_map;

int (get_permission)(unsigned int base_addr, unsigned int size) {
    struct minix_mem_range mmr;
    mmr.mr_base = base_addr;
    mmr.mr_limit = base_addr + size;
    return sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mmr);
}

int (get_permissions_first_mbyte)(void) {
    return get_permission(MBYTE_BASE, MBYTE_SIZE);
}

int (vbe_get_mode_information)(uint16_t mode, vbe_mode_info_t *vmi_p) {
    memset(vmi_p, 0, sizeof(vbe_mode_info_t)); // reset values

    struct reg86 reg_86;
    memset(&reg_86, 0, sizeof(reg_86)); // reset struct

    vbe_mode_info_t *virtual_addr = lm_alloc(sizeof(vbe_mode_info_t), &mem_map);

    reg_86.intno = VC_BIOS_SERV;
    reg_86.ah = VBE_CALL;
    reg_86.al = VBE_MD_INFO;
    reg_86.cx = mode;
    reg_86.es = PB2BASE(mem_map.phys);
    reg_86.di = PB2OFF(mem_map.phys);
    // BIOS CALL
    if (sys_int86(&reg_86)) {
        printf("%s: sys_int86 failed\n", __func__);
        if (free_memory()) {
            printf("%s: lm_free failed\n", __func__);
            return LCF_ERROR;
        }
        return BIOS_CALL_ERROR;
    }

    vmi_p = memcpy((void*)vmi_p, (void*)virtual_addr, mem_map.size);
    return SUCCESS;
}

int (map_vram)(unsigned int vram_base, unsigned int vram_size) {
    int r;
    if ((r = get_permission(vram_base, vram_size)))
        panic("%s: sys_privctl (ADD MEM) failed: %d\n", __func__, r);

    video_mem = vm_map_phys(SELF, (void *)vram_base, vram_size);

    if (video_mem == MAP_FAILED)
        panic("%s: couldn't map video memory.", __func__);

    return SUCCESS;
}

int (free_memory)(void) {
    return lm_free(&mem_map);
}

int (set_graphics_mode)(uint16_t mode) {
    struct reg86 reg_86;

    memset(&reg_86, 0, sizeof(reg_86)); // reset struct

    // Set Reg86
    reg_86.intno = VC_BIOS_SERV;
    reg_86.ah = VBE_CALL;
    reg_86.al = SET_VBE_MD;
    reg_86.bx = mode | LINEAR_FRAME_BUFFER_MD;

    // BIOS CALL
    if (sys_int86(&reg_86)) {
        printf("%s: sys_int86 failed\n", __func__);
        return BIOS_CALL_ERROR;
    }

    return SUCCESS;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    /*
    uint8_t color8 = color;
    for(uint16_t l = 0; l < len; ++l){
        *(video_mem+y*1024+x+len) = color8;
    }*/
    return 0;
}
