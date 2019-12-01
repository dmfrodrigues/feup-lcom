#ifdef LCOM_MACRO
    #include <lcom/lcf.h>
#endif

#include "xpm_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void xpm_save_as_xpm2(char **p, const char *s){
    FILE *f = fopen(s, "w");
    int w, h, num_colors, chars_per_pixel;
    sscanf(p[0], "%d %d %d %d", &w, &h, &num_colors, &chars_per_pixel);
    for(int i = 0; i < 1+num_colors+h; ++i){
        fprintf(f, "%s\n", p[i]);
    }
    fclose(f); f = NULL;
}

char** xpm_load_xpm2(const char *fpath){
    FILE *f = fopen(fpath, "r");
    if(f == NULL) return NULL;
    size_t len = 1024; char *line_buf = malloc(len*sizeof(char));
    int sz;

    char **ret = NULL;

    int w, h, num_colors, chars_per_pixel;

    sz = getline(&line_buf, &len, f);{
        sscanf(line_buf, "%d %d %d %d", &w, &h, &num_colors, &chars_per_pixel);
        ret = malloc((1+num_colors+h)*sizeof(char*));
    }
    ret[0] = malloc((sz+1)*sizeof(char)); if(ret[0] == NULL){ free(ret); return NULL; }
    strcpy(ret[0], line_buf);

    for(int i = 1; i < 1+num_colors+h; ++i){
        sz = getline(&line_buf, &len, f);
        ret[i] = malloc((sz+1)*sizeof(char));
        if(ret[i] == NULL){
            for(int j = 0; j < i; ++j)
                free(ret[i]);
            free(ret);
            return NULL;
        }
        strcpy(ret[i], line_buf);
        ret[i][sz-1] = '\0';
    }
    fclose(f); f = NULL;
    return ret;
}
