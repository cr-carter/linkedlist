#include "llist.h"

#include <stdlib.h>

typedef struct node
{
    struct node *p_next;
    struct node *p_previous;
    void *p_data;
    void *p_key;
} node_t;

struct llist
{
    node_t *p_head;
    node_t *p_tail;
    int size;
};

static node_t *static_create_node(void *p_data);

static void node_link_head(llist_t *p_list, node_t *p_new);

static void node_link_tail(llist_t *p_list, node_t *p_new);

llist_t *llist_create(void)
{
    llist_t *retval = calloc(1, sizeof(llist_t));

    return retval;
}

int llist_insert_head(llist_t *p_list, void *p_data)
{
    int retval = -1;

    if ((NULL != p_list))
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

int llist_insert_tail(llist_t *p_list, void *p_data)
{
    int retval = -1;

    if ((NULL != p_list))
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

int llist_insert_at(llist_t *p_list, void *p_data, int index)
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
                if (NULL == p_position)
                {
                    break;
                }
                p_position = p_position->p_next;
            }

            p_new_insert->p_next = p_position->p_next;
            p_new_insert->p_previous = p_position;
            p_position->p_next = p_new_insert;
            p_new_insert->p_next->p_previous = p_new_insert;
        }
        p_list->size += 1;
        retval = 1;
    }

EXIT_FUNC:
    return retval;
}

int llist_index_of(llist_t *p_list, void *p_data, llist_cmp_f cmp_f)
{
    int retval = -1;

    if ((NULL != p_list) && (NULL != p_data) && (NULL != cmp_f))
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

void *llist_remove_at(llist_t *p_list, int index)
{
    void *retval = NULL;
    node_t *p_remove = NULL;

    if ((NULL != p_list) && (0 < p_list->size) && (0 <= index) && (index < p_list->size))
    {
        if (0 == index)
        {
            if (p_list->p_head == p_list->p_tail)
            {
                p_list->p_tail = NULL;
            }

            retval = p_list->p_head->p_data;
            p_remove = p_list->p_head;
            p_list->p_head = p_remove->p_next;

            if (NULL != p_list->p_head)
                p_list->p_head->p_previous = NULL;
            else
                p_list->p_tail = NULL;
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
                p_remove->p_next->p_previous = p_position;
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

void llist_destroy(llist_t **pp_list, llist_del_f del_func)
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

int llist_size(llist_t *p_list)
{
    int retval = -1;

    if (NULL != p_list)
    {
        retval = p_list->size;
    }

    return retval;
}

void llist_iterate(llist_t *p_list, llist_iter_f iter_f)
{
    if ((NULL != p_list) && (NULL != iter_f))
    {
        for (node_t *p_temp = p_list->p_head; NULL != p_temp; p_temp = p_temp->p_next)
        {
            iter_f(p_temp->p_data);
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

static void node_link_head(llist_t *p_list, node_t *p_new)
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
            p_list->p_head->p_previous = p_new;
            p_list->p_head = p_new;
        }
    }
}

static void node_link_tail(llist_t *p_list, node_t *p_new)
{

    if (NULL != p_list->p_tail)
    {
        p_new->p_previous = p_list->p_tail;
        p_list->p_tail->p_next = p_new;
    }
    else
    {
        p_list->p_head = p_new;
    }

    p_list->p_tail = p_new;
}

/* End of file llist.c */
