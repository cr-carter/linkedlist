#include "unity.h"

// test_hash.c
void test_int_hashtable(void);
void test_char_hashtable(void);

void setUp(void)
{
}

void tearDown(void)
{
}
int main(void)
{

    UNITY_BEGIN();

    // test_hash.c
    {
        printf("\n\n##############################\nTesting hash.c functions\n##############################\n");
        RUN_TEST(test_int_hashtable);
        printf("\n");
        RUN_TEST(test_char_hashtable);
    }

    UNITY_END();

    return 0;
}

/* End of test_all.c */