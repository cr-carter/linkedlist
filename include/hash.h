#ifndef HASH_H
#define HASH_H

#include <stddef.h>

/**
 * @brief Opaque hash table handle.
 *
 * All hash table state is managed internally. Callers interact
 * exclusively through the hashtable_* API; do not access or
 * allocate hashtable_t directly.
 *
 * Keys and values are stored as generic pointers. The caller is
 * responsible for providing a hash function and key comparison
 * function appropriate for the key type being stored.
 */
typedef struct hashtable hashtable_t;

/**
 * @brief Hash function callback.
 *
 * Produces a hash value for a key. The hash table implementation
 * maps the returned value into a bucket index internally.
 *
 * @param[in] p_key  Key to hash.
 *
 * @return Hash value for the key.
 */
typedef size_t (*hashtable_hash_f)(const void *p_key);

/**
 * @brief Key comparison callback.
 *
 * Used to determine whether two keys are equal.
 *
 * @param[in] p_key1  First key.
 * @param[in] p_key2  Second key.
 *
 * @return 0 if keys are equal. Non-zero otherwise.
 */
typedef int (*hashtable_cmp_f)(const void *p_key1, const void *p_key2);

/**
 * @brief Destructor callback for stored values.
 *
 * Called by hashtable_destroy() for each stored value when
 * a non-NULL callback is supplied.
 *
 * @param[in] p_data  Value to destroy.
 */
typedef void (*hashtable_del_f)(void *p_data);

/**
 * @brief Destructor callback for stored keys.
 *
 * Called by hashtable_destroy() for each stored key when
 * a non-NULL callback is supplied.
 *
 * @param[in] p_key  Key to destroy.
 */
typedef void (*hashtable_key_del_f)(void *p_key);

/**
 * @brief Allocate and initialize a hash table. O(n)
 *
 * Allocates the hash table object and bucket array.
 *
 * @param[in] bucket_count Number of buckets to allocate.
 *                         Should be greater than zero.
 * @param[in] hash_func    Hash function for keys.
 * @param[in] cmp_func     Key comparison function.
 *
 * @return Pointer to a new hashtable_t on success, or NULL on failure.
 */
hashtable_t *hashtable_create(size_t bucket_count, hashtable_hash_f hash_func, hashtable_cmp_f cmp_func);

/**
 * @brief Insert a key/value pair into the table. O(1) average
 *
 * If duplicate keys are not permitted by the implementation,
 * insertion fails when the key already exists.
 *
 * @param[in] p_table  Hash table to insert into.
 * @param[in] p_key    Key to store.
 * @param[in] p_data   Value associated with the key.
 *
 * @return 1 on success, -1 on failure.
 */
int hashtable_insert(hashtable_t *p_table, void *p_key, void *p_data);

/**
 * @brief Find the value associated with a key. O(1) average
 *
 * @param[in] p_table  Hash table to search.
 * @param[in] p_key    Key to locate.
 *
 * @return Stored value pointer if found, otherwise NULL.
 */
void *hashtable_find(hashtable_t *p_table, const void *p_key);

/**
 * @brief Remove a key/value pair from the table. O(1) average
 *
 * Removes the entry and returns ownership of the stored value
 * to the caller.
 *
 * The stored key is freed only if the implementation explicitly
 * owns key memory. Otherwise ownership remains with the caller.
 *
 * @param[in] p_table  Hash table to modify.
 * @param[in] p_key    Key to remove.
 *
 * @return Stored value pointer if found, otherwise NULL.
 */
void *hashtable_remove(hashtable_t *p_table, const void *p_key);

/**
 * @brief Return the number of entries stored in the table. O(1)
 *
 * @param[in] p_table  Hash table to measure.
 *
 * @return Entry count, or -1 if @p p_table is NULL.
 */
int hashtable_size(hashtable_t *p_table);

/**
 * @brief Destroy all entries and free the hash table. O(n)
 *
 * Traverses every bucket and chain, invoking the supplied
 * destructor callbacks on stored keys and values when
 * provided.
 *
 * Passing NULL destructor callbacks is valid when the caller
 * manages key and value lifetimes independently.
 *
 * On return, *pp_table is set to NULL.
 *
 * @param[in,out] pp_table       Address of the caller's table pointer.
 * @param[in]     key_del_func   Destructor for keys, or NULL.
 * @param[in]     data_del_func  Destructor for values, or NULL.
 */
void hashtable_destroy(hashtable_t **pp_table, hashtable_key_del_f key_del_func, hashtable_del_f data_del_func);

/**
 * @brief Determine whether a key exists. O(1) average
 *
 * @param[in] p_table  Hash table to search.
 * @param[in] p_key    Key to locate.
 *
 * @return 1 if found, 0 if not found, -1 on error.
 */
int hashtable_contains(hashtable_t *p_table, const void *p_key);

#endif /* HASH_H */

/* End of file hash.h */
