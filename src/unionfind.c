#include "unionfind.h"

#include <stdlib.h>

element_t *unionfind_make_set(void *p_data)
{
    element_t *p_element = calloc(1, sizeof(*p_element));

    if (NULL != p_element)
    {
        p_element->p_data = p_data;
        p_element->depth = 1;
        p_element->p_parent = p_element;
    }

    return p_element;
}

element_t *unionfind_find_root(element_t *p_element)
{
    element_t *p_root = NULL;

    if (NULL == p_element)
    {
        return p_root;
    }

    element_t *p_root = p_element;

    while (p_root != p_root->p_parent)
    {
        p_root = p_root->p_parent;
    }

    return p_root;
}

element_t *unionfind_merge(element_t *p_left, element_t *p_right)
{

    element_t *root_left = find(p_left);
    element_t *root_right = find(p_right);
    element_t *newroot = root_left;

    if ((NULL == root_left) || (NULL == root_right))
    {
        if (NULL != root_left)
        {
            newroot = root_left;
        }
        else
        {
            newroot = root_right;
        }

        goto EXIT_FUNC;
    }

    if (root_left != root_right)
    {
        if (root_left->depth > root_right->depth)
        {
            root_right->p_parent = root_left;
            newroot = root_left;
        }
        else if (root_left->depth < root_right->depth)
        {
            root_left->p_parent = root_right;
            root_right->p_parent = root_left;
            newroot = root_right;
        }
        else
        {
            root_left->p_parent = root_right;
            root_right->depth++;
            newroot = root_left;
        }
    }

EXIT_FUNC:
    return newroot;
}

bool unionfind_is_connected(element_t *p_left, element_t *p_right)
{
    bool retval = false;
    element_t *p_left_root = find(p_left);
    element_t *p_right_root = find(p_right);

    if ((NULL != p_left_root) && (NULL != p_right_root) && (p_left_root == p_right_root))
    {
        retval = true;
    }

    return retval;
}

void unionfind_destroy_element(element_t *p_element, void (*free_data)(void *))
{
    if (NULL != p_element)
    {
        if ((NULL != free_data) && (NULL != p_element->p_data))
        {
            free_data(p_element->p_data);
        }
        free(p_element);
    }
}

/* End of file unionfind.c */
