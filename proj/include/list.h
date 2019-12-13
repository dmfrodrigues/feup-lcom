#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

struct list_node;
typedef struct list_node list_node_t;

list_node_t* (list_node_ctor)(list_node_t *p, list_node_t *n, void *val);
void         (list_node_dtor)(list_node_t *p);
list_node_t* (list_node_next)(const list_node_t *p);
list_node_t* (list_node_prev)(const list_node_t *p);
void**       (list_node_val )(list_node_t *p);

struct list;
typedef struct list list_t;

list_t* (list_ctor)(void);
int     (list_dtor)(list_t *l);
list_node_t* (list_begin )(list_t *l);
list_node_t* (list_end   )(list_t *l);
size_t       (list_size  )(const list_t *l);
list_node_t* (list_insert)(list_t *l, list_node_t *position, void *val);
void*        (list_erase )(list_t *l, list_node_t *position);

#endif //LIST_H_INCLUDED
