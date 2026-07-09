#include <stdlib.h>

typedef struct pqueue pqueue_t;

typedef void(p_delete_f)(void *p_data);

pqueue_t *pqueue_create();

void pqueue_destroy(pqueue_t **p_priorq, p_delete_f p_delete);

int pqueue_insert(pqueue_t *p_priorq, size_t priority, void *p_data);

void *pqueue_remove(pqueue_t *p_priorq, size_t *priority);

void *pqueue_peek(pqueue_t *p_priorq, size_t *priority);

/* End of file pqueue.h */
