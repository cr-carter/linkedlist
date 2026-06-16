/**
 * @file avl.c
 * @author Chase Carter
 * @brief Binary Search Tree Aavlract Data Type.
 *
 * This module provides an implementation of a binary search tree (BST)
 * that stores generic user data through void pointers. Users interact
 * with the tree through the public interface functions and do not have
 * direct access to the internal node structure.
 *
 * The tree maintains data in sorted order according to a user-supplied
 * comparison function.
 */

#include "avl.h"

#include <stdbool.h>
#include <stdio.h>
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
    int height;
    void *p_data;
};

typedef struct trunk trunk_t;

struct trunk
{
    trunk_t *prev;
    char *str;
};

// Static create/insert functions
static node_t *static_create_node(void *p_data, node_t *p_parent);
static node_t *static_insert_data(compare_fn compare, node_t *p_current, void *p_data);

// Static search functions
static node_t *static_find_node(compare_fn compare, node_t *p_current, void *p_key);
static node_t *static_min_node(node_t *p_current);
static node_t *static_max_node(node_t *p_current);

// Static delete functions
static void static_destroy_node(tree_t *p_tree, node_t **pp_delete_me);
static void static_destroy_all_nodes(tree_t *p_tree, node_t **pp_current);
static void static_transplant(tree_t *p_tree, node_t *p_delete, node_t *p_replace);

// Static print functions
static void static_show_trunks(trunk_t *p);
static void print_tree_recursive(node_t *p_node, trunk_t *prev, bool is_left, print_fn print);

// Static AVL functions
static int static_get_height(node_t *p_node);
static void static_update_height(node_t *p_node);
static int static_get_balance(node_t *p_node);
static void static_balance_tree(tree_t *p_tree, node_t *p_node);

// Static traversal functions
static void static_pre_order(node_t *p_current, order_fn order_func);
static void static_post_order(node_t *p_current, order_fn order_func);
static void static_in_order(node_t *p_current, order_fn order_func);

// Static rotate function
static void static_left_rotate(tree_t *p_tree, node_t *p_rotate);
static void static_right_rotate(tree_t *p_tree, node_t *p_rotate);

tree_t *avl_create_tree(compare_fn compare, destroy_fn destroy)
{
    tree_t *p_retval = NULL;

    if (NULL != compare)
    {
        p_retval = calloc(1, sizeof(*p_retval));

        if (NULL != p_retval)
        {
            p_retval->compare = compare;
            p_retval->destroy = destroy;
        }
    }

    return p_retval;
}

bool avl_add_node(tree_t *p_tree, void *p_data)
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
            p_tree->p_root = new_node;
            p_tree->size++;
            retval = true;
        }

        goto EXIT_FUNC;
    }

    // Case for non-empty tree
    new_node = static_insert_data(p_tree->compare, p_tree->p_root, p_data);

    if (NULL != new_node)
    {
        retval = true;
        p_tree->size++;

        static_balance_tree(p_tree, new_node);
    }

EXIT_FUNC:
    return retval;
}

void *avl_find(tree_t *p_tree, void *p_key)
{
    void *p_retval = NULL;

    if ((NULL == p_tree) || (NULL == p_key) || (NULL == p_tree->p_root))
    {
        goto EXIT_FUNC;
    }

    node_t *found_node = static_find_node(p_tree->compare, p_tree->p_root, p_key);

    if (NULL != found_node)
    {
        p_retval = found_node->p_data;
    }

EXIT_FUNC:
    return p_retval;
}

void *avl_get_root(tree_t *p_tree)
{
    void *p_retval = NULL;

    if ((NULL != p_tree) && (NULL != p_tree->p_root))
    {
        p_retval = p_tree->p_root->p_data;
    }

    return p_retval;
}

void *avl_minimum_value(tree_t *p_tree)
{
    void *p_retval = NULL;

    if ((NULL != p_tree) && (NULL != p_tree->p_root))
    {
        node_t *found_node = static_min_node(p_tree->p_root);
        p_retval = found_node->p_data;
    }

    return p_retval;
}

void *avl_maximum_value(tree_t *p_tree)
{
    void *p_retval = NULL;

    if ((NULL != p_tree) && (NULL != p_tree->p_root))
    {
        node_t *found_node = static_max_node(p_tree->p_root);
        p_retval = found_node->p_data;
    }

    return p_retval;
}

size_t avl_size_of_tree(tree_t *p_tree)
{
    size_t retval = 0;

    if (NULL != p_tree)
    {
        retval = p_tree->size;
    }

    return retval;
}

bool avl_is_empty(tree_t *p_tree)
{
    bool retval = false;

    if ((NULL != p_tree) && (0 == p_tree->size))
    {
        retval = true;
    }

    return retval;
}

void avl_print(tree_t *p_tree, print_fn print)
{
    if ((p_tree == NULL) || (p_tree->p_root == NULL) || (print == NULL))
    {
        return;
    }

    print_tree_recursive(p_tree->p_root, NULL, false, print);
}

bool avl_delete_node(tree_t *p_tree, void *p_key)
{
    bool retval = false;

    if ((NULL == p_tree) || (NULL == p_key))
    {
        goto EXIT_FUNC;
    }

    node_t *p_delete_me = static_find_node(p_tree->compare, p_tree->p_root, p_key);

    if (NULL == p_delete_me)
    {
        goto EXIT_FUNC;
    }

    node_t *p_start = NULL;

    if (NULL == p_delete_me->p_left)
    {
        p_start = p_delete_me->p_parent;
        static_transplant(p_tree, p_delete_me, p_delete_me->p_right);
    }
    else if (NULL == p_delete_me->p_right)
    {
        p_start = p_delete_me->p_parent;
        static_transplant(p_tree, p_delete_me, p_delete_me->p_left);
    }
    else
    {
        node_t *p_replace_with = static_max_node(p_delete_me->p_left);

        if (p_replace_with->p_parent == p_delete_me)
        {
            p_start = p_replace_with;
        }
        else
        {
            p_start = p_replace_with->p_parent;
            static_transplant(p_tree, p_replace_with, p_replace_with->p_left);
            p_replace_with->p_left = p_delete_me->p_left;
            p_replace_with->p_left->p_parent = p_replace_with;
        }

        static_transplant(p_tree, p_delete_me, p_replace_with);

        p_replace_with->p_right = p_delete_me->p_right;
        p_replace_with->p_right->p_parent = p_replace_with;
    }

    static_destroy_node(p_tree, &p_delete_me);
    p_tree->size--;
    retval = true;

    if (NULL != p_start)
    {
        static_balance_tree(p_tree, p_start);
    }
    else
    {
        if (NULL != p_tree->p_root)
        {
            static_balance_tree(p_tree, p_tree->p_root);
        }
    }

EXIT_FUNC:
    return retval;
}

void avl_clear(tree_t *p_tree)
{
    if ((NULL != p_tree) && (NULL != p_tree->p_root))
    {
        static_destroy_all_nodes(p_tree, &(p_tree->p_root));
        p_tree->p_root = NULL;
        p_tree->size = 0;
    }
}

void avl_destroy_tree(tree_t **pp_tree)
{
    if ((NULL != pp_tree) && (NULL != *pp_tree))
    {
        avl_clear(*pp_tree);
        free(*pp_tree);
        *pp_tree = NULL;
    }
}

void avl_pre_order(tree_t *p_tree, order_fn order_func)
{
    if ((NULL == p_tree) || (NULL == order_func))
    {
        return;
    }

    static_pre_order(p_tree->p_root, order_func);
}

void avl_post_order(tree_t *p_tree, order_fn order_func)
{
    if ((NULL == p_tree) || (NULL == order_func))
    {
        return;
    }

    static_post_order(p_tree->p_root, order_func);
}

void avl_in_order(tree_t *p_tree, order_fn order_func)
{
    if ((NULL == p_tree) || (NULL == order_func))
    {
        return;
    }

    static_in_order(p_tree->p_root, order_func);
}

static node_t *static_create_node(void *p_data, node_t *p_parent)
{
    node_t *p_retval = NULL;

    if (NULL != p_data)
    {
        p_retval = calloc(1, sizeof(*p_retval));

        if (NULL != p_retval)
        {
            p_retval->p_data = p_data;
            p_retval->p_parent = p_parent;
            p_retval->height = 0;
        }
    }

    return p_retval;
}

static node_t *static_insert_data(compare_fn compare, node_t *p_current, void *p_data)
{
    node_t *p_retval = NULL;

    int comparison = compare(p_current->p_data, p_data);

    // Case for data already in tree
    if (0 == comparison)
    {
        goto EXIT_FUNC;
    }

    // Case for data less than current node (insert to left)
    else if (0 < comparison)
    {
        if (NULL == p_current->p_left)
        {
            p_retval = static_create_node(p_data, p_current);
            p_current->p_left = p_retval;
            goto EXIT_FUNC;
        }
        else
        {
            p_retval = static_insert_data(compare, p_current->p_left, p_data);
            goto EXIT_FUNC;
        }
    }

    // Case for data greater than current node (insert to right)
    else if (0 > comparison)
    {
        if (NULL == p_current->p_right)
        {
            p_retval = static_create_node(p_data, p_current);
            p_current->p_right = p_retval;
            goto EXIT_FUNC;
        }
        else
        {
            p_retval = static_insert_data(compare, p_current->p_right, p_data);
            goto EXIT_FUNC;
        }
    }

EXIT_FUNC:
    return p_retval;
}

static node_t *static_find_node(compare_fn compare, node_t *p_current, void *p_key)
{
    node_t *p_retval = NULL;

    if ((NULL == compare) || (NULL == p_current) || (NULL == p_key))
    {
        goto EXIT_FUNC;
    }

    int comparison = compare(p_current->p_data, p_key);

    // Case for key found in p_current
    if (0 == comparison)
    {
        p_retval = p_current;
        goto EXIT_FUNC;
    }

    // Case for key less than current node (look to left)
    if (0 < comparison)
    {
        p_retval = static_find_node(compare, p_current->p_left, p_key);
    }

    // Case for key greater than current node (look to right)
    if (0 > comparison)
    {
        p_retval = static_find_node(compare, p_current->p_right, p_key);
    }

EXIT_FUNC:
    return p_retval;
}

static node_t *static_min_node(node_t *p_current)
{
    node_t *p_retval = NULL;

    if (NULL != p_current)
    {
        p_retval = p_current;

        while (NULL != p_retval->p_left)
        {
            p_retval = p_retval->p_left;
        }
    }

    return p_retval;
}

static node_t *static_max_node(node_t *p_current)
{
    node_t *p_retval = NULL;

    if (NULL != p_current)
    {
        p_retval = p_current;

        while (NULL != p_retval->p_right)
        {
            p_retval = p_retval->p_right;
        }
    }

    return p_retval;
}

static void static_destroy_node(tree_t *p_tree, node_t **pp_delete_me)
{
    if ((NULL != pp_delete_me) && (NULL != *pp_delete_me) && (NULL != p_tree))
    {
        if (NULL != p_tree->destroy)
        {
            p_tree->destroy((*pp_delete_me)->p_data);
        }
        free(*pp_delete_me);
        *pp_delete_me = NULL;
    }
}

static void static_destroy_all_nodes(tree_t *p_tree, node_t **pp_current)
{
    if ((NULL != pp_current) && (NULL != p_tree) && (NULL != *pp_current))
    {
        static_destroy_all_nodes(p_tree, &(*pp_current)->p_left);
        static_destroy_all_nodes(p_tree, &(*pp_current)->p_right);
        static_destroy_node(p_tree, pp_current);
    }
}

static void static_transplant(tree_t *p_tree, node_t *p_delete, node_t *p_replace)
{
    if (p_delete->p_parent == NULL)
    {
        p_tree->p_root = p_replace;
    }
    else if (p_delete == p_delete->p_parent->p_left)
    {
        p_delete->p_parent->p_left = p_replace;
    }
    else
    {
        p_delete->p_parent->p_right = p_replace;
    }

    if (p_replace != NULL)
    {
        p_replace->p_parent = p_delete->p_parent;
    }
}

static void static_show_trunks(trunk_t *p)
{
    if (p == NULL)
    {
        return;
    }

    static_show_trunks(p->prev);
    printf("%s", p->str);
}

static void print_tree_recursive(node_t *p_node, trunk_t *prev, bool is_left, print_fn print_func)
{
    if (p_node == NULL)
    {
        return;
    }

    const char *prev_str = "    ";

    trunk_t trunk = {.prev = prev, .str = (char *)prev_str};

    /* print right subtree first */
    print_tree_recursive(p_node->p_right, &trunk, true, print_func);

    if (prev == NULL)
    {
        trunk.str = (char *)"───";
    }
    else if (is_left)
    {
        trunk.str = (char *)".───";
        prev_str = (char *)"   |";
    }
    else
    {
        trunk.str = (char *)"`───";
        prev->str = (char *)prev_str;
    }

    static_show_trunks(&trunk);

    print_func(p_node->p_data);
    printf("\n");

    if (prev != NULL)
    {
        prev->str = (char *)prev_str;
    }

    trunk.str = (char *)"   |";

    /* print left subtree */
    print_tree_recursive(p_node->p_left, &trunk, false, print_func);
}

static void static_pre_order(node_t *p_current, order_fn order_func)
{
    if (NULL == p_current)
    {
        return;
    }

    if (NULL != order_func)
    {
        order_func(p_current->p_data);
    }

    static_pre_order(p_current->p_left, order_func);
    static_pre_order(p_current->p_right, order_func);
}

static void static_post_order(node_t *p_current, order_fn order_func)
{
    if (NULL == p_current)
    {
        return;
    }

    static_post_order(p_current->p_left, order_func);
    static_post_order(p_current->p_right, order_func);

    if (NULL != order_func)
    {
        order_func(p_current->p_data);
    }
}

static void static_in_order(node_t *p_current, order_fn order_func)
{
    if (NULL == p_current)
    {
        return;
    }

    static_in_order(p_current->p_left, order_func);

    if (NULL != order_func)
    {
        order_func(p_current->p_data);
    }

    static_in_order(p_current->p_right, order_func);
}

static int static_get_height(node_t *p_node)
{
    int retval = -1;

    if (NULL != p_node)
    {
        retval = p_node->height;
    }

    return retval;
}

static void static_update_height(node_t *p_node)
{
    if (p_node != NULL)
    {
        int left_h = static_get_height(p_node->p_left);
        int right_h = static_get_height(p_node->p_right);

        if (left_h > right_h)
        {
            p_node->height = left_h + 1;
        }
        else
        {
            p_node->height = right_h + 1;
        }
    }
}

static int static_get_balance(node_t *p_node)
{
    int retval = 0;

    if (NULL != p_node)
    {
        retval = static_get_height(p_node->p_left) - static_get_height(p_node->p_right);
    }

    return retval;
}

static void static_balance_tree(tree_t *p_tree, node_t *p_node)
{
    node_t *p_temp = p_node;

    while (NULL != p_temp)
    {
        static_update_height(p_temp);
        int balance = static_get_balance(p_temp);

        // Left Heavy (LL or LR)
        if (balance > 1)
        {
            if (static_get_balance(p_temp->p_left) < 0)
            {
                // LR Case
                static_left_rotate(p_tree, p_temp->p_left);
            }
            // LL Case
            static_right_rotate(p_tree, p_temp);
            p_temp = p_temp->p_parent; // Adjust pointer since step shifted down
        }
        // Right Heavy (RR or RL)
        else if (balance < -1)
        {
            if (static_get_balance(p_temp->p_right) > 0)
            {
                // RL Case
                static_right_rotate(p_tree, p_temp->p_right);
            }
            // RR Case
            static_left_rotate(p_tree, p_temp);
            p_temp = p_temp->p_parent;
        }

        p_temp = p_temp->p_parent;
    }
}

static void static_left_rotate(tree_t *p_tree, node_t *p_rotate)
{
    if ((NULL == p_tree) || (NULL == p_rotate) || (NULL == p_rotate->p_right))
    {
        return;
    }

    node_t *p_right_child = p_rotate->p_right;

    p_rotate->p_right = p_right_child->p_left;

    if (NULL != p_rotate->p_right)
    {
        p_rotate->p_right->p_parent = p_rotate;
    }

    p_right_child->p_parent = p_rotate->p_parent;

    if (NULL == p_rotate->p_parent)
    {
        p_tree->p_root = p_right_child;
    }
    else if (p_rotate == p_rotate->p_parent->p_left)
    {
        p_rotate->p_parent->p_left = p_right_child;
    }
    else
    {
        p_rotate->p_parent->p_right = p_right_child;
    }

    p_right_child->p_left = p_rotate;
    p_rotate->p_parent = p_right_child;

    static_update_height(p_rotate);
    static_update_height(p_right_child);
}

static void static_right_rotate(tree_t *p_tree, node_t *p_rotate)
{
    if ((NULL == p_tree) || (NULL == p_rotate) || (NULL == p_rotate->p_left))
    {
        return;
    }
    node_t *p_left_child = p_rotate->p_left;

    p_rotate->p_left = p_left_child->p_right;

    if (NULL != p_rotate->p_left)
    {
        p_rotate->p_right->p_parent = p_rotate;
    }

    p_left_child->p_parent = p_rotate->p_parent;

    if (NULL == p_rotate->p_parent)
    {
        p_tree->p_root = p_left_child;
    }
    else if (p_rotate == p_rotate->p_parent->p_left)
    {
        p_rotate->p_parent->p_left = p_left_child;
    }
    else
    {
        p_rotate->p_parent->p_right = p_left_child;
    }

    p_left_child->p_right = p_rotate;
    p_rotate->p_parent = p_left_child;

    static_update_height(p_rotate);
    static_update_height(p_left_child);
}

/* End of file avl.c */
