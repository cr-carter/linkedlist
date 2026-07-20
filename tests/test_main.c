#include "unity.h"

// test_hash.c
void test_int_hashtable(void);
void test_char_hashtable(void);
void test_string_hashtable(void);

// test_qsort.c
void test_quicksort_random(void);
void test_quicksort_sorted(void);
void test_quicksort_backwards(void);
void test_quicksort_large(void);
void test_quicksort_all_zeros(void);
void test_quicksort_single(void);
void test_quicksort_bad(void);

void test_mergesort_random(void);
void test_mergesort_sorted(void);
void test_mergesort_backwards(void);
void test_mergesort_large(void);
void test_mergesort_all_zeros(void);
void test_mergesort_single(void);
void test_mergesort_bad(void);

// test_pqueue.c
void test_pqueue_random(void);
void test_pqueue_sorted(void);
void test_pqueue_backwards(void);
void test_pqueue_large(void);
void test_pqueue_all_zeros(void);
void test_pqueue_single(void);
void test_pqueue_bad(void);

// test_trie.c
void test_trie(void);

// test_graph.c
void test_graph_search(void);

void setUp(void)
{
}

void tearDown(void)
{
}
int main(void)
{

    UNITY_BEGIN();

    // test_hash_generic.c
    {
        printf(
            "\n\n##############################\nTesting hash_generic.c functions\n##############################\n");
        // RUN_TEST(test_int_hashtable);
        // printf("\n");
        // RUN_TEST(test_char_hashtable);
    }

    // test_hash_string.c
    {
        printf("\n\n##############################\nTesting hash_string.c functions\n##############################\n");
        // RUN_TEST(test_string_hashtable);
    }

    // test_qsort.c
    {
        printf("\n\n##############################\nTesting qsort.c functions\n##############################\n");
        // RUN_TEST(test_quicksort_random);
        // RUN_TEST(test_quicksort_sorted);
        // RUN_TEST(test_quicksort_large);
        // RUN_TEST(test_quicksort_backwards);
        // RUN_TEST(test_quicksort_all_zeros);
        // RUN_TEST(test_quicksort_single);
        // RUN_TEST(test_quicksort_bad);

        // printf("Test mergesort\n");
        // RUN_TEST(test_mergesort_random);
        // RUN_TEST(test_mergesort_sorted);
        // RUN_TEST(test_mergesort_large);
        // RUN_TEST(test_mergesort_backwards);
        // RUN_TEST(test_mergesort_all_zeros);
        // RUN_TEST(test_mergesort_single);
        // RUN_TEST(test_mergesort_bad);
    }

    // test_pqueue.c
    {
        // printf("\n\n##############################\nTesting pqueue.c functions\n##############################\n");
        // RUN_TEST(test_pqueue_random);
        // RUN_TEST(test_pqueue_sorted);
        // RUN_TEST(test_pqueue_backwards);
        // RUN_TEST(test_pqueue_large);
        // RUN_TEST(test_pqueue_all_zeros);
        // RUN_TEST(test_pqueue_single);
        // RUN_TEST(test_pqueue_bad);
    }

    // test_trie.c
    {
        // printf("\n\n##############################\nTesting trie.c functions\n##############################\n");
        // RUN_TEST(test_trie);
    }

    // test_graph.c
    {
        printf("\n\n##############################\nTesting graph.c functions\n##############################\n");
        RUN_TEST(test_graph_search);
    }

    UNITY_END();

    return 0;
}

/* End of test_all.c */

// AESD 866-335-ARMY
// Calling...

// CVTE 706-787-7266
