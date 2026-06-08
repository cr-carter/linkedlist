#include "dll.h"

#include <stdlib.h>

typedef struct node
{
    struct node *p_next;
    struct node *p_previous;
    void *p_data;
} node_t;

struct dll
{
    node_t *p_head;
    node_t *p_tail;
    int size;
};

struct dll_iter
{
    dll_t **pp_dll;
    node_t *p_current;
};

static node_t *static_create_node(void *p_data);

static void node_link_head(dll_t *p_list, node_t *p_new);

static void node_link_tail(dll_t *p_list, node_t *p_new);

dll_t *dll_create(void)
{
    dll_t *retval = calloc(1, sizeof(dll_t));

    return retval;
}

int dll_insert_head(dll_t *p_list, void *p_data)
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

int dll_insert_tail(dll_t *p_list, void *p_data)
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

void *dll_remove_head(dll_t *p_list)
{

    void *retval = NULL;
    node_t *p_remove = NULL;

    if ((NULL != p_list) && (NULL != p_list->p_head))
    {

        retval = p_list->p_head->p_data;
        p_remove = p_list->p_head;
        p_list->p_head = p_remove->p_next;

        if (NULL != p_list->p_head)
            p_list->p_head->p_previous = NULL;
        else
            p_list->p_tail = NULL;
    }

    if (NULL != p_remove)
    {
        free(p_remove);
        p_list->size -= 1;
    }

    return retval;
}

void *dll_remove_tail(dll_t *p_list)
{

    void *retval = NULL;
    node_t *p_remove = NULL;

    if ((NULL != p_list) && (NULL != p_list->p_tail))
    {

        p_remove = p_list->p_tail;
        retval = p_remove->p_data;

        if (p_list->p_head == p_list->p_tail)
        {
            p_list->p_head = NULL;
            p_list->p_tail = NULL;
        }
        else
        {
            p_list->p_tail = p_remove->p_previous;
            p_list->p_tail->p_next = NULL;
        }
    }

    if (NULL != p_remove)
    {
        free(p_remove);
        p_list->size -= 1;
    }

    return retval;
}

void dll_destroy(dll_t **pp_list, dll_del_f del_func)
{
    if ((NULL != pp_list) && (NULL != *pp_list))
    {
        node_t *p_current = (*pp_list)->p_head;

        while (NULL != p_current)
        {
            node_t *p_delete = p_current;
            p_current = p_current->p_next;

            if (NULL != del_func)
            {
                del_func(p_delete->p_data);
            }

            if (NULL != p_delete)
            {
                free(p_delete);
            }
        }
        free(*pp_list);
        *pp_list = NULL;
    }
}

int dll_size(dll_t *p_list)
{
    int retval = -1;

    if (NULL != p_list)
    {
        retval = p_list->size;
    }

    return retval;
}

void dll_iterate(dll_t *p_list, dll_iter_f iter_f)
{
    if ((NULL != p_list) && (NULL != iter_f))
    {
        for (node_t *p_temp = p_list->p_head; NULL != p_temp; p_temp = p_temp->p_next)
        {
            iter_f(p_temp->p_data);
        }
    }
}

static void insert_node_before(dll_t *p_list, node_t *p_position, node_t *p_insert)
{
    // Update head if inserting before current node
    if (NULL == p_position->p_previous)
    {
        p_list->p_head = p_insert;
    }

    // Update tail if inserting tail before another node
    if (p_insert == p_list->p_tail)
    {
        p_list->p_tail = p_insert->p_previous;
    }

    p_insert->p_previous->p_next = p_insert->p_next;

    if (NULL != p_insert->p_next)
    {
        p_insert->p_next->p_previous = p_insert->p_previous;
    }

    p_insert->p_previous = p_position->p_previous;

    p_insert->p_next = p_position;

    if (NULL != p_position->p_previous)
    {
        p_position->p_previous->p_next = p_insert;
    }
    p_position->p_previous = p_insert;
}

void dll_selection_sort(dll_t *p_list, dll_compar_f cmp_func)
{
    if ((NULL != p_list) && (NULL != cmp_func) && (1 < p_list->size))
    {
        int loop_count = 0;
        for (node_t *p_unsorted_beginning = p_list->p_head; NULL != p_unsorted_beginning->p_next;)
        {

            node_t *p_comparator = p_unsorted_beginning;
            node_t *p_iterator = p_unsorted_beginning->p_next;

            for (; NULL != p_iterator; p_iterator = p_iterator->p_next)
            {
                if (0 < cmp_func(p_comparator->p_data, p_iterator->p_data))
                {
                    p_comparator = p_iterator;
                }
            }

            if (0 < cmp_func(p_unsorted_beginning->p_data, p_comparator->p_data))
            {
                insert_node_before(p_list, p_unsorted_beginning, p_comparator);
            }
            else
            {
                p_unsorted_beginning = p_unsorted_beginning->p_next;
                if (NULL == p_unsorted_beginning->p_next)
                {
                    p_list->p_tail = p_unsorted_beginning;
                }
            }
            loop_count += 1;
        }
    }
}

void dll_insertion_sort(dll_t *p_list, dll_compar_f cmp_func)
{
    if ((NULL != p_list) && (NULL != cmp_func) && (1 < p_list->size))
    {
        node_t *p_current = p_list->p_head->p_next;

        while (NULL != p_current)
        {
            // Save next node
            node_t *p_next_to_sort = p_current->p_next;
            // Search backward
            node_t *p_search = p_current->p_previous;
            int b_move = 0;

            while (NULL != p_search)
            {
                // Check if current is less than search
                if (0 > cmp_func(p_current->p_data, p_search->p_data))
                {
                    b_move = 1;

                    if (NULL == p_search->p_previous)
                    {
                        break;
                    }
                    p_search = p_search->p_previous;
                }
                else
                {
                    p_search = p_search->p_next;
                    break;
                }
            }

            if (1 == b_move)
            {
                insert_node_before(p_list, p_search, p_current);
            }

            p_current = p_next_to_sort;
        }
    }
}

dll_iter_t *dll_iter_create(dll_t **pp_list)
{
    dll_iter_t *p_retval = NULL;

    if ((NULL != pp_list) && (NULL != *pp_list))
    {
        p_retval = calloc(1, sizeof(*p_retval));
        p_retval->pp_dll = pp_list;
        p_retval->p_current = (*p_retval->pp_dll)->p_head;
    }

    return p_retval;
}

void *dll_iter_next(dll_iter_t *p_iter)
{
    void *p_retval = NULL;

    if ((NULL != p_iter) && (NULL != *p_iter->pp_dll))
    {
        if ((NULL != p_iter->p_current) && (NULL != p_iter->p_current->p_next))
        {
            p_iter->p_current = p_iter->p_current->p_next;
            p_retval = p_iter->p_current->p_data;
        }
    }

    return p_retval;
}

void *dll_iter_prev(dll_iter_t *p_iter)
{
    void *p_retval = NULL;

    if ((NULL != p_iter) && (NULL != *p_iter->pp_dll))
    {
        if ((NULL != p_iter->p_current) && (NULL != p_iter->p_current->p_previous))
        {
            p_iter->p_current = p_iter->p_current->p_previous;
            p_retval = p_iter->p_current->p_data;
        }
    }

    return p_retval;
}

void *dll_iter_data(dll_iter_t *p_iter)
{
    void *p_retval = NULL;

    if ((NULL != p_iter) && (NULL != *p_iter->pp_dll) && (NULL != p_iter->p_current))
    {
        p_retval = p_iter->p_current->p_data;
    }

    return p_retval;
}

void dll_iter_destroy(dll_iter_t **pp_iter)
{
    if ((NULL != pp_iter) && (NULL != *pp_iter))
    {
        free((*pp_iter));
        *pp_iter = NULL;
    }
}

int dll_iter_reset(dll_iter_t *p_iter)
{
    int retval = -1;

    if ((NULL != p_iter) && (NULL != (*p_iter->pp_dll)) && (NULL != (*p_iter->pp_dll)->p_head))
    {
        p_iter->p_current = (*p_iter->pp_dll)->p_head;
        retval = 1;
    }

    return retval;
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

static void node_link_head(dll_t *p_list, node_t *p_new)
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

static void node_link_tail(dll_t *p_list, node_t *p_new)
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

/* End of file dll.c */
