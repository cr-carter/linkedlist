#include "deque.h"

#include <stdlib.h>

typedef struct node
{
    struct node *p_next;
    struct node *p_previous;
    void *p_data;
} node_t;

struct deque
{
    node_t *p_head;
    node_t *p_tail;
    int size;
};

deque_t *deque_create(void)
{
    deque_t *retval = calloc(1, sizeof(*retval));

    return retval;
}

static node_t *static_create_node(void *p_data);

static void static_node_link_head(deque_t *p_q, node_t *p_new);

static void static_node_link_tail(deque_t *p_q, node_t *p_new);

static void *static_remove_head(deque_t *p_q);

int deque_push_front(deque_t *p_q, void *p_data)
{
    int retval = -1;

    if ((NULL != p_q))
    {
        node_t *p_new_head = static_create_node(p_data);
        if (NULL != p_new_head)
        {
            static_node_link_head(p_q, p_new_head);
            p_q->size += 1;
            retval = 1;
        }
    }

    return retval;
}

void *deque_pop_front(deque_t *p_q)
{
    return static_remove_head(p_q);
}

int deque_enqueue_back(deque_t *p_q, void *p_data)
{
    int retval = -1;

    if ((NULL != p_q))
    {
        node_t *p_new_tail = static_create_node(p_data);
        if (NULL != p_new_tail)
        {
            static_node_link_tail(p_q, p_new_tail);
            p_q->size += 1;
            retval = 1;
        }
    }

    return retval;
}

void *deque_dequeue_front(deque_t *p_q)
{
    return static_remove_head(p_q);
}

void *deque_peek(deque_t *p_q)
{
    void *retval = NULL;

    if ((NULL != p_q) && (NULL != p_q->p_head))
    {
        retval = p_q->p_head->p_data;
    }

    return retval;
}

int deque_size(deque_t *p_q)
{
    int retval = -1;

    if (NULL != p_q)
    {
        retval = p_q->size;
    }

    return retval;
}

void deque_destroy(deque_t **pp_q, deque_del_f del_func)
{
    if ((NULL != pp_q) && (NULL != *pp_q))
    {
        node_t *p_current = (*pp_q)->p_head;

        while (NULL != p_current)
        {

            node_t *p_delete = p_current;

            if (NULL != del_func)
            {
                del_func(p_delete->p_data);
            }

            p_current = p_current->p_next;
            free(p_delete);
        }
        free(*pp_q);
        *pp_q = NULL;
    }
}

static node_t *static_create_node(void *p_data)
{
    node_t *p_node = calloc(1, sizeof(*p_node));

    if (NULL != p_node)
    {
        p_node->p_data = p_data;
    }

    return p_node;
}

static void static_node_link_head(deque_t *p_q, node_t *p_new)
{

    if ((NULL != p_q) && (NULL != p_new))
    {
        if (NULL == p_q->p_head)
        {
            p_q->p_tail = p_new;
            p_q->p_head = p_new;
        }
        else
        {
            p_new->p_next = p_q->p_head;
            p_q->p_head->p_previous = p_new;
            p_q->p_head = p_new;
        }
    }
}

static void static_node_link_tail(deque_t *p_q, node_t *p_new)
{

    if (NULL != p_q->p_tail)
    {
        p_new->p_previous = p_q->p_tail;
        p_q->p_tail->p_next = p_new;
    }
    else
    {
        p_q->p_head = p_new;
    }

    p_q->p_tail = p_new;
}

static void *static_remove_head(deque_t *p_q)
{
    void *retval = NULL;
    node_t *p_remove = NULL;

    if ((NULL != p_q) && (NULL != p_q->p_head))
    {
        p_remove = p_q->p_head;
        retval = p_remove->p_data;
        p_q->p_head = p_remove->p_next;

        if (NULL != p_q->p_head)
        {
            p_q->p_head->p_previous = NULL;
        }
        else
        {
            p_q->p_tail = NULL;
        }

        free(p_remove);
        p_q->size -= 1;
    }

    return retval;
}

/* End of file deque.c */
