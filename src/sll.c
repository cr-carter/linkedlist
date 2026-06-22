#include "sll.h"

#include <stdlib.h>

typedef struct node
{
    struct node *p_next;
    void *p_data;
} node_t;

struct sll
{
    node_t *p_head;
    node_t *p_tail;
    int size;
};

static node_t *static_create_node(void *p_data);

static void static_node_link_head(sll_t *p_list, node_t *p_new);

static void static_node_link_tail(sll_t *p_list, node_t *p_new);

sll_t *sll_create(void)
{
    sll_t *retval = calloc(1, sizeof(sll_t));

    return retval;
}

int sll_insert_head(sll_t *p_list, void *p_data)
{
    int retval = -1;

    if (NULL != p_list)
    {
        node_t *p_new_head = static_create_node(p_data);
        if (NULL != p_new_head)
        {
            node_link_head(p_list, p_new_head);
            p_list->size += 1;
            retval = 1;
        }
    }

    return retval;
}

int sll_insert_tail(sll_t *p_list, void *p_data)
{
    int retval = -1;

    if (NULL != p_list)
    {
        node_t *p_new_tail = static_create_node(p_data);
        if (NULL != p_new_tail)
        {
            node_link_tail(p_list, p_new_tail);
            p_list->size += 1;
            retval = 1;
        }
    }

    return retval;
}

int sll_insert_at(sll_t *p_list, void *p_data, int index)
{
    int retval = -1;

    if (NULL != p_list)
    {
        node_t *p_new_insert = static_create_node(p_data);

        if (NULL == p_new_insert)
        {
            goto EXIT_FUNC;
        }

        if (index <= 0)
        {
            node_link_head(p_list, p_new_insert);
        }
        else if (index >= p_list->size)
        {
            node_link_tail(p_list, p_new_insert);
        }
        else
        {
            node_t *p_position = p_list->p_head;

            for (int pos = 0; pos < index - 1; pos++)
            {
                p_position = p_position->p_next;
            }

            p_new_insert->p_next = p_position->p_next;
            p_position->p_next = p_new_insert;
        }
        p_list->size += 1;
        retval = 1;
    }

EXIT_FUNC:
    return retval;
}

int sll_index_of(sll_t *p_list, void *p_data, sll_cmp_f cmp_f)
{
    int retval = -1;

    if ((NULL != p_list) && (NULL != cmp_f))
    {
        node_t *p_position = p_list->p_head;
        for (int index = 0; NULL != p_position; p_position = p_position->p_next, index++)
        {
            if (0 == cmp_f(p_data, p_position->p_data))
            {
                retval = index;
                break;
            }
        }
    }

    return retval;
}

void *sll_remove_at(sll_t *p_list, int index)
{
    void *retval = NULL;
    node_t *p_remove = NULL;

    if ((NULL != p_list) && (0 < p_list->size) && (0 <= index) && (index < p_list->size))
    {
        if (0 == index)
        {
            retval = p_list->p_head->p_data;
            p_remove = p_list->p_head;
            p_list->p_head = p_remove->p_next;

            if (NULL == p_list->p_head)
            {
                p_list->p_tail = NULL;
            }
        }
        else
        {
            node_t *p_position = p_list->p_head;

            for (int pos = 0; pos < index - 1; pos++)
            {
                p_position = p_position->p_next;
            }

            p_remove = p_position->p_next;
            retval = p_remove->p_data;

            if (p_remove == p_list->p_tail)
            {
                p_list->p_tail = p_position;
                p_position->p_next = NULL;
            }
            else
            {
                p_position->p_next = p_remove->p_next;
            }
        }
    }

    if (NULL != p_remove)
    {
        free(p_remove);
        p_list->size -= 1;
    }

    return retval;
}

void sll_destroy(sll_t **pp_list, sll_del_f del_func)
{
    if ((NULL != pp_list) && (NULL != *pp_list))
    {
        node_t *p_current = (*pp_list)->p_head;

        while (NULL != p_current)
        {

            node_t *p_delete = p_current;

            if (NULL != del_func)
            {
                del_func(p_delete->p_data);
            }

            p_current = p_current->p_next;

            if (NULL != p_delete)
            {
                free(p_delete);
            }
        }
        free(*pp_list);
        *pp_list = NULL;
    }
}

int sll_size(sll_t *p_list)
{
    int retval = -1;

    if (NULL != p_list)
    {
        retval = p_list->size;
    }

    return retval;
}

void sll_iterate(sll_t *p_list, sll_iter_f iter_f)
{
    if ((NULL != p_list) && (NULL != iter_f))
    {
        for (node_t *p_temp = p_list->p_head; NULL != p_temp;)
        {
            node_t *p_next = p_temp->p_next;

            iter_f(p_temp->p_data);

            p_temp = p_next;
        }
    }
}

static node_t *static_create_node(void *p_data)
{
    node_t *p_node = calloc(1, sizeof(*p_node));

    if (NULL != p_node)
    {
        p_node->p_data = p_data;
        p_node->p_next = NULL;
    }

    return p_node;
}

static void static_node_link_head(sll_t *p_list, node_t *p_new)
{

    if ((NULL != p_list) && (NULL != p_new))
    {
        if (NULL == p_list->p_head)
        {
            p_list->p_tail = p_new;
            p_list->p_head = p_new;
        }
        else
        {
            p_new->p_next = p_list->p_head;
            p_list->p_head = p_new;
        }
    }
}

static void static_node_link_tail(sll_t *p_list, node_t *p_new)
{

    if (NULL != p_list->p_tail)
    {
        p_list->p_tail->p_next = p_new;
    }
    else
    {
        p_list->p_head = p_new;
    }

    p_list->p_tail = p_new;
}

/* End of file sll.c */
