#include <lcom/lcf.h>

#include "list.h"

struct list_node{
    list_node_t *p, *n;
    void *val;
};

list_node_t* (list_node_ctor)(list_node_t *p, list_node_t *n, void *val){
    list_node_t *ret = malloc(sizeof(list_node_t));
    if(ret == NULL) return NULL;
    ret->p = p;
    ret->n = n;
    ret->val = val;
    return ret;
}
void         (list_node_dtor)(list_node_t *p){
    free(p);
}
list_node_t* (list_node_next)(const list_node_t *p){ return p->n; }
list_node_t* (list_node_prev)(const list_node_t *p){ return p->p; }
void**       (list_node_val )(list_node_t *p){ return &p->val; }

struct list{
    list_node_t *begin_;
    list_node_t *end_;
    size_t sz;
};

list_t*      (list_ctor)     (void){
    list_t *ret = malloc(sizeof(list_t));
    if(ret == NULL) return NULL;
    ret->sz = 0;
    ret->begin_ = NULL; ret->end_ = NULL;
    ret->begin_ = list_node_ctor(NULL, NULL, NULL);
    ret->end_   = list_node_ctor(NULL, NULL, NULL);
    if(ret->begin_ == NULL || ret->end_ == NULL){
        list_dtor(ret);
        return NULL;
    }
    ret->begin_->n = ret->end_  ;
    ret->end_  ->p = ret->begin_;
    return ret;
}
int          (list_dtor)     (list_t *l){
    if(l == NULL) return 0;
    if(list_size(l) > 0) return 1;
    list_node_dtor(l->begin_);
    list_node_dtor(l->end_);
    free(l);
    return 0;
}
list_node_t* (list_begin)    (list_t *l){ return l->begin_->n; }
list_node_t* (list_end)      (list_t *l){ return l->end_; }
size_t       (list_size)     (const list_t *l){ return l->sz; }
int          (list_empty)    (const list_t *l){ return (l->sz ? false : true); }
list_node_t* (list_insert)   (list_t *l, list_node_t *position, void *val){
    list_node_t *node = list_node_ctor(position->p, position, val);
    position->p->n = node;
    position->p    = node;
    ++l->sz;
    return node;
}
void*        (list_erase)    (list_t *l, list_node_t *position){
    position->p->n = position->n;
    position->n->p = position->p;
    void *ret = position->val;
    list_node_dtor(position);
    --l->sz;
    return ret;
}
void         (list_push_back)(list_t *l, void *val){
    list_insert(l, list_end(l), val);
}
void**       (list_front)    (list_t *l){
    return list_node_val(list_begin(l));
}
void         (list_pop_front)(list_t *l){
    list_erase(l, list_begin(l));
}
list_node_t* (list_find)     (list_t *l, void *val){
    list_node_t *it = list_begin(l);
    while(it != list_end(l) && *list_node_val(it) != val){
        it = list_node_next(it);
    }
    return it;
}
