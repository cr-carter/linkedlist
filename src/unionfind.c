#include "unionfind.h"

#include <stdlib.h>

element_t *make_set(void *p_data)
{
    element_t *p_element = calloc(1, sizeof(*p_element));

    if (NULL != p_element)
    {
        p_element->p_data = p_data;
        p_element->depth = 1;
        p_element->p_parent = p_element;
    }
}

element_t *find(element_t *p_element)
{
    element_t *p_root = NULL;

    if (NULL == p_element)
    {
        return p_root;
    }

    element_t *p_find = p_element;

    while (p_find != p_find->p_parent)
    {
        p_root = p_find;
        p_find = p_find->p_parent;
    }

    if ((p_find != p_element->p_parent) && (p_find != p_element))
    {
        p_element->p_parent = p_find;
    }

    return p_root;
}

element_t *merge(element_t *p_left, element_t *p_right)
{

    element_t *root_left = find(p_left);
    element_t *root_right = find(p_right);
    element_t *newroot = root_left;

    if (root_left != root_right)
    {
        if (root_left->depth > root_right->depth)
        {
            root_right->p_parent = root_left;
            root_left->depth++;
        }
        else
        {
            root_left->p_parent = root_right;
            root_right->depth++;
            newroot = root_right;
        }
    }

    return newroot;
}

/* End of file unionfind.c */
