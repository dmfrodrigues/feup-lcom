#include <lcom/lcf.h>

#include "font.h"

#include "xpm_utils.h"
#include "graph.h"
#include "utils.h"
#include "errors.h"
#include <assert.h>

struct glyph{
    uint16_t w, h;
    uint8_t *map;
};
typedef struct glyph glyph_t;
static glyph_t* (glyph_ctor)(const char **xpm){
    if(xpm == NULL) return NULL;
    glyph_t *ret = malloc(sizeof(glyph_t));
    if(ret == NULL) return NULL;
    enum xpm_image_type type = XPM_8_8_8_8;
    xpm_image_t img;
    ret->map = xpm_load((xpm_map_t)xpm, type, &img);
    if(ret->map == NULL){
        free(ret);
        return NULL;
    }
    ret->w = img.width;
    ret->h = img.height;
    return ret;
}
static void (glyph_dtor)(glyph_t *p){
    if(p == NULL) return;
    free(p->map);
    free(p);
}
static int (glyph_draw_to_alpha_buffer)(const glyph_t *p, int16_t x, int16_t y, uint8_t *alp_buf, uint16_t W, uint16_t H){
    if(p == NULL) return NULL_PTR;
    int r;
    for(int16_t h = 0; h < p->h; ++h){
        for(int16_t w = 0; w < p->w; ++w){
            uint32_t c = *((uint32_t*)p->map + w + h*p->w);
            uint8_t a = GET_ALP(c);
            //c = c & 0xFF000000;
            //if(c != 0) printf("%d %d 0x%X\n", w, h, c);
            //else       printf("%d %d <========================\n");

            r = graph_set_pixel_alpha_buffer(x+w, y-p->h+h, a, alp_buf, W, H);


            //r = graph_set_pixel_alpha(x+w, y-p->h+h, GET_COLOR(c), GET_ALP(c));

            //printf("%d %d 0x%X\n", x, y, *(buf+(x+y*p->w)*sizeof(uint32_t)));
            if(r != SUCCESS && r != OUT_OF_RANGE) return r;
        }
    }
    return SUCCESS;
}

struct font{
    size_t nchars;
    glyph_t **glyphs;
};
#include "sprite.h"
#include "rectangle.h"
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
        ret->glyphs[i] = glyph_ctor((const char**)xpm);
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

struct text{
    const font_t *fnt;
    char *txt;
    int16_t x, y;
    int size;
    uint32_t color;
    enum text_valign valign;
    enum text_halign halign;
};
text_t* (text_ctor)(const font_t *fnt, const char *txt){
    if(fnt == NULL) return NULL;
    text_t *ret = malloc(sizeof(text_t));
    if(ret == NULL) return NULL;
    ret->fnt = fnt;
    ret->txt = NULL;
    text_set_text(ret, txt);
    ret->x = 0;
    ret->y = 0;
    ret->size = 25;
    ret->color = BLACK;
    ret->valign = text_valign_top;
    ret->halign = text_halign_left;
    return ret;
}
void (text_dtor)(text_t *p){
    if(p == NULL) return;
    free(p->txt);
    free(p);
}
void (text_set_text) (text_t *p, const char *txt){
    size_t sz = strlen(txt);
    p->txt = realloc(p->txt, (sz+1)*sizeof(char));
    if(p->txt == NULL) return;
    strcpy(p->txt, txt);
}
void (text_set_pos)   (text_t *p, int16_t x, int16_t y   ){ p->x = x; p->y = y; }
void (text_set_size)  (text_t *p, unsigned size          ){ p->size = size    ; }
void (text_set_color) (text_t *p, uint32_t color         ){ p->color = color  ; }
void (text_set_valign)(text_t *p, enum text_valign valign){ p->valign = valign; }
void (text_set_halign)(text_t *p, enum text_halign halign){ p->halign = halign; }

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
                if(g != NULL){ W += g->w; H = max(H, g->h); }
            }
        }
        uint8_t *alp_buf = malloc(W*H);
        if(alp_buf == NULL) return ALLOC_ERROR;{
            int16_t y = H;
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

        newH = H*factor;
        newW = W*factor;
        alp_new_buf = malloc(newW*newH);
        if(alp_new_buf == NULL) return ALLOC_ERROR;

        for(size_t newy = 0; newy < newH; ++newy){
            size_t y = newy/factor;
            for(size_t newx = 0; newx < newW; ++newx){
                size_t x = newx/factor;
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
            default: return LOGIC_ERROR;
        }
    }
    // Get initial value of y
    int16_t inity;{
        switch(p->valign){
            case text_valign_top   : inity = p->y         ; break;
            case text_valign_center: inity = p->y - newH/2; break;
            case text_valign_bottom: inity = p->y - newH  ; break;
            default: return LOGIC_ERROR;
        }
    }
    // Draw text
    for(int16_t newy = 0; newy < newH; ++newy){
        for(int16_t newx = 0; newx < newW; ++newx){
            uint8_t a = *(alp_new_buf+newx+newy*newW);
            graph_set_pixel_alpha(initx+newx,inity+newy,p->color, a);
        }
    }
    free(alp_new_buf);
    return SUCCESS;
}
