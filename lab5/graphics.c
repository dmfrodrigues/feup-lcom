#include "graphics.h"
#include "graphics_macros.h"
#include "errors.h"

#include <lcom/lcf.h>

#include <stdio.h>

static void *video_mem; /** @brief Frame-buffer VM address */
static vbe_mode_info_t vbe_mem_info;
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

int (vbe_get_mode_information)(uint16_t mode) {
    memset(&vbe_mem_info, 0, sizeof(vbe_mode_info_t)); // reset values

    struct reg86 reg_86;
    memset(&reg_86, 0, sizeof(struct reg86)); // reset struct

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

    memcpy((void*)&vbe_mem_info, (void*)virtual_addr, mem_map.size);
    return SUCCESS;
}

phys_bytes get_phys_addr(void) {
    return vbe_mem_info.PhysBasePtr;
}

unsigned int get_vram_size(void) {
    return vbe_mem_info.XResolution * vbe_mem_info.YResolution * ((vbe_mem_info.BitsPerPixel + 7) >> 3);
}

uint16_t get_XRes(void) {
    return vbe_mem_info.XResolution;
}

uint16_t get_YRes(void) {
    return vbe_mem_info.YResolution;
}

uint16_t get_bits_pixel(void) {
    return vbe_mem_info.BitsPerPixel;
}

uint16_t get_bytes_pixel(void) {
    return (vbe_mem_info.BitsPerPixel + 7) >> 3;
}

int (map_vram)(void) {
    int r;
    unsigned int vram_base = get_phys_addr();
    unsigned int vram_size = get_vram_size();
    if ((r = get_permission(vram_base, vram_size)))
        panic("%s: sys_privctl (ADD MEM) failed: %d\n", __func__, r);

    video_mem = vm_map_phys(SELF, (void *)vram_base, vram_size);

    if (video_mem == MAP_FAILED)
        panic("%s: couldn't map video memory.", __func__);

    return SUCCESS;
}

int (free_memory)(void) {
    return !lm_free(&mem_map);
}

int (set_pixel)(uint16_t row, uint16_t col, uint32_t color) {
    if (row >= vbe_mem_info.XResolution || col >= vbe_mem_info.YResolution) {
        printf("%s: invalid pixel.\n", __func__);
        return OUT_OF_RANGE;
    }
    unsigned int pos = (row + col * vbe_mem_info.XResolution) * get_bytes_pixel();
    memcpy((void*)((unsigned int)video_mem + pos), &color, get_bytes_pixel());
    return SUCCESS;
}

int (set_graphics_mode)(uint16_t mode) {
    struct reg86 reg_86;

    memset(&reg_86, 0, sizeof(struct reg86)); // reset struct

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

    //uint8_t color8 = color;
    uint8_t *p = video_mem;
    p += y*1024+x;
    //*(p++) = color8;
    //*(p++) = color8;
    //*(p++) = color8;
    /*
    for(void *p = video_mem; p-video_mem < len; ++p){
        *p = color;
    }
    */
    /*
    for(uint16_t l = 0; l < len; ++l){
        *((uint8_t*)(video_mem)+y*1024+x+len) = color8;
    }
    */
    return 0;
}
