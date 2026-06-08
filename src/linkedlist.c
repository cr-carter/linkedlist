#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dll.h"

#define ARR_LEN(arr) (sizeof(arr) / sizeof(*arr))

// qsort gives the address of the pointer because the caller owns all the data
int cmp_qsort(const void *p_left, const void *p_right)
{
    return strcmp(*(const char **)p_left, *(const char **)p_right);
}

int int_qsort(const void *p_left, const void *p_right)
{
    if (**(int **)p_left == **(int **)p_right)
    {
        return 0;
    }

    if (**(int **)p_left > **(int **)p_right)
    {
        return 1;
    }

    return -1;
}

// you would not want to expose the internal address of your node, so no deref
int cmp_dll(const void *p_left, const void *p_right)
{
    return strcmp((const char *)p_left, (const char *)p_right);
}

int int_cmp_dll(const void *p_left, const void *p_right)
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

void print_dll(void *p_data)
{
    printf("%s <-> ", (const char *)p_data);
}

void print_int_dll(void *p_data)
{
    printf("%d <-> ", *(int *)p_data);
}

int main(void)
{
    // no p, since it's an array OF pointers (I think this is right?)
    const char *words[] = {
        "tangerine", "apple", "fig",   "mango",  "cherry", "banana", "date",      "elderberry",
        "grape",     "kiwi",  "lemon", "orange", "papaya", "quince", "raspberry",
    };

    int arr_len = ARR_LEN(words);

    dll_t *p_dll = dll_create();
    assert(p_dll != NULL);

    for (int idx = 0; idx < arr_len; idx++)
    {
        if (-1 == dll_insert_tail(p_dll, (char *)words[idx]))
        {
            fprintf(stderr, "[ERR]: Insertion of %s\n", words[idx]);
            break;
        }
    }

    qsort(words, (size_t)arr_len, sizeof(*words), cmp_qsort);
    dll_selection_sort(p_dll, cmp_dll);

    for (int idx = 0; idx < arr_len; idx++)
    {
        printf("%s     ", words[idx]);
    }
    puts("");

    dll_iterate(p_dll, print_dll);
    puts("");

    dll_destroy(&p_dll, NULL);

    /* ------------------------ */
    /* ----- Integer test ----- */
    /* ------------------------ */
    int *test6 = calloc(1, sizeof(int));
    *test6 = 6;
    int *test5 = calloc(1, sizeof(int));
    *test5 = 5;
    int *test3 = calloc(1, sizeof(int));
    *test3 = 3;
    int *test8 = calloc(1, sizeof(int));
    *test8 = 8;
    int *test1 = calloc(1, sizeof(int));
    *test1 = 1;
    int *test7 = calloc(1, sizeof(int));
    *test7 = 7;
    int *test2 = calloc(1, sizeof(int));
    *test2 = 2;

    int *ints[] = {test6, test5, test3, test8, test1, test7, test2};

    arr_len = ARR_LEN(ints);

    p_dll = dll_create();
    assert(p_dll != NULL);

    for (int idx = 0; idx < arr_len; idx++)
    {
        if (-1 == dll_insert_tail(p_dll, ints[idx]))
        {
            fprintf(stderr, "[ERR]: Insertion of %i\n", *ints[idx]);
            break;
        }
    }
    printf("Unsorted:\n");
    for (int idx = 0; idx < arr_len; idx++)
    {
        printf("%d     ", *ints[idx]);
    }
    puts("");
    qsort(ints, (size_t)arr_len, sizeof(*ints), int_qsort);
    dll_insertion_sort(p_dll, int_cmp_dll);

    for (int idx = 0; idx < arr_len; idx++)
    {
        printf("%d     ", *ints[idx]);
    }
    puts("");

    dll_iterate(p_dll, print_int_dll);
    puts("");

    dll_destroy(&p_dll, NULL);
    free(test1);
    free(test2);
    free(test3);
    free(test5);
    free(test6);
    free(test7);
    free(test8);
}