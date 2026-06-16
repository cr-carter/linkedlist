#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"

#define ARR_LEN(arr) (sizeof(arr) / sizeof(*arr))

// qsort gives the address of the pointer because the caller owns all the data
int cmp_qsort(const void *p_left, const void *p_right)
{
    return strcmp(*(const char **)p_left, *(const char **)p_right);
}

int int_qsort(const void *p_left, const void *p_right)
{
    if (*(int *)p_left == *(int *)p_right)
    {
        return 0;
    }
    if (*(int *)p_left > *(int *)p_right)
    {
        return 1;
    }

    return -1;
}

void print_int(const void *p_data)
{
    printf("%i - ", *(int *)p_data);
}

int main(void)
{
    tree_t *p_tree = avl_create_tree(int_qsort, free);

    int *test1 = calloc(1, sizeof(int));
    int *test2 = calloc(1, sizeof(int));
    int *test3 = calloc(1, sizeof(int));
    int *test4 = calloc(1, sizeof(int));
    int *test5 = calloc(1, sizeof(int));
    int *test6 = calloc(1, sizeof(int));
    int *test7 = calloc(1, sizeof(int));
    int *test8 = calloc(1, sizeof(int));
    int *test9 = calloc(1, sizeof(int));
    int *test10 = calloc(1, sizeof(int));
    int *test11 = calloc(1, sizeof(int));
    int *test12 = calloc(1, sizeof(int));
    int *test13 = calloc(1, sizeof(int));
    int *test14 = calloc(1, sizeof(int));
    int *test15 = calloc(1, sizeof(int));

    *test1 = 1;
    *test2 = 2;
    *test3 = 3;
    *test4 = 4;
    *test5 = 5;
    *test6 = 6;
    *test7 = 7;
    *test8 = 8;
    *test9 = 9;
    *test10 = 10;
    *test11 = 11;
    *test12 = 12;
    *test13 = 13;
    *test14 = 14;
    *test15 = 15;

    avl_add_node(p_tree, test1);
    avl_add_node(p_tree, test2);
    avl_add_node(p_tree, test3);
    avl_add_node(p_tree, test4);
    avl_add_node(p_tree, test5);
    avl_add_node(p_tree, test6);
    avl_add_node(p_tree, test7);
    avl_add_node(p_tree, test8);
    avl_add_node(p_tree, test9);
    avl_add_node(p_tree, test10);
    avl_add_node(p_tree, test11);
    avl_add_node(p_tree, test12);
    avl_add_node(p_tree, test13);
    avl_add_node(p_tree, test14);
    avl_add_node(p_tree, test15);

    printf("##### Here is your tree #####\n");
    avl_print(p_tree, print_int);

    printf("\n\n");

    printf("##### Check data #####\n");
    printf("avl_is_empty()\nExpected: false\nActual: ");
    if (avl_is_empty(p_tree))
    {
        printf("true\n");
    }
    else
    {
        printf("false\n");
    }
    puts("");

    printf("avl_size_of_tree()\nExpected: 15\nActual: ");
    size_t tree_size = avl_size_of_tree(p_tree);
    printf("%zu\n", tree_size);
    puts("");

    printf("avl_minimum_value()\nExpected: 1\nActual: ");
    int *min_value = avl_minimum_value(p_tree);
    printf("%i\n", *min_value);
    puts("");

    printf("avl_maximum_value()\nExpected: 15\nActual: ");
    int *max_value = avl_maximum_value(p_tree);
    printf("%i", *max_value);
    puts("\n");

    printf("avl_in_order\n");
    avl_in_order(p_tree, print_int);
    printf("\n\n");

    printf("avl_pre_order\n");
    avl_pre_order(p_tree, print_int);
    printf("\n\n");

    printf("avl_post_order\n");
    avl_post_order(p_tree, print_int);
    printf("\n\n");

    avl_delete_node(p_tree, test15);
    avl_delete_node(p_tree, test12);
    avl_delete_node(p_tree, test4);
    avl_delete_node(p_tree, test3);
    avl_delete_node(p_tree, test2);
    avl_delete_node(p_tree, test1);

    printf("##### Here is modified tree #####\n");
    avl_print(p_tree, print_int);

    printf("\n\n");

    printf("##### Check data #####\n");
    printf("avl_is_empty()\nExpected: false\nActual: ");
    if (avl_is_empty(p_tree))
    {
        printf("true\n");
    }
    else
    {
        printf("false\n");
    }
    puts("");

    printf("avl_size_of_tree()\nExpected: 9\nActual: ");
    tree_size = avl_size_of_tree(p_tree);
    printf("%zu\n", tree_size);
    puts("");

    printf("avl_minimum_value()\nExpected: 5\nActual: ");
    min_value = avl_minimum_value(p_tree);
    printf("%i\n", *min_value);
    puts("");

    printf("avl_maximum_value()\nExpected: 14\nActual: ");
    max_value = avl_maximum_value(p_tree);
    printf("%i", *max_value);
    puts("\n");

    printf("avl_in_order\n");
    avl_in_order(p_tree, print_int);
    printf("\n\n");

    printf("avl_pre_order\n");
    avl_pre_order(p_tree, print_int);
    printf("\n\n");

    printf("avl_post_order\n");
    avl_post_order(p_tree, print_int);
    printf("\n\n");

     printf("##### Here is modified tree #####\n");
    test15 = calloc(1, sizeof(int));
    test12 = calloc(1, sizeof(int));
    test2 = calloc(1, sizeof(int));
    test4 = calloc(1, sizeof(int));
    test3 = calloc(1, sizeof(int));
    test1 = calloc(1, sizeof(int));

    *test15 = 15;
    *test12 = 12;
    *test2 = 2;
    *test4 = 4;
    *test3 = 3;
    *test1 = 1;

    avl_add_node(p_tree, test15);
    avl_add_node(p_tree, test12);
    avl_add_node(p_tree, test2);
    avl_add_node(p_tree, test4);
    avl_add_node(p_tree, test3);
    avl_add_node(p_tree, test1);

    avl_print(p_tree, print_int);
    puts("\n");

    printf("##### avl_clear() #####\n");
    avl_clear(p_tree);

    printf("avl_size_of_tree()\nExpected: 0\nActual: ");
    tree_size = avl_size_of_tree(p_tree);
    printf("%zu\n", tree_size);
    puts("");

    printf("avl_minimum_value()\nExpected: 0\nActual: ");
    min_value = avl_minimum_value(p_tree);
    printf("%i\n", (NULL != min_value) ? *min_value : 0);
    puts("");

    printf("avl_maximum_value()\nExpected: 0\nActual: ");
    max_value = avl_maximum_value(p_tree);
    printf("%i", (NULL != max_value) ? *max_value : 0);
    puts("\n");

    printf("avl_in_order\n");
    avl_in_order(p_tree, print_int);
    printf("\n\n");

    printf("avl_pre_order\n");
    avl_pre_order(p_tree, print_int);
    printf("\n\n");

    printf("avl_post_order\n");
    avl_post_order(p_tree, print_int);
    printf("\n\n");

    avl_destroy_tree(&p_tree);
}