#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

task_t *task_create(int id, const char *name, int priority)
{
    task_t *t = malloc(sizeof(task_t));
    if (!t)
        return NULL;

    t->id = id;
    t->priority = priority;
    strncpy(t->name, name, sizeof(t->name) - 1);
    t->name[63] = '\0';

    return t;
}

void task_print(void *p_data)
{
    task_t *t = (task_t *)p_data;
    printf("Task[%d]: %s (priority %d)\n", t->id, t->name, t->priority);
}

void task_destroy(void *p_data)
{
    free(p_data);
}

int task_compare(const void *a, const void *b)
{
    const task_t *t = (const task_t *)b; // list element is second arg in your API
    const int *id = (const int *)a;

    return (*id != t->id);
}