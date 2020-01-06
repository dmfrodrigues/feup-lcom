#include <lcom/lcf.h>

#include "scoreboards.h"

#include "graph.h"
#include "rectangle.h"
#include "proj_func.h"
#include "proj_macros.h"
#include "rtc.h"
#include "errors.h"

#define MAX_HIGHSCORES   3 /** @brief Number of scores saved */

static const int W  = 400;
static const int H  = 80;
static const int W1 = 200;
static const int H1 = 60;
static const int H2 = 100;

struct  score_info {
    int day;
    int month;
    int year;
    int hour;
    int min;
    int sec;
    int score;
    int time_played;
};

struct highscores {
    score_info_t *highscores[MAX_HIGHSCORES];

    const font_t *fnt;
    rectangle_t *r[(MAX_HIGHSCORES+1)*3 + 1];
    text_t      *t[(MAX_HIGHSCORES+1)*3 + 1];
};

score_info_t* (score_ctor)(int score, int time_played){
    score_info_t *ret = (score_info_t*)malloc(sizeof(score_info_t));
    if (ret == NULL) return NULL;
    uint8_t score_date[4];
    if (rtc_read_time(score_date)) return NULL;
    ret->sec = score_date[0];
    ret->min = score_date[1];
    ret->hour = score_date[2];

    if (rtc_read_date(score_date)) return NULL;

    ret->day = score_date[1];
    ret->month = score_date[2];
    ret->year = score_date[3];

    ret->score = score;
    ret->time_played = time_played;

    return ret;
}

void (score_dtor)(score_info_t *p){
    if (p != NULL) free(p);
}

highscores_t* (highscores_ctor)(const font_t *fnt, const char *path) {
    if(fnt == NULL || path == NULL) return NULL;

    FILE *f = fopen(path, "r");
    if (f == NULL) { printf("NULL\n");return NULL;}
    highscores_t *ret = (highscores_t*)malloc(sizeof(highscores_t));
    if (ret == NULL) return NULL;
    for (int i = 0; i < MAX_HIGHSCORES; i++) {
        score_info_t *score = (score_info_t*)malloc(sizeof(score_info_t));
        memset(score, 0, sizeof(score_info_t));

        fscanf(f, "%02d/%02d/%02d %02d:%02d:%02d - %d - %d\n",
                    &(score->day), &(score->month), &(score->year),
                    &(score->hour), &(score->min), &(score->sec),
                    &(score->score), &(score->time_played));
        ret->highscores[i] = score;
    }
    fclose(f);

    ret->fnt = fnt;
    const uint16_t Wtotal = (MAX_HIGHSCORES+1)*W1;
    const uint16_t Htotal = (uint16_t)((MAX_HIGHSCORES+1)*H1);
    int16_t xi = graph_get_XRes()/2-Wtotal/2+W1/2;
    int16_t x = xi;
    int16_t y = graph_get_YRes()/2-Htotal/2-H1/2;

    {
        rectangle_t *r = rectangle_ctor(x, y, W1, H1);
        text_t      *t = text_ctor(ret->fnt, "Date");
        rectangle_set_fill_color   (r, GRAPH_BLACK);
        rectangle_set_outline_width(r, -2);
        rectangle_set_outline_color(r, GRAPH_WHITE);

        text_set_pos(t, x + W1/2, y + H1/2);
        text_set_valign(t, text_valign_center);
        text_set_halign(t, text_halign_center);
        text_set_color (t, TEXT_COLOR);
        ret->r[0] = r;
        ret->t[0] = t;

        x+=W1;

        r = rectangle_ctor(x, y, W1, H1);
        t = text_ctor(ret->fnt, "Score");
        rectangle_set_fill_color   (r, GRAPH_BLACK);
        rectangle_set_outline_width(r, -2);
        rectangle_set_outline_color(r, GRAPH_WHITE);

        text_set_pos(t, x + W1/2, y + H1/2);
        text_set_valign(t, text_valign_center);
        text_set_halign(t, text_halign_center);
        text_set_color (t, TEXT_COLOR);
        ret->r[1] = r;
        ret->t[1] = t;

        x+=W1;

        r = rectangle_ctor(x, y, W1, H1);
        t = text_ctor(ret->fnt, "Time Played");
        rectangle_set_fill_color   (r, GRAPH_BLACK);
        rectangle_set_outline_width(r, -2);
        rectangle_set_outline_color(r, GRAPH_WHITE);

        text_set_pos(t, x + W1/2, y + H1/2);
        text_set_valign(t, text_valign_center);
        text_set_halign(t, text_halign_center);
        text_set_color (t, TEXT_COLOR);
        ret->r[2] = r;
        ret->t[2] = t;


        x = xi;
    }
    y+= H1;

     for (int i = 0; i < MAX_HIGHSCORES*3; i++) {
         rectangle_t *r = rectangle_ctor(x, y, W1, H1);
         text_t      *t = text_ctor(ret->fnt, "");
         rectangle_set_fill_color   (r, GRAPH_BLACK);
         rectangle_set_outline_width(r, -2);
         rectangle_set_outline_color(r, GRAPH_WHITE);

         text_set_pos(t, x + W1/2, y + H1/2);
         text_set_valign(t, text_valign_center);
         text_set_halign(t, text_halign_center);
         text_set_color (t, TEXT_COLOR);

         x += W1;
         ret->r[MAX_HIGHSCORES + i] = r;
         ret->t[MAX_HIGHSCORES + i] = t;
         if ((i+1) % 3 == 0) {
             x = xi;
             y += H1;
         }
     }
     y += H2;
     x = graph_get_XRes()/2-W/2;
     {
         rectangle_t *r = rectangle_ctor(x, y, W, H);
         text_t      *t = text_ctor(ret->fnt, "Back");
         rectangle_set_fill_color   (r, GRAPH_BLACK);
         rectangle_set_outline_width(r, -2);
         rectangle_set_outline_color(r, GRAPH_WHITE);

         text_set_pos(t, x + W/2, y + H/2);
         text_set_valign(t, text_valign_center);
         text_set_halign(t, text_halign_center);
         text_set_color (t, TEXT_COLOR);
         ret->r[(MAX_HIGHSCORES+1)*3] = r;
         ret->t[(MAX_HIGHSCORES+1)*3] = t;
     }

     highscores_update_text(ret);

    return ret;
}

void (highscores_update_text)(highscores_t *p) {
    for (int i = 0; i < MAX_HIGHSCORES; i++) {
        char buffer[20];
        score_info_t *score = p->highscores[i];
        sprintf(buffer, "%02d/%02d/%02d %02d:%02d:%02d",
                    (score->day), (score->month), (score->year),
                    (score->hour), (score->month), (score->sec));


        text_set_string(p->t[MAX_HIGHSCORES + i*3], buffer);
        sprintf(buffer, "%d", score->score);
        text_set_string(p->t[MAX_HIGHSCORES + i*3+1], buffer);
        sprintf(buffer, "%d", score->time_played);
        text_set_string(p->t[MAX_HIGHSCORES + i*3+2], buffer);
    }
}

int (highscore_update_state)(highscores_t *p, int click) {
    if(!click) return -1;
    if (rectangle_collide_point(p->r[(MAX_HIGHSCORES+1)*3], *get_mouse_X(), *get_mouse_Y()))
        return 0;
    return -1;
}

void (highscores_draw)(highscores_t *p) {
    for(size_t i = 0; i < (MAX_HIGHSCORES+1)*3; ++i){
        rectangle_draw(p->r[i]);
        text_draw     (p->t[i]);
    }
    int x = *get_mouse_X(), y = *get_mouse_Y();
    if(rectangle_collide_point(p->r[(MAX_HIGHSCORES+1)*3], x, y))
        rectangle_set_fill_color(p->r[(MAX_HIGHSCORES+1)*3], HIGHLIGHT_COLOR);

    rectangle_draw(p->r[(MAX_HIGHSCORES+1)*3]);
    text_draw     (p->t[(MAX_HIGHSCORES+1)*3]);
    rectangle_set_fill_color(p->r[(MAX_HIGHSCORES+1)*3], GRAPH_BLACK);
}

void (check_new_score)(highscores_t *p, int score, int time_played){
    score_info_t *new_score = score_ctor(score, time_played);
    if (new_score == NULL) return;
    if (score > (p->highscores[0])->score) { // rank 1
        score_info_t *s = p->highscores[0];
        p->highscores[0] = new_score;
        new_score = s;
        s = p->highscores[1];
        p->highscores[1] = new_score;
        new_score = s;
        s = p->highscores[2];
        p->highscores[2] = new_score;
        score_dtor(s);
    } else if (score > (p->highscores[1])->score) { // rank 2
        score_info_t *s = p->highscores[1];
        p->highscores[1] = new_score;
        new_score = s;
        s = p->highscores[2];
        p->highscores[2] = new_score;
        score_dtor(s);
    } else if (score > (p->highscores[2])->score) { // rank 3
        score_info_t *s = p->highscores[2];
        p->highscores[2] = new_score;
        score_dtor(s);
    } else {
        score_dtor(new_score);
    }
}

void (highscores_save)(const highscores_t *p, const char *path) {
    if(p == NULL || path == NULL) return;

    FILE *f = fopen(path, "w");
    if (f == NULL) return;
    for (int i = 0; i < MAX_HIGHSCORES; i++) {
        score_info_t *score = p->highscores[i];
        fprintf(f, "%02d/%02d/%02d %02d:%02d:%02d - %d - %d\n",
                    (score->day), (score->month), (score->year),
                    (score->hour), (score->month), (score->sec),
                    (score->score), (score->time_played));
    }
    fclose(f);
}

void (highscores_dtor)(highscores_t *p) {
    if (p == NULL) return;
    for (int i = 0; i < MAX_HIGHSCORES; i++) {
        score_dtor(p->highscores[i]);
    }
    free(p);
}
