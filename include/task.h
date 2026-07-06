#ifndef TASK_H
#define TASK_H

typedef struct task
{
    int id;
    char name[64];
    int priority;
} task_t;

task_t *task_create(int task_id, const char *name, int priority);
void task_print(void *p_data);
void task_destroy(void *p_data);
int task_compare(const void *val_a, const void *val_b);

#endif