#include <lcom/lcf.h>

#include "font.h"

#include "xpm_utils.h"
#include "graph.h"
#include "utils.h"
#include "errors.h"
#include <assert.h>

/**
 * @brief Text glyph. Represents a letter that can be drawn.
 */
typedef struct {
    uint16_t w, h;
    uint8_t *map;
} glyph_t;
/**
 * @brief Construct glyph.
 * @param   xpm XPM describing the glyph
 * @return      Pointer to created glyph
 */
static glyph_t* (glyph_ctor)(const char *const *xpm){
    if(xpm == NULL) return NULL;
    glyph_t *ret = malloc(sizeof(glyph_t));
    if(ret == NULL) return NULL;
    enum xpm_image_type type = XPM_8_8_8_8;
    xpm_image_t img;
    ret->map = NULL;
    uint8_t *map = xpm_load((xpm_map_t)xpm, type, &img);
    if(map == NULL){
        free(ret);
        return NULL;
    }
    ret->w = img.width;
    ret->h = img.height;
    ret->map = malloc(ret->w*ret->h*sizeof(uint8_t));
    if(ret->map == NULL){
        free(ret);
        return NULL;
    }
    for(unsigned i = 0; i < ret->w*ret->h; ++i){
        ret->map[i] = map[4*i+3];
    }
    return ret;
}
/**
 * @brief Destruct glyph.
 * @param   p   Pointer to glyph to be destroyed
 */
static void (glyph_dtor)(glyph_t *p){
    if(p == NULL) return;
    free(p->map);
    free(p);
}
/**
 * @brief Draw glyph to an alpha-buffer.
 *
 * An alpha-buffer is an array of uint8_t that represents transparencies.
 * It proves fit as a tool for drawing text.
 * @param   p       Pointer to glyph to be drawn
 * @param   x       X-position to draw the glyph
 * @param   y       Y-position to draw the glyph
 * @param   alp_buf Alpha-buffer to which the glyph will be drawn
 * @param   W       Width of the alpha-buffer
 * @param   H       Height of the alpha-buffer
 */
static int (glyph_draw_to_alpha_buffer)(const glyph_t *p, int16_t x, int16_t y, uint8_t *alp_buf, uint16_t W, uint16_t H){
    if(p == NULL) return NULL_PTR;
    for(int16_t h = 0; h < p->h; ++h){
        for(int16_t w = 0; w < p->w; ++w){
            uint8_t a = *(p->map + w + h*p->w);
            int16_t x_ = x+w, y_ = y-p->h+h;
            int32_t pos = x_ +y_*W;
            if(0 <= pos && pos < W*H) alp_buf[pos] = a;
        }
    }
    return SUCCESS;
}

/// FONT
struct font{
    size_t nchars;
    glyph_t **glyphs;
};
font_t* (font_ctor)(const char *s){
    font_t *ret = malloc(sizeof(font_t));
    if(ret == NULL) return NULL;
    ret->nchars = 128;
    ret->glyphs = malloc(ret->nchars*sizeof(glyph_t*));
    if(ret->glyphs == NULL){
        free(ret->glyphs);
        free(ret);
        return NULL;
    }
    int good = false;
    char filepath[1024];
    for(size_t i = 0; i < ret->nchars; ++i){
        sprintf(filepath, "%s/ascii%03d.xpm2", s, i);
        char **xpm = xpm_load_xpm2(filepath);
        ret->glyphs[i] = glyph_ctor((const char *const *)xpm);
        if(ret->glyphs[i] != NULL) good = true;
    }
    if(good) return ret;
    else{
        //font_dtor(ret);
        return NULL;
    }
}
void (font_dtor)(font_t *p){
    if(p == NULL) return;
    for(size_t i = 0; i < p->nchars; ++i)
        glyph_dtor(p->glyphs[i]);
    free(p->glyphs);
    free(p);
}

static font_t *consolas     = NULL;
static font_t *default_font = NULL;

int (font_init)(void){
    consolas = font_ctor("/home/lcom/labs/proj/media/font/Consolas/xpm2");
    if(consolas == NULL) return NULL_PTR;
    default_font = consolas;
    return SUCCESS;
}
const font_t* font_get_default(void){ return default_font; }
const font_t* font_get_consolas(void){ return consolas; }
void (font_free)(void){
    font_dtor(consolas);
    consolas = NULL;
    default_font = NULL;
}

/// TEXT
#include "text.h"

struct text{
    const font_t *fnt;
    char *txt;
    int16_t x, y;
    uint16_t size;
    text_valign valign;
    text_halign halign;
    uint32_t color;
};
text_t* (text_ctor)(const font_t *fnt, const char *txt){
    if(fnt == NULL) return NULL;
    text_t *ret = malloc(sizeof(text_t));
    if(ret == NULL) return NULL;
    ret->fnt = fnt;
    ret->txt = NULL;
    text_set_string(ret, txt);
    ret->x = 0;
    ret->y = 0;
    ret->size = 25;
    ret->color = GRAPH_BLACK;
    ret->valign = text_valign_top;
    ret->halign = text_halign_left;
    return ret;
}
void (text_dtor)(text_t *p){
    if(p == NULL) return;
    free(p->txt);
    free(p);
}
void (text_set_string) (text_t *p, const char *txt){
    size_t sz = strlen(txt);
    p->txt = realloc(p->txt, (sz+1)*sizeof(char));
    if(p->txt == NULL) return;
    strcpy(p->txt, txt);
}
const char* (text_get_string)(const text_t *p){return p->txt; }
void (text_set_pos)   (text_t *p, int16_t x, int16_t y){ p->x = x; p->y = y; }
void (text_set_size)  (text_t *p, uint16_t size       ){ p->size = size    ; }
void (text_set_color) (text_t *p, uint32_t color      ){ p->color = color  ; }
void (text_set_valign)(text_t *p, text_valign valign  ){ p->valign = valign; }
void (text_set_halign)(text_t *p, text_halign halign  ){ p->halign = halign; }
int16_t (text_get_x)  (const text_t *p){ return p->x; }
int16_t (text_get_y)  (const text_t *p){ return p->y; }
int (text_draw)(const text_t *p){
    if(p == NULL) return NULL_PTR;
    int ret = SUCCESS;
    // Get buffer with rescaled text
    uint8_t *alp_new_buf = NULL;
    uint16_t newH, newW;{
        const size_t len = strlen(p->txt);
        uint16_t W = 0, H = 0; {
            for(size_t i = 0; i < len; ++i){
                const glyph_t *g = p->fnt->glyphs[(size_t)p->txt[i]];
                if(g != NULL){ W += g->w; H = max_u16(H, g->h); }
            }
        }
        uint8_t *alp_buf = malloc(W*H);
        if(alp_buf == NULL) return ALLOC_ERROR;{
            int16_t y = (int16_t)H;
            int16_t x = 0;
            for(size_t i = 0; i < len; ++i){
                const glyph_t *g = p->fnt->glyphs[(size_t)p->txt[i]];
                if(g != NULL){
                    if((ret = glyph_draw_to_alpha_buffer(g, x, y, alp_buf, W, H))) return ret;
                    x += g->w;
                }
            }
        }

        double factor = (double)p->size/(double)H;

        newH = (uint16_t)(H*factor);
        newW = (uint16_t)(W*factor);
        alp_new_buf = malloc(newW*newH);
        if(alp_new_buf == NULL) return ALLOC_ERROR;

        for(uint16_t newy = 0; newy < newH; ++newy){
            uint16_t y = (uint16_t)(newy/factor);
            for(uint16_t newx = 0; newx < newW; ++newx){
                uint16_t x = (uint16_t)(newx/factor);
                *(alp_new_buf+newx+newy*newW) = *(alp_buf+x+y*W);
            }
        }
        free(alp_buf);
    }
    // Get initial value of x
    int16_t initx;{
        switch(p->halign){
            case text_halign_left  : initx = p->x         ; break;
            case text_halign_center: initx = p->x - newW/2; break;
            case text_halign_right : initx = p->x - newW  ; break;
            //default: return LOGIC_ERROR;
        }
    }
    // Get initial value of y
    int16_t inity;{
        switch(p->valign){
            case text_valign_top   : inity = p->y         ; break;
            case text_valign_center: inity = p->y - newH/2; break;
            case text_valign_bottom: inity = p->y - newH  ; break;
            //default: return LOGIC_ERROR;
        }
    }
    // Draw text
    for(int16_t x, y, newy = 0; newy < newH; ++newy){
        y = inity+newy;
        for(int16_t newx = 0; newx < newW; ++newx){
            x = initx+newx;
            if(!(0 <= x && x < graph_get_XRes() &&
                 0 <= y && y < graph_get_YRes())) continue;
            uint8_t a = *(alp_new_buf+newx+newy*newW);
            if(a < ALPHA_THRESHOLD) graph_set_pixel((uint16_t)x,(uint16_t)y,p->color);
        }
    }
    free(alp_new_buf);
    return SUCCESS;
}
