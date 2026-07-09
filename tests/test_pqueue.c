#include "pqueue.h"
#include "unity.h"

#include <stdio.h>
#include <stdlib.h>

void test_pqueue_random(void)
{
    char *char_0 = calloc(1, sizeof(*char_0));
    char *char_1 = calloc(1, sizeof(*char_1));
    char *char_2 = calloc(1, sizeof(*char_2));
    char *char_3 = calloc(1, sizeof(*char_3));
    char *char_4 = calloc(1, sizeof(*char_4));
    char *char_5 = calloc(1, sizeof(*char_5));
    char *char_6 = calloc(1, sizeof(*char_6));
    char *char_7 = calloc(1, sizeof(*char_7));
    char *char_8 = calloc(1, sizeof(*char_8));
    char *char_9 = calloc(1, sizeof(*char_9));

    *char_0 = 'a';
    *char_1 = 'b';
    *char_2 = 'c';
    *char_3 = 'd';
    *char_4 = 'e';
    *char_5 = 'f';
    *char_6 = 'h';
    *char_7 = 'i';
    *char_8 = 'j';
    *char_9 = 'k';

    pqueue_t *p_pq = pqueue_create();
    pqueue_insert(p_pq, 5, char_5);
    pqueue_insert(p_pq, 7, char_7);
    pqueue_insert(p_pq, 1, char_1);
    pqueue_insert(p_pq, 3, char_3);
    pqueue_insert(p_pq, 9, char_9);
    pqueue_insert(p_pq, 8, char_8);
    pqueue_insert(p_pq, 2, char_2);
    pqueue_insert(p_pq, 0, char_0);
    pqueue_insert(p_pq, 6, char_6);
    pqueue_insert(p_pq, 4, char_4);

    size_t priority = 0;

    char *temp = pqueue_peek(p_pq, &priority);

    printf("Expecting priority '9' and char 'k'\n");
    printf("priority: %zu, char: %c\n", priority, *temp);

    pqueue_destroy(&p_pq, free);
}

void test_pqueue_sorted(void)
{
}

void test_pqueue_backwards(void)
{
}

void test_pqueue_large(void)
{
    pqueue_t *p_pq = pqueue_create();
    pqueue_insert(p_pq, 5, NULL);
    pqueue_insert(p_pq, 7, NULL);
    pqueue_insert(p_pq, 1, NULL);
    pqueue_insert(p_pq, 3, NULL);
    pqueue_insert(p_pq, 9, NULL);
    pqueue_insert(p_pq, 8, NULL);
    pqueue_insert(p_pq, 2, NULL);
    pqueue_insert(p_pq, 0, NULL);
    pqueue_insert(p_pq, 6, NULL);
    pqueue_insert(p_pq, 4, NULL);
    pqueue_insert(p_pq, 5, NULL);
    pqueue_insert(p_pq, 7, NULL);
    pqueue_insert(p_pq, 1, NULL);
    pqueue_insert(p_pq, 3, NULL);
    pqueue_insert(p_pq, 9, NULL);
    pqueue_insert(p_pq, 8, NULL);
    pqueue_insert(p_pq, 2, NULL);
    pqueue_insert(p_pq, 0, NULL);
    pqueue_insert(p_pq, 6, NULL);
    pqueue_insert(p_pq, 4, NULL);
    pqueue_insert(p_pq, 5, NULL);
    pqueue_insert(p_pq, 7, NULL);
    pqueue_insert(p_pq, 1, NULL);
    pqueue_insert(p_pq, 3, NULL);
    pqueue_insert(p_pq, 9, NULL);
    pqueue_insert(p_pq, 8, NULL);
    pqueue_insert(p_pq, 2, NULL);
    pqueue_insert(p_pq, 0, NULL);
    pqueue_insert(p_pq, 6, NULL);
    pqueue_insert(p_pq, 4, NULL);
    pqueue_insert(p_pq, 5, NULL);
    pqueue_insert(p_pq, 7, NULL);
    pqueue_insert(p_pq, 1, NULL);
    pqueue_insert(p_pq, 3, NULL);
    pqueue_insert(p_pq, 9, NULL);
    pqueue_insert(p_pq, 8, NULL);
    pqueue_insert(p_pq, 2, NULL);
    pqueue_insert(p_pq, 0, NULL);
    pqueue_insert(p_pq, 6, NULL);
    pqueue_insert(p_pq, 4, NULL);

    size_t priority = 0;

    printf("Expecting four '9's and four '8's\n");
    pqueue_remove(p_pq, &priority);
    printf("priority: %zu\n", priority);
    pqueue_remove(p_pq, &priority);
    printf("priority: %zu\n", priority);
    pqueue_remove(p_pq, &priority);
    printf("priority: %zu\n", priority);
    pqueue_remove(p_pq, &priority);
    printf("priority: %zu\n", priority);

    pqueue_remove(p_pq, &priority);
    printf("priority: %zu\n", priority);
    pqueue_remove(p_pq, &priority);
    printf("priority: %zu\n", priority);
    pqueue_remove(p_pq, &priority);
    printf("priority: %zu\n", priority);
    pqueue_remove(p_pq, &priority);
    printf("priority: %zu\n", priority);

    pqueue_destroy(&p_pq, NULL);
}

void test_pqueue_all_zeros(void)
{
}

void test_pqueue_single(void)
{
}

void test_pqueue_bad(void)
{
}
