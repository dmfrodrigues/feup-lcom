#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

struct glyph;
typedef struct glyph glyph_t;

glyph_t* (glyph_ctor)(const char **xpm);
void     (glyph_dtor)(glyph_t *p);

//glyph_t** (get_font)(const char *s);

#endif //FONT_H_INCLUDED
