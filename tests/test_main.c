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
        RUN_TEST(test_int_hashtable);
        printf("\n");
        RUN_TEST(test_char_hashtable);
    }

    // test_hash_string.c
    {
        printf("\n\n##############################\nTesting hash_string.c functions\n##############################\n");
        RUN_TEST(test_string_hashtable);
    }

    // test_qsort.c
    {
        printf("\n\n##############################\nTesting qsort.c functions\n##############################\n");
        RUN_TEST(test_quicksort_random);
        RUN_TEST(test_quicksort_sorted);
        RUN_TEST(test_quicksort_large);
        RUN_TEST(test_quicksort_backwards);
        RUN_TEST(test_quicksort_all_zeros);
        RUN_TEST(test_quicksort_single);
        RUN_TEST(test_quicksort_bad);
    }

    UNITY_END();

    return 0;
}

/* End of test_all.c */