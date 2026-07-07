/**
 * @file hash_string.c
 * @author Chase Carter
 */

// NOLINTNEXTLINE(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp,-warnings-as-errors)
#define _POSIX_C_SOURCE 200809L

#include "hash_string.h"
#include "hash_generic.h"

#include <stdlib.h>
#include <string.h>

static void *static_string_copy(const void *p_key)
{
    return strdup((const char *)p_key);
}

static int static_string_compare(const void *p_key1, const void *p_key2)
{
    return strcmp((const char *)p_key1, (const char *)p_key2);
}

string_hashtable_t *hash_string_create(size_t bucket_count, string_hash_func hash)
{
    return (string_hashtable_t *)hashtable_create(bucket_count, (hash_func)hash, static_string_compare,
                                                  static_string_copy, free);
}

int hash_string_insert(string_hashtable_t *p_ht, const char *p_key, void *p_value)
{
    return hashtable_insert((hashtable_t *)p_ht, p_key, p_value);
}

void *hash_string_find(string_hashtable_t *p_ht, const char *p_key)
{
    return hashtable_find((hashtable_t *)p_ht, p_key);
}

void *hash_string_remove(string_hashtable_t *p_ht, const char *p_key)
{
    return hashtable_remove((hashtable_t *)p_ht, p_key);
}

void hash_string_destroy(string_hashtable_t **pp_ht, value_del_func value_del)
{
    hashtable_destroy((hashtable_t **)pp_ht, value_del);
}

void hash_string_clear(string_hashtable_t *p_ht, value_del_func value_del)
{
    hashtable_clear((hashtable_t *)p_ht, value_del);
}

bool hash_string_contains(string_hashtable_t *p_ht, const char *p_key)
{
    return hashtable_contains((hashtable_t *)p_ht, p_key);
}

size_t hash_string_size(const string_hashtable_t *p_ht)
{
    return hashtable_size((const hashtable_t *)p_ht);
}

void hash_string_iterate(string_hashtable_t *p_ht, string_iter_func iterate)
{
    hashtable_iterate((hashtable_t *)p_ht, (iter_func)iterate);
}

/* End of file hash_string.c */
