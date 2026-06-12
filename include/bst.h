/**
 * @file bst.h
 * @author Chase Carter
 * @brief Binary Search Tree Abstract Data Type.
 *
 * This module provides an implementation of a binary search tree (BST)
 * that stores generic user data through void pointers. Users interact
 * with the tree through the public interface functions and do not have
 * direct access to the internal node structure.
 *
 * The tree maintains data in sorted order according to a user-supplied
 * comparison function.
 */

#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct tree tree_t;
typedef struct node node_t;

struct tree
{
    node_t *p_root;
    size_t size;
    compare_fn compare;
    destroy_fn destroy;
};

struct node
{
    node_t *p_parent;
    node_t *p_left;
    node_t *p_right;
    void *p_data;
};

typedef int (*compare_fn)(const void *, const void *);
typedef void (*destroy_fn)(void *);
typedef void (*print_fn)(const void *);

/**
 * @brief Creates an empty binary search tree.
 *
 * Allocates memory for a new tree object and stores the user-supplied
 * comparison function that will be used to maintain BST ordering.
 *
 * The comparison function must return:
 * - A negative value if the first argument is less than the second.
 * - Zero if the values are equal.
 * - A positive value if the first argument is greater than the second.
 *
 * @param compare Pointer to the comparison function. Must not be NULL.
 * @param destroy Function used to free stored data. May be NULL if
 * the tree should not free user data.
 *
 * @return Pointer to the newly created tree, or NULL if allocation fails.
 *
 * @note The tree will own and destroy data if @p destroy is provided. If NULL, then the caller continues to own data
 * stored in tree and must free data to prevent memory leaks.
 */
tree_t *bst_create_tree(compare_fn compare, destroy_fn destroy);

/**
 * @brief Inserts data into the binary search tree.
 *
 * Creates a new node containing the specified data and inserts it into
 * the appropriate location according to BST ordering rules.
 *
 * @param p_tree Pointer to the tree.
 * @param p_data Pointer to user data to insert. The tree will own and free @p p_data
 *
 * @return true if insertion succeeds, false otherwise.
 *
 * @note After successful insertion, ownership of @p p_data is transferred
 * to the BST. If a destroy function was supplied when the tree was created,
 * the BST will use it to destroy stored data when nodes are removed or the
 * tree is destroyed.
 */
bool bst_add_node(tree_t *p_tree, void *p_data);

/**
 * @brief Searches the tree for data matching the specified key.
 *
 * Traverses the tree using the tree's comparison function to locate
 * data that matches the provided search key.
 *
 * @param p_tree Pointer to the tree.
 * @param p_key Pointer to the key used for comparison.
 *
 * @return Pointer to the matching stored data, or NULL if no matching
 * data exists in the tree.
 */
void *bst_find(tree_t *p_tree, void *p_key);

/**
 * @brief Retrieves the data stored in the root node.
 *
 * Returns the user data associated with the root node of the tree.
 *
 * @param p_tree Pointer to the tree.
 *
 * @return Pointer to the root node's data, or NULL if the tree is empty.
 */
void *bst_get_root(tree_t *p_tree);

/**
 * @brief Retrieves the minimum value stored in the tree.
 *
 * @param p_tree Pointer to the tree.
 *
 * @return Pointer to the minimum data value, or NULL if the tree is empty.
 */
void *bst_minimum_value(tree_t *p_tree);

/**
 * @brief Retrieves the maximum value stored in the tree.
 *
 * @param p_tree Pointer to the tree.
 *
 * @return Pointer to the maximum data value, or NULL if the tree is empty.
 */
void *bst_maximum_value(tree_t *p_tree);

/**
 * @brief Returns the number of nodes currently stored in the tree.
 *
 * @param p_tree Pointer to the tree.
 *
 * @return Number of nodes in the tree.
 */
size_t bst_size_of_tree(tree_t *p_tree);

/**
 * @brief Returns the height of the tree.
 *
 * The height of an empty tree is 0. The height of a tree containing
 * only a root node is 1.
 *
 * @param p_tree Pointer to the tree.
 *
 * @return Height of the tree.
 */
size_t bst_height(tree_t *p_tree);

/**
 * @brief Determines whether the tree is empty.
 *
 * A tree is considered empty when it contains no nodes.
 *
 * @param p_tree Pointer to the tree.
 *
 * @return true if the tree contains no nodes, false otherwise.
 */
bool bst_is_empty(tree_t *p_tree);

/**
 * @brief Prints the contents of the tree in sorted order.
 *
 * Traverses the tree in-order and calls the supplied print function
 * for each stored data item.
 *
 * @param p_tree Pointer to the tree.
 * @param print Function used to display stored data.
 */
void bst_print(tree_t *p_tree, print_fn print);

/**
 * @brief Removes a value from the tree.
 *
 * Searches for the specified value and removes the corresponding node
 * while maintaining BST ordering properties.
 *
 * @param p_tree Pointer to the tree.
 * @param p_key Pointer to the key used for comparison.
 *
 * @return true if the node was removed, false if the value was not found.
 *
 * @note If the value is found, the tree's destroy function is called on
 * the stored data before the node is removed.
 */
bool bst_delete_node(tree_t *p_tree, void *p_key);

/**
 * @brief Removes all nodes from the tree.
 *
 * Frees all nodes currently stored in the tree and resets the tree
 * to an empty state. The tree object itself remains allocated and
 * may be reused for future insertions.
 *
 * The user-supplied destroy function is called on each stored data
 * item before its node is freed.
 *
 * @param p_tree Pointer to the tree to clear.
 */
void bst_clear(tree_t *p_tree);

/**
 * @brief Deletes an entire tree and releases all associated memory.
 *
 * Calls the user-supplied destroy function on every stored data item,
 * frees all nodes in the tree, then frees the tree object itself.
 * The caller's pointer is set to NULL after deletion.
 *
 * @param pp_tree Address of the tree pointer to destroy.
 */
void bst_delete_tree(tree_t **pp_tree);

#endif // BST_H

/* End of file bst.h */
