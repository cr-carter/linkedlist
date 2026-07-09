#include "pqueue.h"

#include <stdlib.h>
#include <string.h>

#define INIT_SIZE 15
#define LEFT_CHILD(pos) (2 * pos) + 1
#define RIGHT_CHILD(pos) (2 * pos) + 2
#define PARENT(pos) (((pos) - 1) / 2)

typedef struct node
{
    size_t priority;
    void *data;
} node_t;

struct pqueue
{
    node_t *p_nodes;
    size_t nmemb;
    size_t capacity;
};

static void static_heapify_up(pqueue_t *p_pq, size_t index)
{
    if (0 == index)
    {
        return;
    }

    size_t parent_index = PARENT(index);
    node_t *child = &p_pq->p_nodes[index];
    node_t *parent = &p_pq->p_nodes[parent_index];

    if (child->priority > parent->priority)
    {
        node_t temp = *parent;
        *parent = *child;
        *child = temp;

        static_heapify_up(p_pq, parent_index);
    }
}

static void static_heapify_down(pqueue_t *p_pq, size_t index)
{

    size_t largest = index;
    size_t left = LEFT_CHILD(index);
    size_t right = RIGHT_CHILD(index);

    if ((left < p_pq->nmemb) && (p_pq->p_nodes[left].priority > p_pq->p_nodes[largest].priority))
    {
        largest = left;
    }

    if ((right < p_pq->nmemb) && (p_pq->p_nodes[right].priority > p_pq->p_nodes[largest].priority))
    {
        largest = right;
    }

    if (largest != index)
    {

        node_t temp = p_pq->p_nodes[index];
        p_pq->p_nodes[index] = p_pq->p_nodes[largest];
        p_pq->p_nodes[largest] = temp;

        static_heapify_down(p_pq, largest);
    }
}

static int static_resize(pqueue_t *p_pq)
{
    size_t new_capacity = (p_pq->capacity * 2) + 1;
    node_t *temp = calloc(new_capacity, sizeof(*temp));

    if (NULL == temp)
    {
        return EXIT_FAILURE;
    }

    memcpy(temp, p_pq->p_nodes, p_pq->capacity * sizeof(*p_pq->p_nodes));
    free(p_pq->p_nodes);
    p_pq->p_nodes = temp;
    p_pq->capacity = new_capacity;
    return EXIT_SUCCESS;
}

pqueue_t *pqueue_create()
{
    pqueue_t *result = calloc(1, sizeof(*result));

    if (NULL == result)
    {
        return NULL;
    }

    result->capacity = INIT_SIZE;
    result->p_nodes = calloc(result->capacity, sizeof(*result->p_nodes));

    if (NULL == result->p_nodes)
    {
        free(result);
        return NULL;
    }

    return result;
}

int pqueue_insert(pqueue_t *p_pq, size_t priority, void *p_data)
{
    if (NULL == p_pq)
    {
        return EXIT_FAILURE;
    }

    if (p_pq->nmemb + 1 >= p_pq->capacity)
        static_resize(p_pq);

    p_pq->p_nodes[p_pq->nmemb].priority = priority;
    p_pq->p_nodes[p_pq->nmemb].data = p_data;

    static_heapify_up(p_pq, p_pq->nmemb);

    p_pq->nmemb++;

    return EXIT_SUCCESS;
}

void *pqueue_remove(pqueue_t *p_pq, size_t *priority)
{
    void *retval = NULL;

    if ((NULL == p_pq) || (0 == p_pq->nmemb))
    {
        return NULL;
    }

    if (NULL != priority)
    {
        *priority = p_pq->p_nodes[0].priority;
    }

    retval = p_pq->p_nodes[0].data;
    retval = p_pq->p_nodes[0].data;

    p_pq->nmemb--;

    if (p_pq->nmemb > 0)
    {
        p_pq->p_nodes[0] = p_pq->p_nodes[p_pq->nmemb];
        static_heapify_down(p_pq, 0);
    }

    return retval;
}

void *pqueue_peek(pqueue_t *p_pq, size_t *priority)
{
    void *retval = NULL;
    if ((NULL == p_pq) || (0 == p_pq->nmemb))
    {
        return NULL;
    }

    if (NULL != priority)
    {
        *priority = p_pq->p_nodes[0].priority;
    }

    retval = p_pq->p_nodes[0].data;
    return retval;
}

void pqueue_destroy(pqueue_t **p_pq, p_delete_f p_delete)
{
    if ((NULL == p_pq) || (NULL == *p_pq))
        return;

    if (p_delete != NULL)
    {
        for (size_t index = 0; index < (*p_pq)->nmemb; ++index)
        {
            p_delete((*p_pq)->p_nodes[index].data);
        }
    }

    free((*p_pq)->p_nodes);
    free(*p_pq);
    *p_pq = NULL;
}
