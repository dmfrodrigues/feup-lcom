#include <lcom/lcf.h>

#include "graph.h"

#include "errors.h"
#include <stdio.h>

#define VC_BIOS_SERV  0x10 /** @brief Interrupt service of video card */
#define VBE_CALL      0x4F /** @brief VBE call function specifier */

#define MBYTE_BASE  0x0         /** @brief Base address (zero address) */
#define MBYTE_SIZE  0xFFFFF     /** @brief Size of a mebibyte */

// Graphics Functions
#define VBE_CTRL_INFO       0x00    /** @brief Get VBE Controller Information */
#define VBE_MD_INFO         0x01    /** @brief Get VBE Mode Information */
#define SET_VBE_MD          0x02    /** @brief Set VBE Mode */

// Error codes (AH)
#define AH_SUCCESS          0x00    /** @brief Success code on BIOS call */
#define AH_FUNC_CALL_FAIL   0x01    /** @brief Function call failed */
#define AH_FUNC_NOT_SUPP    0x02    /** @brief Function call is not supported in current HW configuration */
#define AH_FUNC_INVALID     0x03    /** @brief Invalid function in current video mode */

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

/// PRIVATE GET
static uint16_t   (graph_get_bits_pixel)   (void){ return vbe_mem_info.BitsPerPixel; }
static phys_bytes (graph_get_phys_addr)    (void){ return vbe_mem_info.PhysBasePtr; }
static unsigned   (graph_get_vram_size)    (void){ return vbe_mem_info.XResolution * vbe_mem_info.YResolution * graph_get_bytes_pixel(); }
//static uint16_t   (graph_get_RedMaskSize)  (void){ return vbe_mem_info.RedMaskSize  ; }
//static uint16_t   (graph_get_GreenMaskSize)(void){ return vbe_mem_info.GreenMaskSize; }
//static uint16_t   (graph_get_BlueMaskSize) (void){ return vbe_mem_info.BlueMaskSize ; }

/// PUBLIC GET
uint16_t   (graph_get_XRes)         (void){ return vbe_mem_info.XResolution; }
uint16_t   (graph_get_YRes)         (void){ return vbe_mem_info.YResolution; }
uint16_t   (graph_get_bytes_pixel)  (void){ return (graph_get_bits_pixel() + 7) >> 3; }

///
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
static uint8_t *video_mem = NULL; /** @brief Frame-buffer VM address. */
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
    //pixels are certain to be inside can reduce lag
    /*if (x < 0 || vbe_mem_info.XResolution <= x || y < 0 || vbe_mem_info.YResolution <= y) {
        //printf("%s: invalid pixel.\n", __func__);
        return OUT_OF_RANGE;
    }*/
    unsigned int pos = (x + y * vbe_mem_info.XResolution) * 3/*graph_get_bytes_pixel()*/;
    memcpy(video_buf + pos, &color, 3/*graph_get_bytes_pixel()*/);
    return SUCCESS;
}
void (graph_set_pixel_pos)(unsigned pos, uint32_t color){
    memcpy(video_buf + pos, &color, graph_get_bytes_pixel());
}
int (graph_clear_screen)(void){ memset(video_buf, 0, graph_get_vram_size()); return SUCCESS; }
int (graph_draw)(void){ memcpy(video_mem, video_buf, graph_get_vram_size()); return SUCCESS; }

///SPRITE
#include "utils.h"
#include "fast_math.h"
#include <math.h>

struct basic_sprite{
    uint8_t *map;
    uint16_t w, h;
    int16_t u0, v0;
};
basic_sprite_t* (basic_sprite_ctor)(const char **xpm, int16_t u0, int16_t v0){
    basic_sprite_t *ret = malloc(sizeof(basic_sprite_t));
    if(ret == NULL) return NULL;
    enum xpm_image_type type = XPM_8_8_8_8;
    xpm_image_t img;
    ret->map = xpm_load((xpm_map_t)xpm, type, &img);
    if(ret->map == NULL){
        basic_sprite_dtor(ret);
        return NULL;
    }
    ret->w = img.width;
    ret->h = img.height;
    ret->u0 = u0;
    ret->v0 = v0;
    return ret;
}
void (basic_sprite_dtor)(basic_sprite_t *p){
    if(p == NULL) return;
    free(p->map);
    free(p);
}
const uint8_t* (basic_sprite_get_map)(const basic_sprite_t *p){ return p->map; }
uint16_t       (basic_sprite_get_w)  (const basic_sprite_t *p){ return p->w  ; }
uint16_t       (basic_sprite_get_h)  (const basic_sprite_t *p){ return p->h  ; }
int16_t        (basic_sprite_get_u0) (const basic_sprite_t *p){ return p->u0 ; }
int16_t        (basic_sprite_get_v0) (const basic_sprite_t *p){ return p->v0 ; }

/*
struct basic_sprite_alpha{
    uint8_t *map;
    uint16_t w, h;
    int16_t u0, v0;
};
basic_sprite_alpha_t* (basic_sprite_alpha_ctor)(const char **xpm, int16_t u0, int16_t v0){
    basic_sprite_alpha_t *ret = malloc(sizeof(basic_sprite_t));
    if(ret == NULL) return NULL;
    enum xpm_image_type type = XPM_8_8_8_8;
    xpm_image_t img;
    ret->map = NULL;
    uint8_t *m = xpm_load((xpm_map_t)xpm, type, &img);
    if(m == NULL){
        basic_sprite_alpha_dtor(ret);
        return NULL;
    }
    ret->map = m;
    if(ret->map == NULL){
        basic_sprite_alpha_dtor(ret);
        return NULL;
    }
    ret->w = img.width;
    ret->h = img.height;
    ret->u0 = u0;
    ret->v0 = v0;
    return ret;
}
void (basic_sprite_alpha_dtor)(basic_sprite_alpha_t *p){
    if(p == NULL) return;
    free(p->map);
    free(p);
}
const uint8_t* (basic_sprite_alpha_get_map)(const basic_sprite_alpha_t *p){ return p->map; }
uint16_t       (basic_sprite_alpha_get_w)  (const basic_sprite_alpha_t *p){ return p->w  ; }
uint16_t       (basic_sprite_alpha_get_h)  (const basic_sprite_alpha_t *p){ return p->h  ; }
int16_t        (basic_sprite_alpha_get_u0) (const basic_sprite_alpha_t *p){ return p->u0 ; }
int16_t        (basic_sprite_alpha_get_v0) (const basic_sprite_alpha_t *p){ return p->v0 ; }
*/

struct sprite{
    const basic_sprite_t *bsp;
    int16_t x, y; //position in screen
    int16_t su0, sv0;
    float theta, s, c;
    float scale;
    uint8_t *sbuf;
};
sprite_t* (sprite_ctor)(const basic_sprite_t *bsp){
    sprite_t *ret = malloc(sizeof(sprite_t));
    if(ret == NULL) return NULL;
    ret->bsp = bsp;
    ret->sbuf = NULL;
    ret->x = 0;
    ret->y = 0;
    sprite_set_angle(ret, 0.0);
    sprite_set_scale(ret, 1.0);
    return ret;
}
void (sprite_dtor)(sprite_t *p){
    if(p == NULL) return;
    free(p->sbuf);
    free(p);
}
void (sprite_set_pos)   (sprite_t *p, int16_t x , int16_t y ){ p->x = x; p->y = y; }
void (sprite_set_angle) (sprite_t *p, double angle          ){ p->theta = angle; p->c = fm_cos(p->theta); p->s = fm_sin(p->theta); }
void (sprite_set_scale) (sprite_t *p, double scale          ){
    if(p->scale == scale) return;
    p->scale = scale;

    p->su0 = p->bsp->u0*p->scale;
    p->sv0 = p->bsp->u0*p->scale;

    const uint16_t W = basic_sprite_get_w(p->bsp),
                   H = basic_sprite_get_h(p->bsp);
    uint16_t sW = W*scale, sH = H*scale;
    p->sbuf = realloc(p->sbuf, sW*sH*4);
    const uint8_t *map = basic_sprite_get_map(p->bsp);
    for(uint16_t sx = 0; sx < sW; ++sx){
        for(uint16_t sy = 0; sy < sH; ++sy){
            uint16_t x = sx/scale, y = sy/scale;
            if(x > W || y > H) continue;
            memcpy(p->sbuf+4*(sx+sy*sW), map+4*(x+y*W), 4);
        }
    }
}
int16_t  (sprite_get_x)(const sprite_t *p){ return p->x; }
int16_t  (sprite_get_y)(const sprite_t *p){ return p->y; }
double   (sprite_get_angle)(const sprite_t *p){ return p->theta; }
uint16_t (sprite_get_w)(const sprite_t *p){ return basic_sprite_get_w(p->bsp); }
uint16_t (sprite_get_h)(const sprite_t *p){ return basic_sprite_get_h(p->bsp); }
void (sprite_src2sbuf)(const sprite_t *p, int16_t x, int16_t y, int16_t *u, int16_t *v){
    if(p->theta == 0.0){
        *u = x - p->x + p->su0;
        *v = y - p->y + p->sv0;
    }else{
        float dx = x - p->x;
        float dy = y - p->y;
        int16_t du = dx*p->c - dy*p->s - 0.5f;
        int16_t dv = dx*p->s + dy*p->c - 0.5f;
        *u = du + p->su0;
        *v = dv + p->sv0;
    }
}
void (sprite_sbuf2src)(const sprite_t *p, int16_t u, int16_t v, int16_t *x, int16_t *y){
    int16_t du = u - p->su0;
    int16_t dv = v - p->sv0;
    double dx =  du*p->c + dv*p->s;
    double dy = -du*p->s + dv*p->c;
    *x = dx + 0.5 + p->x;
    *y = dy + 0.5 + p->y;
}

void (sprite_draw)(const sprite_t *p){
    const uint16_t sw = p->scale*basic_sprite_get_w(p->bsp);
    const uint16_t sh = p->scale*basic_sprite_get_h(p->bsp);
    int16_t xmin, xmax, ymin, ymax; {
        int16_t x, y;
        sprite_sbuf2src(p, 0 , 0 , &x, &y);
        xmin = x; xmax = x; ymin = y; ymax = y;
        sprite_sbuf2src(p, sw, 0 , &x, &y);
        xmin = min(x, xmin); xmax = max(x, xmax); ymin = min(y, ymin); ymax = max(y, ymax);
        sprite_sbuf2src(p, 0 , sh, &x, &y);
        xmin = min(x, xmin); xmax = max(x, xmax); ymin = min(y, ymin); ymax = max(y, ymax);
        sprite_sbuf2src(p, sw, sh, &x, &y);
        xmin = min(x, xmin); xmax = max(x, xmax); ymin = min(y, ymin); ymax = max(y, ymax);
        xmin = max(xmin-2, 0); xmax = min(xmax+2, graph_get_XRes());
        ymin = max(ymin-2, 0); ymax = min(ymax+2, graph_get_YRes());
    }
    const uint16_t bytes_pixel = 3/*graph_get_bytes_pixel()*/;
    for(int16_t u, v, y = ymin; y < ymax; ++y){
        uint8_t *place = video_buf + (xmin + y*graph_get_XRes())*bytes_pixel;
        for(int16_t x = xmin; x < xmax; ++x, place += bytes_pixel){
            sprite_src2sbuf(p, x, y, &u, &v);
            //u = x; v = y;
            if(0 <= u && u < sw && 0 <= v && v < sh){
                const uint8_t *c_p = p->sbuf+(v*sw+u)*4;
                if(*(c_p+3) < ALPHA_THRESHOLD){ //alpha
                    memcpy(place, c_p, bytes_pixel);
                }
            }
        }
    }
}
