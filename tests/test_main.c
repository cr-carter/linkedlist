#include "unity.h"

// test_avl.c
void test_avl_create_tree(void);
void test_avl_insert(void);
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

// test_ssl.c
void test_sll_create(void);
void test_sll_destroy(void);
void test_sll_insert_head_good(void);
void test_sll_insert_head_bad(void);
void test_sll_insert_tail_good(void);
void test_sll_insert_tail_bad(void);
void test_sll_insert_at_good(void);
void test_sll_insert_at_bad(void);
void test_sll_index_of_good(void);
void test_sll_index_of_bad(void);
void test_sll_size_good(void);
void test_sll_size_bad(void);

// test_dll.c
void test_dll_create(void);
void test_dll_insert_head(void);
void test_dll_insert_tail(void);
void test_dll_remove_head(void);
void test_dll_remove_tail(void);
void test_dll_destroy(void);
void test_dll_size(void);
void test_dll_iterate(void);
void test_dll_selection_sort(void);
void test_dll_insertion_sort(void);
void test_dll_iter_create(void);
void test_dll_iter_next(void);
void test_dll_iter_prev(void);
void test_dll_iter_data(void);
void test_dll_iter_destroy(void);
void test_dll_iter_reset(void);

void setUp(void)
{
}

void tearDown(void)
{
}
int main(void)
{

    UNITY_BEGIN();

    // test_avl.c
    {
        printf("\n##############################\nTesting avl.c functions\n##############################\n");
        RUN_TEST(test_avl_create_tree);
        RUN_TEST(test_avl_destroy_tree);
        RUN_TEST(test_avl_clear);
        RUN_TEST(test_avl_delete_node);
        RUN_TEST(test_avl_insert);
        RUN_TEST(test_avl_find);
        RUN_TEST(test_avl_get_root);
        RUN_TEST(test_avl_minimum_value);
        RUN_TEST(test_avl_maximum_value);
        RUN_TEST(test_avl_size_of_tree);
        RUN_TEST(test_avl_is_empty);
    }

    // test_sll.c
    {
        printf("\n\n##############################\nTesting sll.c functions\n##############################\n");
        RUN_TEST(test_sll_create);
        RUN_TEST(test_sll_destroy);
        RUN_TEST(test_sll_insert_head_good);
        RUN_TEST(test_sll_insert_head_bad);
        RUN_TEST(test_sll_insert_tail_good);
        RUN_TEST(test_sll_insert_tail_bad);
        RUN_TEST(test_sll_insert_at_good);
        RUN_TEST(test_sll_insert_at_bad);
        RUN_TEST(test_sll_index_of_good);
        RUN_TEST(test_sll_index_of_bad);
        RUN_TEST(test_sll_size_good);
        RUN_TEST(test_sll_size_bad);
    }

    // test_dll.c
    {
        printf("\n\n##############################\nTesting dll.c functions\n##############################\n");
        RUN_TEST(test_dll_create);
        RUN_TEST(test_dll_insert_head);
        RUN_TEST(test_dll_insert_tail);
        RUN_TEST(test_dll_destroy);
        RUN_TEST(test_dll_size);
        RUN_TEST(test_dll_iterate);
        RUN_TEST(test_dll_selection_sort);
        RUN_TEST(test_dll_insertion_sort);
        RUN_TEST(test_dll_iter_create);
        RUN_TEST(test_dll_iter_next);
        RUN_TEST(test_dll_iter_prev);
        RUN_TEST(test_dll_iter_data);
        RUN_TEST(test_dll_iter_destroy);
        RUN_TEST(test_dll_iter_reset);
    }

    UNITY_END();

    return 0;
}

/* End of test_all.c */