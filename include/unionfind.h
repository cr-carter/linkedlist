/**
 * @file unionfind.h
 * @author Chase Carter
 * @brief Union-Find (disjoint-set) Abstract Data Type.
 *
 * This module provides an implementation of a Union-Find data structure
 * using a tree-based disjoint-set approach. It tracks a set of elements
 * partitioned into a number of disjoint (non-overlapping) subsets.
 *
 * The implementation utilizes path compression and union-by-rank heuristics
 * to optimize the time complexity of operations to nearly linear time.
 */

#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <stdbool.h>

/**
 * Structure representing an individual element in the disjoint-set.
 */
typedef struct element
{
    struct element *p_parent;
    void *p_data;
    int depth;
} element_t;

/**
 * @brief Creates a new singleton set containing the specified user data.
 *
 * Allocates memory for a new element, sets its parent pointer to itself,
 * and initializes its depth/rank to 1.
 *
 * @param p_data Pointer to generic user data to store in the element. May be NULL.
 *
 * @return Pointer to the newly created element, or NULL if allocation fails.
 */
element_t *unionfind_make_set(void *p_data);

/**
 * @brief Finds the representative (root) element of the set containing p_element.
 *
 * Traverses up the tree structure to locate the root element. This function
 * performs path compression, flattening the tree structure to ensure future
 * operations run in O(n) time.
 *
 * @param p_element Pointer to the element whose representative is being searched.
 *
 * @return Pointer to the representative root element of the set, or NULL if @p p_element is NULL.
 */
element_t *unionfind_find_root(element_t *p_element);

/**
 * @brief Merges the sets containing the two specified elements.
 *
 * Uses the union-by-rank heuristic to attach the shallower tree under the
 * root of the deeper tree, minimizing the overall height of the resulting structure.
 *
 * @param p_left Pointer to an element in the first set.
 * @param p_right Pointer to an element in the second set.
 *
 * @return Pointer to the root element of the newly joined set.
 */
element_t *unionfind_merge(element_t *p_left, element_t *p_right);

/**
 * @brief Determines whether two elements belong to the same disjoint set.
 *
 * Checks if both elements share the same representative root node.
 *
 * @param p_left Pointer to the first element.
 * @param p_right Pointer to the second element.
 *
 * @return true if both elements are in the same set, false otherwise.
 */
bool unionfind_is_connected(element_t *p_left, element_t *p_right);

/**
 * @brief Deletes a single disjoint-set element and releases its allocated memory.
 *
 * Frees the specified element. If a custom data destruction function is supplied,
 * it will be invoked on the underlying user data pointer prior to freeing the node.
 *
 * @param p_element Pointer to the element to destroy.
 * @param free_data Function used to free the underlying user data. May be NULL if
 * the structure should not attempt to free user data.
 *
 * @note Destroying a single element while it is actively participating in a tree
 * can break the disjoint-set structure. This should ideally be used for complete
 * cleanup or on completely isolated elements.
 */
void unionfind_destroy_element(element_t *p_element, void (*free_data)(void *));

#endif // UNIONFIND_H

/* End of file unionfind.h */
