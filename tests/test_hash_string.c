#define _POSIX_C_SOURCE 200809L

#include "hash_string.h"
#include "unity.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t ooat_hash(const char *key, size_t len)
{
    // Taken from https://en.wikipedia.org/wiki/ooat_hash_function
    const uint8_t *data = (void *)key;
    size_t hash = 0;

    for (size_t index = 0; index < len; index++)
    {
        hash += data[index];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}

size_t string_hash(const char *key)
{
    return ooat_hash(key, strlen(key));
}

void string_iter(const char *p_key, void *p_value)
{
    printf("%s: %i\n", p_key, *(int *)p_value);
}

void test_string_hashtable(void)
{
    string_hashtable_t *p_ht = hash_string_create(5, string_hash);

    TEST_ASSERT_NOT_NULL(p_ht);

    int *int_1 = calloc(1, sizeof(*int_1));
    int *int_2 = calloc(1, sizeof(*int_2));
    int *int_3 = calloc(1, sizeof(*int_3));
    int *int_4 = calloc(1, sizeof(*int_4));
    int *int_5 = calloc(1, sizeof(*int_5));
    int *int_6 = calloc(1, sizeof(*int_6));
    int *int_7 = calloc(1, sizeof(*int_7));
    int *int_8 = calloc(1, sizeof(*int_8));
    int *int_9 = calloc(1, sizeof(*int_9));
    int *int_10 = calloc(1, sizeof(*int_10));
    int *int_11 = calloc(1, sizeof(*int_11));
    int *int_12 = calloc(1, sizeof(*int_12));
    int *int_13 = calloc(1, sizeof(*int_13));

    *int_1 = 1;
    *int_2 = 2;
    *int_3 = 3;
    *int_4 = 4;
    *int_5 = 5;
    *int_6 = 6;
    *int_7 = 7;
    *int_8 = 8;
    *int_9 = 9;
    *int_10 = 10;
    *int_11 = 11;
    *int_12 = 12;
    *int_13 = 13;

    char *char_1 = (char *)"apple";
    char *char_2 = (char *)"banana";
    char *char_3 = (char *)"cherry";
    char *char_4 = (char *)"dog";
    char *char_5 = (char *)"elephant";
    char *char_6 = (char *)"fish";
    char *char_7 = (char *)"golf";
    char *char_8 = (char *)"hockey";
    char *char_9 = (char *)"igloo";
    char *char_10 = (char *)"jump";
    char *char_11 = (char *)"koala";
    char *char_12 = (char *)"logic";
    char *char_13 = (char *)"money";

    hash_string_insert(p_ht, char_1, int_1);
    hash_string_insert(p_ht, char_2, int_2);
    hash_string_insert(p_ht, char_3, int_3);
    hash_string_insert(p_ht, char_4, int_4);
    hash_string_insert(p_ht, char_5, int_5);
    hash_string_insert(p_ht, char_6, int_6);

    printf("Check table size, search for a value...\n");
    TEST_ASSERT_EQUAL(6, hash_string_size(p_ht));
    int *result = hash_string_find(p_ht, char_5);
    TEST_ASSERT_EQUAL(5, *result);

    hash_string_insert(p_ht, char_7, int_7);
    hash_string_insert(p_ht, char_8, int_8);
    hash_string_insert(p_ht, char_9, int_9);
    hash_string_insert(p_ht, char_10, int_10);
    hash_string_insert(p_ht, char_11, int_11);
    hash_string_insert(p_ht, char_12, int_12);
    hash_string_insert(p_ht, char_13, int_13);

    printf("Check table size, remove a value by key...\n");
    result = hash_string_remove(p_ht, char_12);
    TEST_ASSERT_EQUAL(12, hash_string_size(p_ht));
    TEST_ASSERT_EQUAL(12, *result);

    printf("Check if table contains value...\n");
    bool check = hash_string_contains(p_ht, char_12);
    TEST_ASSERT_FALSE(check);
    check = hash_string_contains(p_ht, char_13);
    TEST_ASSERT_TRUE(check);

    printf("Check iteration...\n");
    hash_string_iterate(p_ht, string_iter);

    printf("Check clearing...\n");
    hash_string_clear(p_ht, NULL);
    TEST_ASSERT_EQUAL(0, hash_string_size(p_ht));
    check = hash_string_contains(p_ht, char_1);
    TEST_ASSERT_FALSE(check);
    check = hash_string_contains(p_ht, char_2);
    TEST_ASSERT_FALSE(check);
    check = hash_string_contains(p_ht, char_3);
    TEST_ASSERT_FALSE(check);
    check = hash_string_contains(p_ht, char_4);
    TEST_ASSERT_FALSE(check);
    TEST_ASSERT_NOT_NULL(p_ht);

    printf("Check destroy...\n");
    hash_string_destroy(&p_ht, NULL);
    TEST_ASSERT_NULL(p_ht);

    free(int_1);
    free(int_2);
    free(int_3);
    free(int_4);
    free(int_5);
    free(int_6);
    free(int_7);
    free(int_8);
    free(int_9);
    free(int_10);
    free(int_11);
    free(int_12);
    free(int_13);

    // free(char_1);
    // free(char_2);
    // free(char_3);
    // free(char_4);
    // free(char_5);
    // free(char_6);
    // free(char_7);
    // free(char_8);
    // free(char_9);
    // free(char_10);
    // free(char_11);
    // free(char_12);
    // free(char_13);
}

/* End of test_hash.c */
