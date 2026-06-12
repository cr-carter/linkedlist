/**
 * @file bst.c
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

#include "bst.h"

#include <stdbool.h>
#include <stdlib.h>

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

static node_t *static_create_node(void *p_data, node_t *p_parent);
static node_t *static_create_node(void *p_data, node_t *p_parent);

tree_t *bst_create_tree(compare_fn compare, destroy_fn destroy)
{
    tree_t *retval = NULL;

    if (NULL != compare)
    {
        retval = calloc(1, sizeof(*retval));

        if (NULL != retval)
        {
            retval->compare = compare;
            retval->destroy = destroy;
        }
    }

    return retval;
}

bool bst_add_node(tree_t *p_tree, void *p_data)
{
    bool retval = false;

    if ((NULL == p_tree) || (NULL == p_data))
    {
        goto EXIT_FUNC;
    }

    node_t *new_node = NULL;

    // Case for empty tree
    if (NULL == p_tree->p_root)
    {
        new_node = static_create_node(p_data, NULL);

        if (NULL != new_node)
        {
            p_tree->size++;
            retval = true;
            goto EXIT_FUNC;
        }
    }

    // Case for non-empty tree
    new_node = static_insert_data(p_tree->compare, p_tree->p_root, p_data);

    if (NULL != new_node)
    {
        retval = true;
        p_tree->size++;
    }

EXIT_FUNC:
    return retval;
}

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

static node_t *static_create_node(void *p_data, node_t *p_parent)
{
    node_t *retval = NULL;

    if (NULL != p_data)
    {
        retval = calloc(1, sizeof(*retval));

        if (NULL != retval)
        {
            retval->p_data = p_data;
            retval->p_parent = p_parent;
        }
    }

    return retval;
}

static node_t *static_insert_data(compare_fn compare, node_t *p_current, void *p_data)
{
    node_t *retval = NULL;

    // Case for data already in tree
    if (0 == compare(p_current->p_data, p_data))
    {
        goto EXIT_FUNC;
    }

    // Case for data less than current node (insert to left)
    if (0 > compare(p_current->p_data, p_data))
    {
        if (NULL == p_current->p_left)
        {
            retval = static_create_node(p_data, p_current);
            p_current->p_left = retval;
            goto EXIT_FUNC;
        }
        else
        {
            static_insert_data(compare, p_current->p_left, p_data);
        }
    }

    // Case for data greater than current node (insert to right)
    if (0 < compare(p_current->p_data, p_data))
    {
        if (NULL == p_current->p_right)
        {
            retval = static_create_node(p_data, p_current);
            p_current->p_right = retval;
            goto EXIT_FUNC;
        }
        else
        {
            static_insert_data(compare, p_current->p_right, p_data);
        }
    }

EXIT_FUNC:
    return retval;
}

/* End of file bst.c */
