#define _POSIX_C_SOURCE 200809L

#include "hash_generic.h"
#include "unity.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t jenkins_hash(const void *key, size_t len)
{
    // Taken from https://en.wikipedia.org/wiki/Jenkins_hash_function
    const uint8_t *data = key;
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

size_t int_hash(const void *key)
{
    uint32_t key_as_int = *(const uint32_t *)key;
    return jenkins_hash(&key_as_int, sizeof(key_as_int));
}

int int_compare(const void *val_a, const void *val_b)
{
    uint32_t int_a = *(const uint32_t *)val_a;
    uint32_t int_b = *(const uint32_t *)val_b;
    return (int_a > int_b) - (int_a < int_b);
}

void int_iter(const void *p_key, void *p_value)
{
    printf("%i: %c\n", *(int *)p_key, *(char *)p_value);
}

void *int_copy(const void *p_key)
{
    void *p_copy = calloc(1, sizeof(int));
    memcpy(p_copy, p_key, sizeof(int));
    return p_copy;
}

void int_delete(void *p_key)
{
    free(p_key);
}

size_t char_hash(const void *key)
{
    return jenkins_hash(key, strlen(key));
}

int char_compare(const void *val_a, const void *val_b)
{
    return strcmp(val_a, val_b);
}

void char_iter(const void *p_key, void *p_value)
{
    printf("%s: %i\n", (char *)p_key, *(int *)p_value);
}

void *char_copy(const void *p_key)
{
    return strdup(p_key);
}

void char_delete(void *p_key)
{
    free(p_key);
}

// hashtable_t *hashtable_create(size_t bucket_count, hash_func hash, comp_func compare);
// int hashtable_insert(hashtable_t *p_ht, const void *p_key, void *p_value);
// void *hashtable_find(hashtable_t *p_ht, const void *p_key);
// size_t hashtable_size(const hashtable_t *p_ht);
// void *hashtable_remove(hashtable_t *p_ht, const void *p_key);
// bool hashtable_contains(hashtable_t *p_ht, const void *p_key);
// void hashtable_iterate(hashtable_t *p_ht, iter_func iter, void *p_user_data);
// void hashtable_clear(hashtable_t *p_ht, key_del_func key_del, value_del_func value_del);
// void hashtable_destroy(hashtable_t **pp_ht, key_del_func key_del, value_del_func value_del);

void test_int_hashtable(void)
{
    hashtable_t *p_ht = hashtable_create(5, int_hash, int_compare, int_copy, int_delete);

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

    char *char_1 = calloc(1, sizeof(*char_1));
    char *char_2 = calloc(1, sizeof(*char_2));
    char *char_3 = calloc(1, sizeof(*char_3));
    char *char_4 = calloc(1, sizeof(*char_4));
    char *char_5 = calloc(1, sizeof(*char_5));
    char *char_6 = calloc(1, sizeof(*char_6));
    char *char_7 = calloc(1, sizeof(*char_7));
    char *char_8 = calloc(1, sizeof(*char_8));
    char *char_9 = calloc(1, sizeof(*char_9));
    char *char_10 = calloc(1, sizeof(*char_10));
    char *char_11 = calloc(1, sizeof(*char_11));
    char *char_12 = calloc(1, sizeof(*char_12));
    char *char_13 = calloc(1, sizeof(*char_13));

    *char_1 = 'a';
    *char_2 = 'b';
    *char_3 = 'c';
    *char_4 = 'd';
    *char_5 = 'e';
    *char_6 = 'f';
    *char_7 = 'g';
    *char_8 = 'h';
    *char_9 = 'i';
    *char_10 = 'j';
    *char_11 = 'k';
    *char_12 = 'l';
    *char_13 = 'm';

    hashtable_insert(p_ht, int_1, char_1);
    hashtable_insert(p_ht, int_2, char_2);
    hashtable_insert(p_ht, int_3, char_3);
    hashtable_insert(p_ht, int_4, char_4);
    hashtable_insert(p_ht, int_5, char_5);
    hashtable_insert(p_ht, int_6, char_6);

    printf("Check table size, search for a value...\n");
    TEST_ASSERT_EQUAL(6, hashtable_size(p_ht));
    char *result = hashtable_find(p_ht, int_5);
    TEST_ASSERT_EQUAL('e', *result);

    hashtable_insert(p_ht, int_7, char_7);
    hashtable_insert(p_ht, int_8, char_8);
    hashtable_insert(p_ht, int_9, char_9);
    hashtable_insert(p_ht, int_10, char_10);
    hashtable_insert(p_ht, int_11, char_11);
    hashtable_insert(p_ht, int_12, char_12);
    hashtable_insert(p_ht, int_13, char_13);

    printf("Check table size, remove a value by key...\n");
    result = hashtable_remove(p_ht, int_12);
    TEST_ASSERT_EQUAL(12, hashtable_size(p_ht));
    TEST_ASSERT_EQUAL('l', *result);

    printf("Check if table contains value...\n");
    bool check = hashtable_contains(p_ht, int_12);
    TEST_ASSERT_FALSE(check);
    check = hashtable_contains(p_ht, int_13);
    TEST_ASSERT_TRUE(check);

    printf("Check iteration...\n");
    hashtable_iterate(p_ht, int_iter);

    printf("Check clearing...\n");
    hashtable_clear(p_ht, NULL);
    TEST_ASSERT_EQUAL(0, hashtable_size(p_ht));
    check = hashtable_contains(p_ht, int_1);
    TEST_ASSERT_FALSE(check);
    check = hashtable_contains(p_ht, int_2);
    TEST_ASSERT_FALSE(check);
    check = hashtable_contains(p_ht, int_3);
    TEST_ASSERT_FALSE(check);
    check = hashtable_contains(p_ht, int_4);
    TEST_ASSERT_FALSE(check);
    TEST_ASSERT_NOT_NULL(p_ht);

    printf("Check destroy...\n");
    hashtable_destroy(&p_ht, NULL);
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

    free(char_1);
    free(char_2);
    free(char_3);
    free(char_4);
    free(char_5);
    free(char_6);
    free(char_7);
    free(char_8);
    free(char_9);
    free(char_10);
    free(char_11);
    free(char_12);
    free(char_13);
}

void test_char_hashtable(void)
{
    hashtable_t *p_ht = hashtable_create(5, char_hash, char_compare, char_copy, char_delete);

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

    hashtable_insert(p_ht, char_1, int_1);
    hashtable_insert(p_ht, char_2, int_2);
    hashtable_insert(p_ht, char_3, int_3);
    hashtable_insert(p_ht, char_4, int_4);
    hashtable_insert(p_ht, char_5, int_5);
    hashtable_insert(p_ht, char_6, int_6);

    printf("Check table size, search for a value...\n");
    TEST_ASSERT_EQUAL(6, hashtable_size(p_ht));
    int *result = hashtable_find(p_ht, char_5);
    TEST_ASSERT_EQUAL(5, *result);

    hashtable_insert(p_ht, char_7, int_7);
    hashtable_insert(p_ht, char_8, int_8);
    hashtable_insert(p_ht, char_9, int_9);
    hashtable_insert(p_ht, char_10, int_10);
    hashtable_insert(p_ht, char_11, int_11);
    hashtable_insert(p_ht, char_12, int_12);
    hashtable_insert(p_ht, char_13, int_13);

    printf("Check table size, remove a value by key...\n");
    result = hashtable_remove(p_ht, char_12);
    TEST_ASSERT_EQUAL(12, hashtable_size(p_ht));
    TEST_ASSERT_EQUAL(12, *result);

    printf("Check if table contains value...\n");
    bool check = hashtable_contains(p_ht, char_12);
    TEST_ASSERT_FALSE(check);
    check = hashtable_contains(p_ht, char_13);
    TEST_ASSERT_TRUE(check);

    printf("Check iteration...\n");
    hashtable_iterate(p_ht, char_iter);

    printf("Check clearing...\n");
    hashtable_clear(p_ht, NULL);
    TEST_ASSERT_EQUAL(0, hashtable_size(p_ht));
    check = hashtable_contains(p_ht, char_1);
    TEST_ASSERT_FALSE(check);
    check = hashtable_contains(p_ht, char_2);
    TEST_ASSERT_FALSE(check);
    check = hashtable_contains(p_ht, char_3);
    TEST_ASSERT_FALSE(check);
    check = hashtable_contains(p_ht, char_4);
    TEST_ASSERT_FALSE(check);
    TEST_ASSERT_NOT_NULL(p_ht);

    printf("Check destroy...\n");
    hashtable_destroy(&p_ht, NULL);
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
