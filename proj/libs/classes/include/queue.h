#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

struct queue;
typedef struct queue queue_t;

const size_t queue_max_size;

queue_t* (queue_ctor )(void);
int      (queue_dtor )(queue_t *q);
size_t   (queue_size )(const queue_t *q);
int      (queue_empty)(const queue_t *q);
void     (queue_push )(queue_t *q, void *val);
void*    (queue_top  )(const queue_t *q);
void     (queue_pop  )(queue_t *q);

#endif //QUEUE_H_INCLUDED
