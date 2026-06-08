#include "unity.h"

// test_args
void test_llist_create(void);

void test_llist_destroy(void);

void test_llist_insert_head_good(void);

void test_llist_insert_head_bad(void);

void test_llist_insert_tail_good(void);

void test_llist_insert_tail_bad(void);

void test_llist_insert_at_good(void);

void test_llist_insert_at_bad(void);

void test_llist_index_of_good(void);

void test_llist_index_of_bad(void);

void setUp(void)
{
}

void tearDown(void)
{
}
int main(void)
{

    UNITY_BEGIN();

    RUN_TEST(test_llist_create);

    RUN_TEST(test_llist_destroy);

    RUN_TEST(test_llist_insert_head_good);

    RUN_TEST(test_llist_insert_head_bad);

    RUN_TEST(test_llist_insert_tail_good);

    RUN_TEST(test_llist_insert_tail_bad);

    RUN_TEST(test_llist_insert_at_good);

    RUN_TEST(test_llist_insert_at_bad);

    RUN_TEST(test_llist_index_of_good);

    UNITY_END();

    return 0;
}

/* End of test_all.c */