#include <stdlib.h>

struct node
{
    int priority;
    void *data;
};

struct pqueue
{
    struct node *p_nodes;
    size_t nmemb;
    size_t capacity;
};

typedef void(p_delete_f)(void *p_data);

static void heapify_up(struct pqueue *p_priorq, int index);

static void heapify_down(struct pqueue *p_priorq);

static struct pqueue *resize(struct pqueue *p_priorq);

struct pqueue *create();

void destroy(struct pqueue **p_priorq, p_delete_f p_delete);

int insert(struct pqueue *p_priorq, int priority, void *p_data);

void *remove(struct pqueue *p_priorq, int *priority);

void *peek(struct pqueue *p_priorq, int *priority);

// pqueue
// qsort