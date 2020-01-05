#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

/**
 * @defgroup    list_t    list_t
 * @ingroup classes
 * @brief List module
 *
 * Can be used like a C++ std::list.
 * A list_t is a sequence of list_node_t nodes, that store void* to allow for more flexibility.
 *
 * @{
 */

/**
 * @defgroup    list_node_t   list_node_t
 * @ingroup list
 * @brief List node module
 *
 * Can be used like a C++ std::list iterator.
 * A list node stores a void* to allow for more flexibility.
 *
 * @{
 */

struct list_node;
typedef struct list_node list_node_t;

/**
 * @brief Construct list node.
 * @param   p   Pointer to previous list node
 * @param   n   Pointer to next list node
 * @param   val Value to be stored in the list node
 * @return      Pointer to created list node
 */
list_node_t* (list_node_ctor)(list_node_t *p, list_node_t *n, void *val);
/**
 * @brief Destruct list node.
 * @param   p   Pointer to list node to be destroyed
 */
void         (list_node_dtor)(list_node_t *p);
/**
 * @brief Get next node.
 * @param   p   Pointer to list node
 * @return      Pointer to next list node
 */
list_node_t* (list_node_next)(const list_node_t *p);
/**
 * @brief Get previous node.
 * @param   p   Pointer to list node
 * @return      Pointer to previous list node
 */
list_node_t* (list_node_prev)(const list_node_t *p);
/**
 * @brief Get value stored in the node.
 * @param   p   Pointer to list node
 * @return      Pointer to the stored value (which is in turn a void*).
 */
void**       (list_node_val )(list_node_t *p);

/**
 * @}
 */

struct list;

typedef struct list list_t;

/**
 * @brief Construct empty list.
 * @return  Pointer to created list
 */
list_t*      (list_ctor)     (void);
/**
 * @brief Destruct list.
 *
 * A list can only be destroyed once it is empty.
 * This is because a list stores void*, whose memory most likely need to be free'd.
 * @param   l   Pointer to list to be destroyed
 * @return      SUCCESS if the destruction was successful, other value otherwise.
 */
int          (list_dtor)     (list_t *l);
/**
 * @brief Get pointer to the first node of the list.
 * @param   l   Pointer to list
 * @return      Pointer to first node of the list.
 */
list_node_t* (list_begin)    (list_t *l);
/**
 * @brief Get pointer to the past-the-end node of the list.
 * @param   l   Pointer to list
 * @return      Pointer to past-the-end node of the list
 */
list_node_t* (list_end)      (list_t *l);
/**
 * @brief Get size of the list.
 * @param   l   Pointer to list
 * @return      Size of the list
 */
size_t       (list_size)     (const list_t *l);
/**
 * @brief Know if list is empty or not.
 * @param   l   Pointer to list
 * @return      true if the list is empty (size is zero), false otherwise
 */
int          (list_empty)    (const list_t *l);
/**
 * @brief Insert value in list.
 * @param   l           Pointer to list
 * @param   position    Pointer to node, before which the new value will be inserted
 * @param   val         Value to be inserted before position
 * @return              Pointer to newly-created node
 */
list_node_t* (list_insert)   (list_t *l, list_node_t *position, void *val);
/**
 * @brief Erase value in list.
 * @param   l           Pointer to list
 * @param   position    Node to be deleted
 * @return  Value contained in the deleted node, which must be free'd if appropriate
 */
void*        (list_erase)    (list_t *l, list_node_t *position);
/**
 * @brief Insert new value at the end of the list.
 * @param   l   Pointer to list
 * @param   val Value to be inserted
 */
void         (list_push_back)(list_t *l, void *val);
/**
 * @brief Get first element of the list.
 * @param   l   Pointer to list
 * @return      Pointer to value at the beginning of the list
 */
void**       (list_front)    (list_t *l);
/**
 * @brief Erase first element of the list.
 * @param   l   Pointer to list
 */
void         (list_pop_front)(list_t *l);
/**
 * @brief Find value in list.
 *
 * Values are found by direct comparison of void*
 * @param   l   Pointer to list
 * @param   val Pointer to value to be found in the list
 * @return  Node whose value compares equal to val, or past-the-end node if not found
 */
list_node_t* (list_find)     (list_t *l, void *val);

/**
 * @}
 */

#endif //LIST_H_INCLUDED
