
#include "dll.h"
#include "unity.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
void dll_iterate(dll_t *p_list, dll_iter_f iter_f)
void dll_selection_sort(dll_t *p_list, dll_compar_f cmp_func)
void dll_insertion_sort(dll_t *p_list, dll_compar_f cmp_func)
dll_iter_t *dll_iter_create(dll_t **pp_list)
void *dll_iter_next(dll_iter_t *p_iter)
void *dll_iter_prev(dll_iter_t *p_iter)
void *dll_iter_data(dll_iter_t *p_iter)
void dll_iter_destroy(dll_iter_t **pp_iter)
int dll_iter_reset(dll_iter_t *p_iter)
*/

void test_dll_create(void)
{
    printf("\nTesting dll_create...\n");
    dll_t *p_list = dll_create();

    if ((NULL != p_list))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_dll_insert_head(void)
{
    printf("\nTesting dll_insert_head and dll_remove_head...\n");
    dll_t *p_list = dll_create();
    int test_data8 = 8;
    int test_data6 = 6;
    int test_data7 = 7;
    // int test_data5 = 5;
    // int test_data3 = 3;
    // int test_data0 = 0;
    // int test_data9 = 9;

    printf("Testing with a single insertion...\n");
    int test_val = dll_insert_head(p_list, &test_data8);
    int *return_data = dll_remove_head(p_list);

    if ((1 == test_val) && (8 == *return_data))
    {
    }
    else
    {
        TEST_FAIL();
    }

    printf("Testing with two insertions...\n");
    dll_insert_head(p_list, &test_data7);
    test_val = dll_insert_head(p_list, &test_data6);
    return_data = dll_remove_head(p_list);

    if ((1 == test_val) && (6 == *return_data))
    {
    }
    else
    {
        TEST_FAIL();
    }

    printf("Testing with bad values...\n");
    test_val = dll_insert_head(NULL, NULL);

    if (-1 == test_val)
    {
    }
    else
    {
        TEST_FAIL();
    }

    test_val = dll_insert_head(NULL, &test_data7);

    if (-1 == test_val)
    {
    }
    else
    {
        TEST_FAIL();
    }
}

void test_dll_insert_tail(void)
{
    printf("\nTesting dll_insert_tail and dll_remove_tail...\n");

    dll_t *p_list = dll_create();
    int test_data8 = 8;
    int test_data6 = 6;
    int test_data7 = 7;
    // int test_data5 = 5;
    // int test_data3 = 3;
    // int test_data0 = 0;
    // int test_data9 = 9;

    printf("Testing with a single tail insertion...\n");
    int test_val = dll_insert_tail(p_list, &test_data8);
    int *return_data = dll_remove_tail(p_list);

    if ((1 == test_val) && (8 == *return_data))
    {
    }
    else
    {
        TEST_FAIL();
    }

    printf("Testing with two tail insertions...\n");
    dll_insert_tail(p_list, &test_data7);
    test_val = dll_insert_tail(p_list, &test_data6);
    return_data = dll_remove_tail(p_list);

    if ((1 == test_val) && (6 == *return_data))
    {
    }
    else
    {
        TEST_FAIL();
    }

    printf("Testing with bad values...\n");
    test_val = dll_insert_tail(NULL, NULL);

    if (-1 == test_val)
    {
    }
    else
    {
        TEST_FAIL();
    }

    test_val = dll_insert_tail(NULL, &test_data7);

    if (-1 == test_val)
    {
    }
    else
    {
        TEST_FAIL();
    }
}

void test_dll_destroy(void)
{
    printf("\nTesting dll_destroy...\n");
    dll_t *p_list = dll_create();

    if ((NULL != p_list))
    {
    }
    else
    {
        TEST_FAIL();
    }

    printf("Test destroying empty list...\n");
    dll_destroy(&p_list, NULL);

    if ((NULL == p_list))
    {
    }
    else
    {
        TEST_FAIL();
    }

    p_list = dll_create();
    int *test_data8 = calloc(1, sizeof(int));
    int *test_data6 = calloc(1, sizeof(int));
    int *test_data7 = calloc(1, sizeof(int));
    *test_data6 = 6;
    *test_data7 = 7;
    *test_data8 = 8;

    if ((NULL != p_list))
    {
        dll_insert_head(p_list, test_data6);
        dll_insert_head(p_list, test_data7);
        dll_insert_head(p_list, test_data8);
    }
    else
    {
        TEST_FAIL();
    }

    printf("Test destroying list with data...\n");
    dll_destroy(&p_list, NULL);

    if ((NULL == p_list))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_dll_size(void)
{
    printf("\nTesting dll_size...\n");
    dll_t *p_list = dll_create();
    int test_data8 = 8;
    int test_data6 = 6;
    int test_data7 = 7;
    // int test_data5 = 5;
    // int test_data3 = 3;
    // int test_data0 = 0;
    // int test_data9 = 9;

    printf("Testing with bad values...\n");
    dll_insert_head(p_list, &test_data7);
    dll_insert_head(p_list, &test_data6);
    dll_insert_head(p_list, &test_data8);
    int test_val = dll_size(NULL);

    if (-1 == test_val)
    {
    }
    else
    {
        TEST_FAIL();
    }

    printf("Testing with good values...\n");
    test_val = dll_size(p_list);
    if (3 == test_val)
    {
    }
    else
    {
        TEST_FAIL();
    }

    dll_remove_head(p_list);
    dll_remove_tail(p_list);
    test_val = dll_size(p_list);
    if (1 == test_val)
    {
    }
    else
    {
        TEST_FAIL();
    }
}

void test_dll_iterate(void)
{
    printf("\nTesting dll_iterate...\n");
}

void test_dll_selection_sort(void)
{
    printf("\nTesting dll_selection_sort...\n");
}

void test_dll_insertion_sort(void)
{
    printf("\nTesting dll_insertion_sort...\n");
}

void test_dll_iter_create(void)
{
    printf("\nTesting dll_iter_create...\n");
    dll_t *p_list = dll_create();
    int test_data8 = 8;
    int test_data6 = 6;
    int test_data7 = 7;

    dll_insert_head(p_list, &test_data7);
    dll_insert_head(p_list, &test_data6);
    dll_insert_head(p_list, &test_data8);

    dll_iter_t *p_iter = dll_iter_create(NULL);

    if (NULL != p_iter)
    {
        TEST_FAIL();
    }

    p_iter = dll_iter_create(&p_list);

    if (NULL != p_iter)
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_dll_iter_next(void)
{
    printf("\nTesting dll_iter_next...\n");
    dll_t *p_list = dll_create();
    int test_data8 = 8;
    int test_data6 = 6;
    int test_data7 = 7;

    dll_insert_head(p_list, &test_data7);
    dll_insert_head(p_list, &test_data6);
    dll_insert_head(p_list, &test_data8);

    dll_iter_t *p_iter = dll_iter_create(&p_list);

    if (NULL == p_iter)
    {
        TEST_FAIL();
    }

    void *p_test_iter = dll_iter_next(p_iter);

    if (NULL == p_test_iter)
    {
        TEST_FAIL();
    }

    if (8 == *(int *)p_test_iter)
    {
    }
    else
    {
        TEST_FAIL();
    }

    p_test_iter = dll_iter_next(p_iter);

    if (NULL == p_test_iter)
    {
        TEST_FAIL();
    }

    if (6 == *(int *)p_test_iter)
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_dll_iter_prev(void)
{
    printf("\nTesting dll_iter_prev...\n");
    dll_t *p_list = dll_create();
    int test_data8 = 8;
    int test_data6 = 6;
    int test_data7 = 7;

    dll_insert_head(p_list, &test_data7);
    dll_insert_head(p_list, &test_data6);
    dll_insert_head(p_list, &test_data8);

    dll_iter_t *p_iter = dll_iter_create(&p_list);

    if (NULL == p_iter)
    {
        TEST_FAIL();
    }

    void *p_test_iter = dll_iter_prev(p_iter);

    if (NULL == p_test_iter)
    {
        TEST_FAIL();
    }

    if (7 == *(int *)p_test_iter)
    {
    }
    else
    {
        TEST_FAIL();
    }

    p_test_iter = dll_iter_prev(p_iter);

    if (NULL == p_test_iter)
    {
        TEST_FAIL();
    }

    if (6 == *(int *)p_test_iter)
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_dll_iter_data(void)
{
    printf("\nTesting dll_iter_data...\n");
}

void test_dll_iter_destroy(void)
{
    printf("\nTesting dll_iter_destroy...\n");
}

void test_dll_iter_reset(void)
{
    printf("\nTesting dll_iter_reset...\n");
}

/* End of file test_dll.c */
