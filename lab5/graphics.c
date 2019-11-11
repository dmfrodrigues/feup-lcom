#include "graphics.h"
#include "graphics_macros.h"
#include "errors.h"

#include <lcom/lcf.h>

#include <stdio.h>

static void *video_mem; /** @brief Frame-buffer VM address */
static vbe_mode_info_t vbe_mem_info;
//static mmap_t mem_map;

int (set_graphics_mode)(uint16_t mode) {
    int r;
    struct minix_mem_range mmr;
    mmr.mr_base =  0x0;
    mmr.mr_limit = 0xFFFFF;

    struct reg86 reg_86;

    if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mmr)))
        panic("sys_privctl (ADD MEM) failed: %d\n", r);

    lm_init(true);
//    lm_alloc(sizeof(vbe_mode_info_t), &mem_map);
    if (vbe_get_mode_info(mode, &vbe_mem_info)) {
        printf("vbe_get_mode_info: Failed to get VBE Mode Info for mode %x\n", mode);
        return LCF_ERROR;
    }
    unsigned int vram_base = vbe_mem_info.PhysBasePtr;
    unsigned int vram_size = vbe_mem_info.XResolution * vbe_mem_info.YResolution * (vbe_mem_info.BitsPerPixel >> 3);

    // Allow memory mapping
    mmr.mr_base =  vram_base;
    mmr.mr_limit = vram_base + vram_size;

    if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mmr)))
        panic("sys_privctl (ADD MEM) failed: %d\n", r);

    // Map memory

    video_mem = vm_map_phys(SELF, (void *)mmr.mr_base, vram_size);

    if (video_mem == MAP_FAILED)
        panic("Error: couldn't map video memory.");

        memset(&reg_86, 0, sizeof(reg_86)); // reset struct

        reg_86.intno = VC_BIOS_SERV;
        reg_86.ah = VBE_CALL;
        reg_86.al = SET_VBE_MD;
        reg_86.bx = mode | LINEAR_FRAME_BUFFER_MD;

        // BIOS CALL

        if (sys_int86(&reg_86)) {
            printf("%s: sys_int86 failed\n", __func__);
            return BIOS_CALL_ERROR;
        }

        memset(&vbe_mem_info, 0, sizeof(vbe_mode_info_t)); // clear mem_info to initialize it

//    lm_free(&mem_map);
    return SUCCESS;
}
