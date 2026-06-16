#include "unity.h"

// Test avl.c
void test_avl_create_tree(void);

void test_avl_add_node(void);

void test_avl_find(void);

void test_avl_get_root(void);

void test_avl_minimum_value(void);

void test_avl_maximum_value(void);

void test_avl_size_of_tree(void);

void test_avl_is_empty(void);

void test_avl_print(void);

void test_avl_delete_node(void);

void test_avl_clear(void);

void test_avl_destroy_tree(void);

void test_avl_pre_order(void);

void test_avl_post_order(void);

void test_avl_in_order(void);

void setUp(void)
{
}

void tearDown(void)
{
}
int main(void)
{

    UNITY_BEGIN();

    RUN_TEST(test_avl_create_tree);

    RUN_TEST(test_avl_destroy_tree);

    RUN_TEST(test_avl_clear);

    RUN_TEST(test_avl_delete_node);

    RUN_TEST(test_avl_add_node);

    RUN_TEST(test_avl_find);

    RUN_TEST(test_avl_get_root);

    RUN_TEST(test_avl_minimum_value);

    RUN_TEST(test_avl_maximum_value);

    RUN_TEST(test_avl_size_of_tree);

    RUN_TEST(test_avl_is_empty);

    UNITY_END();

    return 0;
}

/* End of test_all.c */