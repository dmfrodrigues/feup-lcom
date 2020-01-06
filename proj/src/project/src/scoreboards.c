#include <lcom/lcf.h>

#include "scoreboards.h"

#include "graph.h"
#include "rectangle.h"
#include "proj_func.h"
#include "proj_macros.h"
#include "rtc.h"
#include "errors.h"

#define MAX_HIGHSCORES   3 /** @brief Number of scores saved */

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
    rectangle_t *frame;
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

    return ret;
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
