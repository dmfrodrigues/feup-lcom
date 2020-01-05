#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

/**
 * @defgroup    queue_t queue_t
 * @ingroup classes
 * @brief Queue module
 *
 * Can be used like a C++ std::queue.
 * A queue_t is an interface for a list_t that simulates a FIFO.
 *
 * @{
 */

struct queue;
typedef struct queue queue_t;

/**
 * @brief Construct empty queue.
 * @return  Pointer to created queue
 */
queue_t* (queue_ctor )(void);
/**
 * @brief Destruct queue.
 *
 * A queue can only be destroyed once it is empty.
 * This is because a queue stores void*, whose memory most likely need to be free'd.
 * @param   q   Pointer to queue to be destroyed
 * @return      SUCCESS if the destruction was successful, other value otherwise.
 */
int      (queue_dtor )(queue_t *q);
/**
 * @brief Get size of the queue.
 * @param   q   Pointer to queue
 * @return      Size of the queue
 */
size_t   (queue_size )(const queue_t *q);
/**
 * @brief Know if queue is empty or not.
 * @param   q   Pointer to queue
 * @return      true if the queue is empty (size is zero), false otherwise
 */
int      (queue_empty)(const queue_t *q);
/**
 * @brief Insert new value in the queue.
 * @param   q   Pointer to queue
 * @param   val Value to be inserted
 */
void     (queue_push )(queue_t *q, void *val);
/**
 * @brief Get next element in the queue.
 * @param   q   Pointer to queue
 * @return      Value at the beginning of the queue
 */
void*    (queue_top  )(const queue_t *q);
/**
 * @brief Erase next element of the queue.
 * @param   q   Pointer to queue
 */
void     (queue_pop  )(queue_t *q);

/**
 * @}
 */

#endif //QUEUE_H_INCLUDED
