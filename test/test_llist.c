
#include "llist.h"
#include "unity.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Functions to test

void *llist_remove_at(llist_t *p_list, int index);

int llist_size(llist_t *p_list);

void llist_iterate(llist_t *p_list, llist_iter_f iter_f);

*/

void test_llist_create(void)
{
    printf("\nTesting llist_create...\n");
    llist_t *p_list = llist_create();

    if ((NULL != p_list))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_destroy(void)
{
    printf("\nTesting llist_destroy...\n");
    llist_t *p_list = llist_create();

    llist_destroy(&p_list, NULL);

    if (NULL == p_list)
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_insert_head_good(void)
{
    printf("\nTesting llist_insert_head with good values...\n");
    llist_t *p_list = llist_create();
    int test_data8 = 8;
    int test_data6 = 6;
    // int test_data7 = 7;
    // int test_data5 = 5;
    // int test_data3 = 3;
    // int test_data0 = 0;
    // int test_data9 = 9;

    int test_val = llist_insert_head(p_list, &test_data8);
    int *return_data = llist_remove_at(p_list, 0);

    if ((1 == test_val) && (8 == *return_data))
    {
    }
    else
    {
        TEST_FAIL();
    }

    test_val = llist_insert_head(p_list, &test_data6);
    return_data = llist_remove_at(p_list, 0);

    if ((1 == test_val) && (6 == *return_data))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_insert_head_bad(void)
{
    printf("\nTesting llist_insert_head with bad values...\n");

    int test_val = llist_insert_head(NULL, NULL);

    if (-1 == test_val)
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_insert_tail_good(void)
{
    printf("\nTesting llist_insert_tail with good values...\n");
    llist_t *p_list = llist_create();
    int test_data8 = 8;
    int test_data6 = 6;
    int test_data7 = 7;
    int test_data5 = 5;
    // int test_data3 = 3;
    // int test_data0 = 0;
    // int test_data9 = 9;

    llist_insert_tail(p_list, &test_data7);
    int test_val = llist_insert_tail(p_list, &test_data8);
    int *return_data = llist_remove_at(p_list, 1);

    if ((1 == test_val) && (8 == *return_data))
    {
    }
    else
    {
        TEST_FAIL();
    }

    llist_insert_tail(p_list, &test_data5);
    test_val = llist_insert_tail(p_list, &test_data6);
    return_data = llist_remove_at(p_list, 2);

    if ((1 == test_val) && (6 == *return_data))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_insert_tail_bad(void)
{
    printf("\nTesting llist_insert_tail with bad values...\n");

    int test_val = llist_insert_tail(NULL, NULL);

    if (-1 == test_val)
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_insert_at_good(void)
{
    printf("\nTesting llist_insert_at with good values...\n");
    llist_t *p_list = llist_create();
    int test_data8 = 8;
    int test_data6 = 6;
    int test_data7 = 7;
    int test_data5 = 5;
    // int test_data3 = 3;
    // int test_data0 = 0;
    // int test_data9 = 9;

    llist_insert_head(p_list, &test_data8);
    llist_insert_tail(p_list, &test_data6);
    llist_insert_tail(p_list, &test_data5);

    int test_val = llist_insert_at(p_list, &test_data7, 2);
    int *return_data = llist_remove_at(p_list, 2);

    if ((1 == test_val) && (7 == *return_data))
    {
    }
    else
    {
        TEST_FAIL();
    }

    test_val = llist_insert_at(p_list, &test_data7, 1);
    return_data = llist_remove_at(p_list, 1);

    if ((1 == test_val) && (7 == *return_data))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_insert_at_bad(void)
{
    printf("\nTesting llist_insert_at with bad values...\n");
    int test_val = llist_insert_at(NULL, NULL, 1);

    if (-1 == test_val)
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

__attribute__((unused)) static int cmp_str(const void *p_a, const void *p_b)
{
    return strcmp((const char *)p_a, (const char *)p_b);
}

void test_llist_index_of_good(void)
{
    printf("\nTesting llist_index_of with good values...\n");

    llist_t *p_list = llist_create();
    char *test_data0 = calloc(1, sizeof(test_data0));
    char *test_data1 = calloc(1, sizeof(test_data1));
    char *test_data2 = calloc(1, sizeof(test_data2));
    char *test_data3 = calloc(1, sizeof(test_data3));

    test_data0 = strcpy(test_data0, "a");
    test_data1 = strcpy(test_data1, "b");
    test_data2 = strcpy(test_data2, "c");
    test_data3 = strcpy(test_data3, "d");
    // int test_data3 = 3;
    // int test_data0 = 0;
    // int test_data9 = 9;

    llist_insert_head(p_list, test_data0);
    llist_insert_tail(p_list, test_data1);
    llist_insert_tail(p_list, test_data2);
    llist_insert_tail(p_list, test_data3);

    int test_val = llist_index_of(p_list, (char *)"c", cmp_str);
    char *return_data = llist_remove_at(p_list, test_val);

    if ((2 == test_val) && (0 == strcmp((char *)"c", return_data)))
    {
    }
    else
    {
        TEST_FAIL();
    }

    test_val = llist_index_of(p_list, (char *)"d", cmp_str);
    return_data = llist_remove_at(p_list, test_val);

    if ((2 == test_val) && (0 == strcmp((char *)"d", return_data)))
    {
    }
    else
    {
        TEST_FAIL();
    }

    test_val = llist_index_of(p_list, (char *)"b", cmp_str);
    return_data = llist_remove_at(p_list, test_val);

    if ((1 == test_val) && (0 == strcmp((char *)"b", return_data)))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_index_of_bad(void)
{
    printf("\nTesting llist_index_of with bad values...\n");

    int test_val = llist_index_of(NULL, NULL, NULL);

    if (-1 == test_val)
    {
    }
    else
    {
        TEST_FAIL();
    }

    llist_t *p_list = llist_create();
    char *test_data0 = calloc(1, sizeof(test_data0));
    char *test_data1 = calloc(1, sizeof(test_data1));

    test_data0 = strcpy(test_data0, "a");
    test_data1 = strcpy(test_data1, "b");

    test_val = llist_index_of(p_list, (char *)"c", cmp_str);

    if (-1 == test_val)
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_size_good(void)
{
    printf("\nTesting llist_size with good values...\n");

    llist_t *p_list = llist_create();
    char *test_data0 = calloc(1, sizeof(test_data0));
    char *test_data1 = calloc(1, sizeof(test_data1));
    char *test_data2 = calloc(1, sizeof(test_data2));
    char *test_data3 = calloc(1, sizeof(test_data3));

    test_data0 = strcpy(test_data0, "a");
    test_data1 = strcpy(test_data1, "b");
    test_data2 = strcpy(test_data2, "c");
    test_data3 = strcpy(test_data3, "d");
    // int test_data3 = 3;
    // int test_data0 = 0;
    // int test_data9 = 9;

    llist_insert_head(p_list, test_data0);
    llist_insert_tail(p_list, test_data1);
    llist_insert_tail(p_list, test_data2);
    llist_insert_tail(p_list, test_data3);

    int test_val = llist_size(p_list);

    if (4 == test_val)
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_size_bad(void)
{
    printf("\nTesting llist_size with good values...\n");

    int test_val = llist_size(NULL);

    if (-1 == test_val)
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

/* End of file test_llist.c */
