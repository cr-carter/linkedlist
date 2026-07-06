#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

task_t *task_create(int task_id, const char *name, int priority)
{
    task_t *p_t = malloc(sizeof(task_t));
    if (!p_t)
        return NULL;

    p_t->id = task_id;
    p_t->priority = priority;
    strncpy(p_t->name, name, sizeof(p_t->name) - 1);
    p_t->name[63] = '\0';

    return p_t;
}

void task_print(void *p_data)
{
    task_t *p_t = (task_t *)p_data;
    printf("Task[%d]: %s (priority %d)\n", p_t->id, p_t->name, p_t->priority);
}

void task_destroy(void *p_data)
{
    free(p_data);
}

int task_compare(const void *val_a, const void *val_b)
{
    const task_t *p_t = (const task_t *)val_b; // list element is second arg in your API
    const int *val_id = (const int *)val_a;

    return (*val_id != p_t->id);
}