#ifndef LLIST_H
#define LLIST_H

/**
 * @brief Opaque doubly-linked list handle.
 *
 * All list state is managed internally. Callers interact exclusively
 * through the llist_* API; do not access or allocate llist_t directly.
 */
typedef struct llist llist_t;

/**
 * @brief Destructor callback for list element data.
 *
 * Called by llist_destroy() on each element's data pointer. Provide
 * free() for simple heap-allocated data, or a custom function when
 * elements own additional resources.
 *
 * @param[in] p_data  Pointer to the element data to destroy.
 */
typedef void (*llist_del_f)(void *);

/**
 * @brief Comparator callback for searching the list.
 *
 * Must return 0 when the two values are considered equal, and any
 * non-zero value otherwise. Compatible with the qsort/bsearch
 * comparator convention.
 *
 * @param[in] p_lhs  First value.
 * @param[in] p_rhs  Second value.
 *
 * @return 0 if equal, non-zero otherwise.
 */
typedef int (*llist_cmp_f)(const void *, const void *);

/**
 * @brief Iterator callback applied to each element during traversal.
 *
 * Receives a pointer to each element's data in list order.
 * Must not modify the list during iteration.
 *
 * @param[in] p_data  Pointer to the current element's data.
 */
typedef void (*llist_iter_f)(void *);

/**
 * @brief Allocate and initialize an empty list. O(1)
 *
 * @return Pointer to a new llist_t on success, or NULL on allocation failure.
 */
llist_t *llist_create(void);

/**
 * @brief Insert data at the head of the list. O(1)
 *
 * @param[in] p_list  List to insert into. Must be non-NULL.
 * @param[in] p_data  Data pointer to store. May be NULL.
 *
 * @return 1 on success, -1 on failure (e.g., NULL list or allocation failure).
 */
int llist_insert_head(llist_t *p_list, void *p_data);

/**
 * @brief Insert data at the tail of the list. O(1)
 *
 * @param[in] p_list  List to insert into. Must be non-NULL.
 * @param[in] p_data  Data pointer to store. May be NULL.
 *
 * @return 1 on success, -1 on failure (e.g., NULL list or allocation failure).
 */
int llist_insert_tail(llist_t *p_list, void *p_data);

/**
 * @brief Insert data at an arbitrary index. O(n)
 *
 * Zero-based indexing from the head. Out-of-bounds positive indices
 * append to the tail. Index 0 and negatives insert before the current
 * head (equivalent to insert_head).
 *
 * @param[in] p_list  List to insert into. Must be non-NULL.
 * @param[in] p_data  Data pointer to store. May be NULL.
 * @param[in] index   Insertion position.
 *
 * @return 1 on success, -1 on failure (e.g., NULL list or allocation failure).
 */
int llist_insert_at(llist_t *p_list, void *p_data, int index);

/**
 * @brief Return the index of the first element matching p_data. O(n)
 *
 * Traverses the list and invokes @p cmp_f(@p p_data, element) for each
 * node, stopping at the first match (return value 0 from @p cmp_f).
 *
 * @param[in] p_list  List to search. Must be non-NULL.
 * @param[in] p_data  Search key passed as the first argument to @p cmp_f.
 * @param[in] cmp_f   Comparator function. Must be non-NULL.
 *
 * @return Zero-based index of the first matching element,
 *         or -1 if not found or an argument is NULL.
 */
int llist_index_of(llist_t *p_list, void *p_data, llist_cmp_f cmp_f);

/**
 * @brief Unlink and return the data at a given index. O(n)
 *
 * Removes the node at position @p index and returns its data pointer
 * to the caller, who is then responsible for managing that memory.
 * Zero-based indexing from the head; negative indices are not defined.
 *
 * @param[in] p_list  List to remove from. Must be non-NULL.
 * @param[in] index   Zero-based position of the node to remove.
 *
 * @return Data pointer from the removed node, or NULL if @p index is
 *         out of bounds or @p p_list is NULL.
 */
void *llist_remove_at(llist_t *p_list, int index);

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
void llist_destroy(llist_t **pp_list, llist_del_f del_func);

/**
 * @brief Return the number of elements in the list. O(1)
 *
 * @param[in] p_list  List to measure. Must be non-NULL.
 *
 * @return Element count as a non-negative integer,
 *         or -1 if @p p_list is NULL.
 */
int llist_size(llist_t *p_list);

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
void llist_iterate(llist_t *p_list, llist_iter_f iter_f);

#endif /* LLIST_H */

/* End of file llist.h */
