#include <lcom/lcf.h>

#include "queue.h"

#include "list.h"
#include "errors.h"

const size_t queue_max_size = UINT_MAX;

struct queue{
    list_t *l;
};

queue_t* (queue_ctor )(void){
    queue_t *ret = malloc(sizeof(queue_t));
    if(ret == NULL) return NULL;
    ret->l = list_ctor();
    if(ret->l == NULL){
        queue_dtor(ret);
        return NULL;
    }
    return ret;
}
int      (queue_dtor )(queue_t *q){
    int r;
    if((r = list_dtor(q->l))) return r;
    free(q);
    return SUCCESS;
}
size_t   (queue_size )(const queue_t *q           ){ return list_size (q->l); }
int      (queue_empty)(const queue_t *q           ){ return list_empty(q->l); }
void     (queue_push )(      queue_t *q, void *val){ list_push_back(q->l, val); }
void*    (queue_top  )(const queue_t *q           ){ return *list_front(q->l); }
void     (queue_pop  )(      queue_t *q           ){ list_pop_front(q->l); }
