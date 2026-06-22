#ifndef DEQUE_H
#define DEQUE_H

/**
 * @brief Opaque double-ended queue handle.
 *
 * All deque state is managed internally. Callers interact exclusively
 * through the deque_* API; do not access or allocate deque_t directly.
 *
 * @note Stack interface:
 *       push() / pop()
 *
 *       Queue interface:
 *       enqueue() / dequeue()
 *
 *       Do not mix the interfaces on the same deque unless
 *       you intentionally want custom behavior.
 */
typedef struct deque deque_t;

/**
 * @brief Destructor callback for deque element data.
 *
 * Called by deque_destroy() on each element's data pointer. Provide
 * free() for simple heap-allocated data, or a custom function when
 * elements own additional resources.
 *
 * @param[in] p_data  Pointer to the element data to destroy.
 */
typedef void (*deque_del_f)(void *);

/**
 * @brief Allocate and initialize an empty deque. O(1)
 *
 * @return Pointer to a new deque_t on success, or NULL on allocation failure.
 */
deque_t *deque_create(void);

/**
 * @brief Push data onto the front of the deque. O(1)
 *
 * @param[in] p_q    Deque to push onto. Must be non-NULL.
 * @param[in] p_data Data pointer to store. May be NULL.
 *
 * @return 1 on success, -1 on failure (e.g., NULL argument or allocation failure).
 */
int deque_push_front(deque_t *p_q, void *p_data);

/**
 * @brief Pop data from the front of the deque. O(1)
 *
 * Removes the front element and returns its data pointer to the caller,
 * who is then responsible for managing that memory.
 *
 * @param[in] p_q  Deque to pop from. Must be non-NULL.
 *
 * @return Data pointer from the removed element, or NULL if the deque
 *         is empty or @p p_q is NULL.
 */
void *deque_pop_front(deque_t *p_q);

/**
 * @brief Enqueue data onto the back of the deque. O(1)
 *
 * @param[in] p_q    Deque to enqueue onto. Must be non-NULL.
 * @param[in] p_data Data pointer to store. May be NULL.
 *
 * @return 1 on success, -1 on failure (e.g., NULL argument or allocation failure).
 */
int deque_enqueue_back(deque_t *p_q, void *p_data);

/**
 * @brief Dequeue data from the front of the deque. O(1)
 *
 * Removes the front element and returns its data pointer to the caller,
 * who is then responsible for managing that memory.
 *
 * @param[in] p_q  Deque to dequeue from. Must be non-NULL.
 *
 * @return Data pointer from the removed element, or NULL if the deque
 *         is empty or @p p_q is NULL.
 */
void *deque_dequeue_front(deque_t *p_q);

/**
 * @brief Return the data at the front of the deque without removing it. O(1)
 *
 * @param[in] p_q  Deque to peek at. Must be non-NULL.
 *
 * @return Data pointer of the front element, or NULL if the deque
 *         is empty or @p p_q is NULL.
 */
void *deque_peek(deque_t *p_q);

/**
 * @brief Return the number of elements in the deque. O(1)
 *
 * @param[in] p_q  Deque to measure. Must be non-NULL.
 *
 * @return Element count as a non-negative integer, or -1 if @p p_q is NULL.
 */
int deque_size(deque_t *p_q);

/**
 * @brief Destroy all elements and free the deque handle. O(n)
 *
 * Traverses the deque, invoking @p del_func on each element's data
 * pointer if @p del_func is non-NULL. Then frees all nodes and the
 * deque handle itself, and sets *pp_q to NULL.
 *
 * Passing a NULL @p del_func is valid when the caller manages element
 * lifetimes independently.
 *
 * @param[in,out] pp_q      Address of the caller's deque pointer. Set to
 *                          NULL on return. Must be non-NULL.
 * @param[in]     del_func  Destructor for element data, or NULL to skip.
 */
void deque_destroy(deque_t **pp_q, deque_del_f del_func);

#endif /* DEQUE_H */

/* End of file deque.h */
