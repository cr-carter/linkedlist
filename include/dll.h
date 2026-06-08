#ifndef DLL_H
#define DLL_H

/**
 * @brief Opaque doubly-linked list handle.
 *
 * All list state is managed internally. Callers interact exclusively
 * through the dll_* API; do not access or allocate dll_t directly.
 */
typedef struct dll dll_t;

/**
 * @brief Opaque iterator handle for traversing a dll_t.
 *
 * Maintains cursor state for forward and backward traversal.
 * An iterator is invalidated if the list is modified during traversal.
 */
typedef struct dll_iter dll_iter_t;

/**
 * @brief Destructor callback for list element data.
 *
 * Called by dll_destroy() on each element's data pointer. Provide
 * free() for simple heap-allocated data, or a custom function when
 * elements own additional resources.
 *
 * @param[in] p_data  Pointer to the element data to destroy.
 */
typedef void (*dll_del_f)(void *);

/**
 * @brief Iterator callback applied to each element during traversal.
 *
 * Receives a pointer to each element's data in list order.
 * Must not modify the list during iteration.
 *
 * @param[in] p_data  Pointer to the current element's data.
 *
 * @note Modifying the list during iteration, including the deletion of the list, will lead to undefined behavior. If
 * the list is to be modified, call dll_iter_reset().
 */
typedef void (*dll_iter_f)(void *);

/**
 * @brief Comparator callback applied to each element during sorting.
 *
 * Receives a pointer to two element's data.
 * Must not modify the list during comparison.
 *
 * @return 0, if the data1 and data2 are equal;
 *         A negative value if data1 is less than data2;
 *         A positive value if data1 is greater than data2.
 *
 * @param[in] p_data1  Pointer to the current element's data.
 * @param[in] p_data2  Pointer to the next element's data.
 *
 * @note Modifying the list during comparison, including the deletion of the list, will lead to undefined behavior.
 *       The comparator function must be able to handle NULL as one of the elements.
 */
typedef int (*dll_compar_f)(const void *, const void *);

/* ------------------------------------------------------------------ */
/* List operations                                                      */
/* ------------------------------------------------------------------ */

/**
 * @brief Allocate and initialize an empty list. O(1)
 *
 * @return Pointer to a new dll_t on success, or NULL on allocation failure.
 */
dll_t *dll_create(void);

/**
 * @brief Insert data at the head of the list. O(1)
 *
 * @param[in] p_list  List to insert into. Must be non-NULL.
 * @param[in] p_data  Data pointer to store. Must be non-NULL.
 *
 * @return 1 on success, -1 on failure (e.g., NULL argument or allocation failure).
 */
int dll_insert_head(dll_t *p_list, void *p_data);

/**
 * @brief Insert data at the tail of the list. O(1)
 *
 * @param[in] p_list  List to insert into. Must be non-NULL.
 * @param[in] p_data  Data pointer to store. Must be non-NULL.
 *
 * @return 1 on success, -1 on failure (e.g., NULL argument or allocation failure).
 */
int dll_insert_tail(dll_t *p_list, void *p_data);

/**
 * @brief Remove and return data from the head of the list. O(1)
 *
 * The caller is responsible for managing the returned data pointer.
 *
 * @param[in] pp_list  List to remove from. Must be non-NULL.
 *
 * @return Data pointer from the removed element, or NULL if the list
 *         is empty or @p pp_list is NULL.
 */
void *dll_remove_head(dll_t *p_list);

/**
 * @brief Remove and return data from the tail of the list. O(1)
 *
 * The caller is responsible for managing the returned data pointer.
 *
 * @param[in] p_list  List to remove from. Must be non-NULL.
 *
 * @return Data pointer from the removed element, or NULL if the list
 *         is empty or @p p_list is NULL.
 */
void *dll_remove_tail(dll_t *p_list);

/**
 * @brief Return the number of elements in the list. O(1)
 *
 * @param[in] p_list  List to measure. Must be non-NULL.
 *
 * @return Element count as a non-negative integer, or -1 if @p p_list is NULL.
 */
int dll_size(dll_t *p_list);

/**
 * @brief Apply a function to every element in list order. O(n)
 *
 * Invokes @p iter_f on each element's data pointer from head to tail.
 * The list must not be modified during iteration. If either argument
 * is NULL the function returns without iterating.
 *
 * @param[in] p_list  List to traverse. Must be non-NULL.
 * @param[in] iter_f  Function to call on each element. Must be non-NULL.
 */
void dll_iterate(dll_t *p_list, dll_iter_f iter_f);

/**
 * @brief Destroy all nodes and free the list handle. O(n)
 *
 * Traverses the list, invoking @p del_func on each element's data
 * pointer if @p del_func is non-NULL. Then frees all nodes and the
 * list handle itself, and sets *pp_list to NULL.
 *
 * Passing a NULL @p del_func is valid when the caller manages element
 * lifetimes independently.
 *
 * @param[in,out] pp_list   Address of the caller's list pointer. Set to
 *                          NULL on return. Must be non-NULL.
 * @param[in]     del_func  Destructor for element data, or NULL to skip.
 */
void dll_destroy(dll_t **pp_list, dll_del_f del_func);

/**
 * @brief Sort the list in place using the insertion sort algorithm. O(n^2)
 *
 * Rearranges the nodes of the list based on the comparison function @p cmp_func.
 * If the list has fewer than two elements, or if either argument is NULL,
 * the function returns immediately without modifying the list.
 *
 * @param[in,out] p_list   List to sort. Must be non-NULL.
 * @param[in]     cmp_func Comparison function used to determine order.
 * Must be non-NULL.
 */
void dll_insertion_sort(dll_t *p_list, dll_compar_f cmp_func);

/**
 * @brief Sort the list in place using the selection sort algorithm. O(n^2)
 *
 * Rearranges the nodes of the list based on the comparison function @p cmp_func.
 * If the list has fewer than two elements, or if either argument is NULL,
 * the function returns immediately without modifying the list.
 *
 * @param[in,out] p_list   List to sort. Must be non-NULL.
 * @param[in]     cmp_func Comparison function used to determine order.
 * Must be non-NULL.
 */
void dll_selection_sort(dll_t *p_list, dll_compar_f cmp_func);

/* ------------------------------------------------------------------ */
/* Iterator operations                                                  */
/* ------------------------------------------------------------------ */

/**
 * @brief Allocate an iterator positioned before the head. O(1)
 *
 * The iterator starts in at the head; call dll_iter_next()
 * to advance to the next element.
 *
 * @param[in] p_list  List to iterate over. Must be non-NULL.
 *
 * @return Pointer to a new dll_iter_t on success, or NULL on failure.
 */
dll_iter_t *dll_iter_create(dll_t **pp_list);

/**
 * @brief Advance the iterator forward and return the data of the new position. O(1)
 *
 * @param[in] p_iter  Iterator to advance. Must be non-NULL.
 *
 * @return Data pointer at the new position, or NULL if the end of
 *         the list has been reached or @p p_iter is NULL.
 *
 * @note Iterator will not move past the tail of the list. If current position is the tail, dll_iter_next() will return
 * NULL.
 */
void *dll_iter_next(dll_iter_t *p_iter);

/**
 * @brief Advance the iterator backward and return the data of the new position. O(1)
 *
 * @param[in] p_iter  Iterator to advance. Must be non-NULL.
 *
 * @return Data pointer at the new position, or NULL if the beginning of
 *         the list has been reached or @p p_iter is NULL.
 *
 * @note Iterator will not move before the head of the list. If current position is the head, dll_iter_next() will
 * return NULL.
 */
void *dll_iter_prev(dll_iter_t *p_iter);

/**
 * @brief Return the data at the iterator's current position. O(1)
 *
 * Does not advance the iterator.
 *
 * @param[in] p_iter  Iterator to read. Must be non-NULL.
 *
 * @return Data pointer at the current position, or NULL if @p p_iter is NULL.
 */
void *dll_iter_data(dll_iter_t *p_iter);

/**
 * @brief Free the iterator handle. O(1)
 *
 * Does not modify the list or its elements.
 *
 * @param[in/out] p_iter  Iterator to free. Must be non-NULL.
 */
void dll_iter_destroy(dll_iter_t **pp_iter);

/**
 * @brief Resets the iterator position back to the head of the list. O(1)
 *
 * @param[in] p_iter  Iterator to reset. Must be non-NULL.
 *
 * @return 1 on success, -1 on failure (e.g., NULL argument).
 */
int dll_iter_reset(dll_iter_t *p_iter);

#endif /* DLL_H */
