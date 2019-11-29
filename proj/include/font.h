#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

struct font;
typedef struct font font_t;
font_t* (font_ctor)(const char *s);
void    (font_dtor)(font_t *p);

struct text;
typedef struct text text_t;
text_t* (text_ctor)(const font_t *fnt, const char *txt);
void    (text_dtor)(text_t *p);
void (text_set_text) (text_t *p, const char *txt);
void (text_set_pos)  (text_t *p, int16_t x, int16_t y);
void (text_set_size) (text_t *p, unsigned size);
void (text_set_color)(text_t *p, uint32_t color);
void (text_draw)     (const text_t *p);

#endif //FONT_H_INCLUDED
