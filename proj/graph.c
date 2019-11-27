#include "graph.h"
#include "graph_macros.h"
#include "errors.h"

#include <lcom/lcf.h>

#include <stdio.h>

/// MACROS
#define FAR2PHYS(n)         ((((n)>>12) & 0xFFFFFFF0) + ((n) & 0x0000FFFF))

/// STRUCT
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

static vbe_mode_info_t vbe_mem_info;

/// PUBLIC GET
uint16_t   (graph_get_XRes)         (void){ return vbe_mem_info.XResolution; }
uint16_t   (graph_get_YRes)         (void){ return vbe_mem_info.YResolution; }

/// PRIVATE GET
static uint16_t   (graph_get_bits_pixel)   (void){ return vbe_mem_info.BitsPerPixel; }
static uint16_t   (graph_get_bytes_pixel)  (void){ return (graph_get_bits_pixel() + 7) >> 3; }
static phys_bytes (graph_get_phys_addr)    (void){ return vbe_mem_info.PhysBasePtr; }
static unsigned   (graph_get_vram_size)    (void){ return vbe_mem_info.XResolution * vbe_mem_info.YResolution * graph_get_bytes_pixel(); }
//static uint16_t   (graph_get_RedMaskSize)  (void){ return vbe_mem_info.RedMaskSize  ; }
//static uint16_t   (graph_get_GreenMaskSize)(void){ return vbe_mem_info.GreenMaskSize; }
//static uint16_t   (graph_get_BlueMaskSize) (void){ return vbe_mem_info.BlueMaskSize ; }

static int (get_permission)(unsigned int base_addr, unsigned int size) {
    struct minix_mem_range mmr;
    mmr.mr_base = base_addr;
    mmr.mr_limit = base_addr + size;
    return sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mmr);
}

//static int (get_permissions_first_mbyte)(void) {
//    return get_permission(MBYTE_BASE, MBYTE_SIZE);
//}

/// MEMORY
static void    *video_mem = NULL; /** @brief Frame-buffer VM address. */
static uint8_t *video_buf = NULL; /** @brief Primary buffer for drawing before copying to video_mem. */
static mmap_t mem_map;
static int (graph_free_memory)(void) {
    int r = SUCCESS;
    free(video_buf); video_buf = NULL;
    r = !lm_free(&mem_map);
    return r;
}
static int (graph_map_vram)(void) {
    int r;
    const unsigned vram_base = graph_get_phys_addr();
    const unsigned vram_size = graph_get_vram_size();
    if ((r = get_permission(vram_base, vram_size))) {
        if (graph_free_memory()) {
            printf("%s: lm_free failed\n", __func__);
        }
        panic("%s: sys_privctl (ADD MEM) failed: %d\n", __func__, r);
    }

    video_mem = vm_map_phys(SELF, (void *)vram_base, vram_size);

    if (video_mem == MAP_FAILED) {
        if (graph_free_memory()) {
            printf("%s: lm_free failed\n", __func__);
        }
        panic("%s: couldn't map video memory.", __func__);
    }

    video_buf = malloc(vram_size);

    return SUCCESS;
}

/// INFO GET
static int (vbe_get_mode_information)(uint16_t mode) {
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
    if (sys_int86(&reg_86) || reg_86.ah != AH_SUCCESS) {
        printf("%s: sys_int86 failed\n", __func__);
        if (graph_free_memory()) {
            printf("%s: lm_free failed\n", __func__);
        }
        return BIOS_CALL_ERROR;
    }

    memcpy((void*)&vbe_mem_info, (void*)virtual_addr, mem_map.size);
    return SUCCESS;
}
/*
static int (vbe_get_controller_information)(vg_vbe_contr_info_t *info_p) {
    memset(info_p, 0, sizeof(vg_vbe_contr_info_t)); // reset values

    mmap_t controller_map;

    struct reg86 reg_86;
    memset(&reg_86, 0, sizeof(struct reg86)); // reset struct

    VbeInfoBlock *virtual_addr = lm_alloc(sizeof(VbeInfoBlock), &controller_map);

    uint32_t virtual_base = (uint32_t)(virtual_addr) - controller_map.phys;

    virtual_addr->VbeSignature[0] = 'V';
    virtual_addr->VbeSignature[1] = 'B';
    virtual_addr->VbeSignature[2] = 'E';
    virtual_addr->VbeSignature[3] = '2';


    reg_86.intno = VC_BIOS_SERV;
    reg_86.ah = VBE_CALL;
    reg_86.al = VBE_CTRL_INFO;
    reg_86.es = PB2BASE(controller_map.phys);
    reg_86.di = PB2OFF(controller_map.phys);
    // BIOS CALL
    if (sys_int86(&reg_86) || reg_86.ah != AH_SUCCESS) {
        printf("%s: sys_int86 failed\n", __func__);
        if (!lm_free(&controller_map)) {
            printf("%s: lm_free failed\n", __func__);
        }
        return BIOS_CALL_ERROR;
    }

    info_p->VBESignature[0] = virtual_addr->VbeSignature[0];
    info_p->VBESignature[1] = virtual_addr->VbeSignature[1];
    info_p->VBESignature[2] = virtual_addr->VbeSignature[2];
    info_p->VBESignature[3] = virtual_addr->VbeSignature[3];

    uint8_t lsb, msb;
    util_get_LSB(virtual_addr->VbeVersion, &lsb);
    util_get_MSB(virtual_addr->VbeVersion, &msb);
    info_p->VBEVersion[0] = lsb;
    info_p->VBEVersion[1] = msb;

    info_p->TotalMemory = (virtual_addr->TotalMemory << 6);

    // Convert Far Far Pointer to Virtual Address

    uint32_t phys_ptr = FAR2PHYS(virtual_addr->OemStringPtr);
    uint32_t virtual_ptr = phys_ptr + virtual_base;
    info_p->OEMString = (char*)(virtual_ptr);

    phys_ptr = FAR2PHYS(virtual_addr->VideoModePtr);
    virtual_ptr = phys_ptr + virtual_base;
    info_p->VideoModeList = (uint16_t*)(virtual_ptr);

    phys_ptr = FAR2PHYS(virtual_addr->OemVendorNamePtr);
    virtual_ptr = phys_ptr + virtual_base;
    info_p->OEMVendorNamePtr = (char*)(virtual_ptr);

    phys_ptr = FAR2PHYS(virtual_addr->OemProductNamePtr);
    virtual_ptr = phys_ptr + virtual_base;
    info_p->OEMProductNamePtr = (char*)(virtual_ptr);

    phys_ptr = FAR2PHYS(virtual_addr->OemProductRevPtr);
    virtual_ptr = phys_ptr + virtual_base;
    info_p->OEMProductRevPtr = (char*)(virtual_ptr);

    if (!lm_free(&controller_map)) {
        printf("%s: lm_free failed\n", __func__);
        return LCF_ERROR;
    }

    return SUCCESS;
}
*/

/// INIT
/**
 * @brief
 * @param mode
 * @return
 */
static int (graph_set_mode)(uint16_t mode) {
    struct reg86 reg_86;

    memset(&reg_86, 0, sizeof(struct reg86)); // reset struct

    // Set Reg86
    reg_86.intno = VC_BIOS_SERV;
    reg_86.ah = VBE_CALL;
    reg_86.al = SET_VBE_MD;
    reg_86.bx = mode | LINEAR_FRAME_BUFFER_MD;

    // BIOS CALL
    if (sys_int86(&reg_86) || reg_86.ah != AH_SUCCESS) {
        printf("%s: sys_int86 failed\n", __func__);
        return BIOS_CALL_ERROR;
    }

    return SUCCESS;
}
int (graph_init)(uint16_t mode){
    if (vbe_get_mode_information(mode)) {
        printf("%s: failed to get information for mode %x.\n", __func__, mode);
        return 1;
    }

    graph_map_vram(); // if function fails it aborts program

    if (graph_set_mode(mode)) {
        printf("%s: failed to set graphic mode %x.\n", __func__, mode);
        return 1;
    };
    return SUCCESS;
}

/// CLEANUP
int (graph_cleanup)(void){
    int r = SUCCESS;
    if ((r = vg_exit()))
        printf("%s: vg_exit failed to exit to text mode.\n", __func__);
    if ((r = graph_free_memory()))
        printf("%s: lm_free failed\n", __func__);
    return r;
}

/// PIXEL DRAWING
int (graph_set_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    if (x >= vbe_mem_info.XResolution || y >= vbe_mem_info.YResolution) {
        //printf("%s: invalid pixel.\n", __func__);
        return OUT_OF_RANGE;
    }
    unsigned int pos = (x + y * vbe_mem_info.XResolution) * graph_get_bytes_pixel();
    memcpy(video_buf + pos, &color, graph_get_bytes_pixel());
    return SUCCESS;
}
int (graph_set_pixel_alpha)(uint16_t x, uint16_t y, uint32_t color, uint8_t alpha){
    if (x >= vbe_mem_info.XResolution || y >= vbe_mem_info.YResolution) {
        //printf("%s: invalid pixel.\n", __func__);
        return OUT_OF_RANGE;
    }
    unsigned int pos = (x + y * vbe_mem_info.XResolution) * graph_get_bytes_pixel();
    uint32_t color_;
    memcpy(&color_, video_buf + pos, graph_get_bytes_pixel());
    float a = 1.0-(alpha&0xFF)/(float)0xFF;
    uint8_t r = GET_RED(color)*a + GET_RED(color_)*(1.0-a);
    uint8_t g = GET_GRE(color)*a + GET_GRE(color_)*(1.0-a);
    uint8_t b = GET_BLU(color)*a + GET_BLU(color_)*(1.0-a);
    return graph_set_pixel(x,y,SET_RGB(r,g,b));
    //return set_pixel(x,y,color);
}

int (graph_clear_screen)(void){
    //return graph_paint_screen(BLACK);
    memset(video_buf, 0, graph_get_vram_size());
    return SUCCESS;
}

int (graph_draw)(void){
    memcpy((uint8_t*)video_mem, video_buf, graph_get_vram_size());
    return 0;
}

/// RECTANGLE
int (graph_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    int r;
    for (uint16_t i = 0; i < len; i++)
        if ((r = graph_set_pixel(x + i, y, color))) return r;
    return SUCCESS;
}

int (graph_draw_rectangle)(uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint32_t color)	{
    int r;
    for (uint16_t i = 0; i < height; i++)
        if ((r = graph_draw_hline(x, y + i, width, color))) return r;
    return SUCCESS;
}
