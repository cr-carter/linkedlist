#include "unity.h"

// test_args
void test_llist_create_node_good(void);

void test_llist_create_node_bad(void);

void test_llist_insert_head_good(void);

void test_llist_insert_head_bad(void);

void test_llist_insert_tail_good(void);

void test_llist_insert_tail_bad(void);

void test_llist_insert_after_good(void);

void test_llist_insert_after_bad(void);

void test_llist_find_node_good(void);

void test_llist_find_node_bad(void);

void test_llist_remove_node_good(void);

void test_llist_remove_node_bad(void);

void test_llist_delete_node_good(void);

void test_llist_delete_node_bad(void);

void test_llist_size_good(void);

void test_llist_size_bad(void);

void setUp(void)
{
}

void tearDown(void)
{
}
int main(void)
{

    UNITY_BEGIN();

    RUN_TEST(test_llist_create_node_good);

    RUN_TEST(test_llist_insert_head_good);

    RUN_TEST(test_llist_insert_head_bad);

    RUN_TEST(test_llist_insert_tail_good);

    RUN_TEST(test_llist_insert_tail_bad);

    RUN_TEST(test_llist_insert_after_good);

    RUN_TEST(test_llist_insert_after_bad);

    RUN_TEST(test_llist_find_node_good);

    RUN_TEST(test_llist_find_node_bad);

    RUN_TEST(test_llist_remove_node_good);

    RUN_TEST(test_llist_remove_node_bad);

    RUN_TEST(test_llist_delete_node_good);

    RUN_TEST(test_llist_delete_node_bad);

    RUN_TEST(test_llist_size_good);

    RUN_TEST(test_llist_size_bad);

    UNITY_END();

    return 0;
}

/* End of test_all.c */