/**
 * @file hash.h
 * @author Chase Carter
 */

#ifndef HASH_H
#define HASH_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Opaque hash table type.
 *
 * All implementation details are hidden from the caller.
 * Keys and values are stored as generic pointers.
 */
typedef struct hashtable hashtable_t;

/**
 * @brief Hash function callback.
 *
 * Computes a hash value for a key.
 *
 * @param[in] p_key Key to hash.
 *
 * @return Hash value.
 */
typedef size_t (*hash_func)(const void *p_key);

/**
 * @brief Key comparison callback.
 *
 * Compares two keys.
 *
 * @param[in] p_key1 First key.
 * @param[in] p_key2 Second key.
 *
 * @return 0 if equal, non-zero otherwise.
 */
typedef int (*comp_func)(const void *p_key1, const void *p_key2);

/**
 * @brief Destructor callback for stored keys.
 *
 * @param[in] p_key Key to destroy.
 */
typedef void (*key_del_func)(void *p_key);

/**
 * @brief Destructor callback for stored values.
 *
 * @param[in] p_value Value to destroy.
 */
typedef void (*value_del_func)(void *p_value);

/**
 * @brief Callback used by hashtable_iterate().
 *
 * @param[in] p_key User key.
 * @param[in] p_value User value.
 */
typedef void (*iter_func)(const void *p_key, void *p_value);

/**
 * @brief Create a new hash table.
 *
 * @param[in] bucket_count Initial number of buckets.
 * @param[in] hash Hash function.
 * @param[in] compare Key comparison function.
 *
 * @return New hash table, or NULL on failure.
 */
hashtable_t *hashtable_create(size_t bucket_count, hash_func hash, comp_func compare);

/**
 * @brief Insert or update a key/value pair.
 *
 * If the key already exists, its associated value is replaced.
 *
 * @param[in,out] p_ht Hash table.
 * @param[in] p_key Key.
 * @param[in] p_value Value.
 *
 * @return 0 on success, 1 on failure, 2 if key is already being used.
 */
int hashtable_insert(hashtable_t *p_ht, const void *p_key, void *p_value);

/**
 * @brief Find the value associated with a key.
 *
 * @param[in] p_ht Hash table.
 * @param[in] p_key Key to locate.
 *
 * @return Associated value, or NULL if not found.
 */
void *hashtable_find(hashtable_t *p_ht, const void *p_key);

/**
 * @brief Remove a key/value pair.
 *
 * Removes the entry and returns the stored value.
 * Ownership of the returned value is transferred
 * to the caller.
 *
 * @param[in,out] p_ht Hash table.
 * @param[in] p_key Key to remove.
 *
 * @return Stored value, or NULL if the key is absent.
 */
void *hashtable_remove(hashtable_t *p_ht, const void *p_key);

/**
 * @brief Remove every entry from the table.
 *
 * The bucket array remains allocated.
 *
 * @param[in,out] p_ht Hash table.
 * @param[in] key_del Optional key destructor.
 * @param[in] value_del Optional value destructor.
 */
void hashtable_clear(hashtable_t *p_ht, key_del_func key_del, value_del_func value_del);

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
void hashtable_destroy(hashtable_t **pp_ht, key_del_func key_del, value_del_func value_del);

/**
 * @brief Determine whether a key exists.
 *
 * @param[in] p_ht Hash table.
 * @param[in] p_key Key to search for.
 *
 * @retval true Key exists.
 * @retval false Key does not exist.
 */
bool hashtable_contains(hashtable_t *p_ht, const void *p_key);

/**
 * @brief Return the number of stored entries.
 *
 * @param[in] p_ht Hash table.
 *
 * @return Number of key/value pairs.
 */
size_t hashtable_size(const hashtable_t *p_ht);

/**
 * @brief Visit every key/value pair in the table.
 *
 * Iteration order is implementation-defined.
 *
 * @param[in] p_ht Hash table.
 * @param[in] iter Callback invoked for each entry.
 */
void hashtable_iterate(hashtable_t *p_ht, iter_func iter);

#endif /* HASH_H */
