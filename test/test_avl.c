
#include "avl.h"
#include "unity.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Functions to test

void avl_print(tree_t *p_tree, print_fn print);

void avl_pre_order(tree_t *p_tree, order_fn order_func);

void avl_post_order(tree_t *p_tree, order_fn order_func);

void avl_in_order(tree_t *p_tree, order_fn order_func);
*/

// Helper functions

int str_comp(const void *p_left, const void *p_right)
{
    return strcmp(*(const char **)p_left, *(const char **)p_right);
}

int int_comp(const void *p_left, const void *p_right)
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

// Test functions

void test_avl_create_tree(void)
{
    puts("\n");
    printf("###############\nTesting avl_create_tree...\n");
    tree_t *p_test = NULL;
    TEST_ASSERT_NULL(p_test);

    puts("avl_create_tree(NULL, NULL), should not return tree");
    p_test = avl_create_tree(NULL, NULL);
    TEST_ASSERT_NULL(p_test);

    puts("avl_create_tree(NULL, free), should not return tree");
    p_test = avl_create_tree(NULL, free);
    TEST_ASSERT_NULL(p_test);

    puts("avl_create_tree(int_comp, NULL), should return tree");
    p_test = avl_create_tree(int_comp, NULL);
    TEST_ASSERT_NOT_NULL(p_test);

    puts("avl_create_tree(int_comp, free), should return tree");
    avl_destroy_tree(&p_test);
    p_test = avl_create_tree(int_comp, free);
    TEST_ASSERT_NOT_NULL(p_test);
}

void test_avl_clear(void)
{
    puts("\n");
    printf("###############\nTesting avl_clear_tree...\n");
    tree_t *p_test = avl_create_tree(int_comp, NULL);
    int test5 = 5;

    avl_add_node(p_test, &test5);
    TEST_ASSERT_EQUAL(1, avl_size_of_tree(p_test));

    puts("avl_add_clear(p_tree), with p_tree having at least one node. Checking size should return 0");
    avl_clear(p_test);
    TEST_ASSERT_EQUAL(0, avl_size_of_tree(p_test));

    puts("avl_add_clear(p_tree), with p_tree having at least no nodes. Checking size should return 0");
    avl_clear(p_test);
    TEST_ASSERT_EQUAL(0, avl_size_of_tree(p_test));
}

void test_avl_destroy_tree(void)
{
    puts("\n");
    printf("###############\nTesting avl_destroy_tree...\n");
    tree_t *p_test = avl_create_tree(int_comp, NULL);
    TEST_ASSERT_NOT_NULL(p_test);

    int test5 = 5;
    avl_add_node(p_test, &test5);

    puts("avl_destroy_tree(&p_tree), p_tree should now be NULL");
    avl_destroy_tree(&p_test);
    TEST_ASSERT_NULL(p_test);
}

void test_avl_delete_node(void)
{
    puts("\n");
    printf("###############\nTesting avl_delete_node...\n");
    tree_t *p_test = avl_create_tree(int_comp, NULL);
    int test5 = 5;
    int test3 = 3;
    bool check = false;

    avl_add_node(p_test, &test5);
    TEST_ASSERT_EQUAL(1, avl_size_of_tree(p_test));

    puts("avl_delete_node(p_tree, &test3), with test3 not being in tree, should return false");
    check = avl_delete_node(p_test, &test3);
    TEST_ASSERT_EQUAL(1, avl_size_of_tree(p_test));
    TEST_ASSERT_EQUAL(false, check);

    puts("avl_delete_node(p_tree, &test5), with test5 being in tree, should return true");
    check = avl_delete_node(p_test, &test5);
    TEST_ASSERT_EQUAL(0, avl_size_of_tree(p_test));
    TEST_ASSERT_EQUAL(true, check);
}

void test_avl_add_node(void)
{
    puts("\n");
    printf("###############\nTesting avl_add_node...\n");
    tree_t *p_test = avl_create_tree(int_comp, NULL);
    int test5 = 5;
    int test7 = 7;
    int test9 = 9;
    bool check = false;

    puts("avl_add_node(p_tree, &test5), with test5 not already in tree, should return true");
    check = avl_add_node(p_test, &test5);
    TEST_ASSERT_EQUAL(1, avl_size_of_tree(p_test));
    TEST_ASSERT_EQUAL(true, check);

    puts("avl_add_node(p_tree, &test5), with test5 already in tree, should return false");
    check = avl_add_node(p_test, &test5);
    TEST_ASSERT_EQUAL(1, avl_size_of_tree(p_test));
    TEST_ASSERT_EQUAL(false, check);

    puts("avl_add_node(p_tree, &test7), with test7 not already in tree, should return true, checking size should "
         "return 2");
    check = avl_add_node(p_test, &test7);
    TEST_ASSERT_EQUAL(2, avl_size_of_tree(p_test));
    TEST_ASSERT_EQUAL(true, check);

    puts("avl_add_node(p_tree, &test9), with test9 not already in tree, should return true, checking size should "
         "return 3, the root should be 7 after rotation");
    check = avl_add_node(p_test, &test9);
    TEST_ASSERT_EQUAL(3, avl_size_of_tree(p_test));
    TEST_ASSERT_EQUAL(true, check);
    TEST_ASSERT_EQUAL(7, *(int *)avl_get_root(p_test));
}

void test_avl_find(void)
{
    puts("\n");
    printf("###############\nTesting avl_find...\n");
    tree_t *p_test = avl_create_tree(int_comp, NULL);
    int test5 = 5;
    int test3 = 3;
    int test7 = 7;
    int *check = NULL;

    avl_add_node(p_test, &test5);
    avl_add_node(p_test, &test3);
    TEST_ASSERT_EQUAL(2, avl_size_of_tree(p_test));

    puts("avl_find(p_tree, &test3), with test3 in tree, should return test3");
    check = avl_find(p_test, &test3);
    TEST_ASSERT_NOT_NULL(check);

    puts("avl_find(p_tree, &test7), with test7 not in tree, should return NULL");
    check = avl_find(p_test, &test7);
    TEST_ASSERT_NULL(check);
}

void test_avl_get_root(void)
{
    puts("\n");
    printf("###############\nTesting avl_get_root...\n");
    tree_t *p_test = avl_create_tree(int_comp, NULL);
    int test5 = 5;
    int test3 = 3;
    int test7 = 7;
    int *check = NULL;

    avl_add_node(p_test, &test5);
    avl_add_node(p_test, &test3);
    avl_add_node(p_test, &test7);
    TEST_ASSERT_EQUAL(3, avl_size_of_tree(p_test));

    puts("avl_get_root(p_tree), should return test5");
    check = avl_get_root(p_test);
    TEST_ASSERT_NOT_NULL(check);
    TEST_ASSERT_EQUAL(5, *(int *)check);
}

void test_avl_minimum_value(void)
{
    puts("\n");
    printf("###############\nTesting avl_minimum_value...\n");
    tree_t *p_test = avl_create_tree(int_comp, NULL);
    int test5 = 5;
    int test3 = 3;
    int test7 = 7;
    int test1 = 1;
    int *check = NULL;

    avl_add_node(p_test, &test5);
    avl_add_node(p_test, &test3);
    avl_add_node(p_test, &test7);
    avl_add_node(p_test, &test1);
    TEST_ASSERT_EQUAL(4, avl_size_of_tree(p_test));

    puts("avl_minimum_value(p_tree), should return test1");
    check = avl_minimum_value(p_test);
    TEST_ASSERT_NOT_NULL(check);
    TEST_ASSERT_EQUAL(1, *(int *)check);
}

void test_avl_maximum_value(void)
{
    puts("\n");
    printf("###############\nTesting avl_maximum_value...\n");
    tree_t *p_test = avl_create_tree(int_comp, NULL);
    int test5 = 5;
    int test3 = 3;
    int test7 = 7;
    int test9 = 9;
    int *check = NULL;

    avl_add_node(p_test, &test5);
    avl_add_node(p_test, &test3);
    avl_add_node(p_test, &test7);
    avl_add_node(p_test, &test9);
    TEST_ASSERT_EQUAL(4, avl_size_of_tree(p_test));

    puts("avl_maximum_value(p_tree), should return test9");
    check = avl_maximum_value(p_test);
    TEST_ASSERT_NOT_NULL(check);
    TEST_ASSERT_EQUAL(9, *(int *)check);
}

void test_avl_size_of_tree(void)
{
    puts("\n");
    printf("###############\nTesting avl_size_of_tree...\n");
    tree_t *p_test = avl_create_tree(int_comp, NULL);
    int test5 = 5;
    int test3 = 3;
    int test7 = 7;
    int test9 = 9;

    avl_add_node(p_test, &test5);
    avl_add_node(p_test, &test3);

    TEST_ASSERT_EQUAL(2, avl_size_of_tree(p_test));
    puts("avl_size_of_tree(p_tree), should return 2");

    avl_add_node(p_test, &test7);
    avl_add_node(p_test, &test9);

    TEST_ASSERT_EQUAL(4, avl_size_of_tree(p_test));
    puts("avl_size_of_tree(p_tree), should return 4");
}

void test_avl_is_empty(void)
{
    puts("\n");
    printf("###############\nTesting avl_is_empty...\n");
    tree_t *p_test = avl_create_tree(int_comp, NULL);
    int test5 = 5;
    int test3 = 3;

    TEST_ASSERT_TRUE(avl_is_empty(p_test));
    puts("avl_is_empty(p_tree), after creating new tree, should return true");

    avl_add_node(p_test, &test5);
    avl_add_node(p_test, &test3);

    TEST_ASSERT_FALSE(avl_is_empty(p_test));
    puts("avl_is_empty(p_tree), after adding two values, should return false");

    avl_clear(p_test);
    TEST_ASSERT_TRUE(avl_is_empty(p_test));
    puts("avl_is_empty(p_tree), after avl_clear, should return true");
}

void test_avl_print(void);

void test_avl_pre_order(void);

void test_avl_post_order(void);

void test_avl_in_order(void);

/* End of file test_avl.c */
