/**
 * @file hash_string.h
 * @author Chase Carter
 */

#ifndef HASH_STRING_H
#define HASH_STRING_H

#include <stdbool.h>
#include <stddef.h>

typedef struct hashtable string_hashtable_t;

typedef size_t (*string_hash_func)(const char *);

typedef void (*string_value_del_func)(void *);

typedef void (*string_iter_func)(const char *, void *);

/**
 * @brief Create a new hash table.
 *
 * @param[in] bucket_count Initial number of buckets.
 * @param[in] hash Hash function.
 *
 * @return New hash table, or NULL on failure.
 */
string_hashtable_t *hash_string_create(size_t bucket_count, string_hash_func hash);

/**
 * @brief Insert or update a key/value pair.
 *
 * If the key already exists, the new value is not stored.
 *
 * @param[in,out] p_ht Hash table.
 * @param[in] p_key Key to be copied, caller controls original key.
 * @param[in] p_value Value to be stored, caller maintains ownership of value.
 *
 * @return 0 on success, 1 on failure, 2 if key is already being used.
 */
int hash_string_insert(string_hashtable_t *p_ht, const char *p_key, void *p_value);

/**
 * @brief Find the value associated with a key.
 *
 * @param[in] p_ht Hash table.
 * @param[in] p_key Key to locate.
 *
 * @return Associated value, or NULL if not found.
 */
void *hash_string_find(string_hashtable_t *p_ht, const char *p_key);

/**
 * @brief Remove a key/value pair.
 *
 * Removes the entry and returns the stored value.
 * Ownership of the returned value is transferred
 * to the caller.
 *
 * @param[in,out] p_ht Hash table.
 * @param[in] p_key Key to remove. The copy stored in the hashtable will be destroyed, caller maintains ownership of
 * passed in key.
 *
 * @return Stored value, or NULL if the key is absent.
 */
void *hash_string_remove(string_hashtable_t *p_ht, const char *p_key);

/**
 * @brief Remove every entry from the table.
 *
 * The bucket array remains allocated. This function will destroy all copies of keys stored inside of it, and will
 * destroy values associated with them if the caller passes a function to destroy the values.
 *
 * @param[in,out] p_ht Hash table.
 * @param[in] value_del Optional value destructor.
 */
void hash_string_clear(string_hashtable_t *p_ht, string_value_del_func value_del);

/**
 * @brief Destroy a hash table.
 *
 * Frees every node, every bucket, and the table itself.
 * Optional destructor callbacks are invoked on stored
 * keys and values before they are discarded.
 *
 * On return, *pp_ht is NULL.
 *
 * @param[in,out] pp_ht Address of table pointer.
 * @param[in] key_del Optional key destructor.
 * @param[in] value_del Optional value destructor.
 */
void hash_string_destroy(string_hashtable_t **pp_ht, string_value_del_func value_del);

/**
 * @brief Determine whether a key exists.
 *
 * @param[in] p_ht Hash table.
 * @param[in] p_key Key to search for.
 *
 * @retval true Key exists.
 * @retval false Key does not exist.
 */
bool hash_string_contains(string_hashtable_t *p_ht, const char *p_key);

/**
 * @brief Return the number of stored entries.
 *
 * @param[in] p_ht Hash table.
 *
 * @return Number of key/value pairs.
 */
size_t hash_string_size(const string_hashtable_t *p_ht);

/**
 * @brief Visit every key/value pair in the table.
 *
 * Iteration order is implementation-defined.
 *
 * @param[in] p_ht Hash table.
 * @param[in] iter Callback invoked for each entry.
 */
void hash_string_iterate(string_hashtable_t *p_ht, string_iter_func iter);

#endif /* HASH_STRING_H */
