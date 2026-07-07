/**
 * @file hash.c
 * @author Chase Carter
 */

#include "hash.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct node node_t;

struct node
{
    void *p_key;
    void *p_value;
    node_t *p_next;
    node_t *p_previous;
};

typedef struct bucket
{
    node_t *head;
    node_t *tail;
} bucket_t;

struct hashtable
{
    bucket_t **pp_buckets;
    size_t bucket_count;
    size_t size;
    hash_func hash;
    key_comp_func key_compare;
    key_cpy_func key_copy;
    key_del_func key_delete;
};

/* Static functions */

static size_t static_next_prime(size_t p_current);

static bool static_resize_if_needed(hashtable_t *p_ht);

static node_t *static_find_node(hashtable_t *p_ht, const void *p_key);

static size_t static_bucket_index(const hashtable_t *p_ht, const void *p_key);

static int static_insert_without_resizing(hashtable_t *p_ht, const void *p_key, void *p_value);

/* Public functions*/

hashtable_t *hashtable_create(size_t bucket_count, hash_func hash, key_comp_func key_compare, key_cpy_func key_copy,
                              key_del_func key_del)
{
    hashtable_t *retval = NULL;

    // Validate input, allocate table
    if ((NULL == hash) || (NULL == key_compare) || (0 == bucket_count) || (NULL == key_copy) || (NULL == key_del))
    {
        return NULL;
    }

    retval = calloc(1, sizeof(*retval));

    // Validate table allocation, allocate array of buckets
    if (NULL == retval)
    {
        return NULL;
    }

    retval->pp_buckets = calloc(bucket_count, sizeof(bucket_t *));

    // Validate array, allocate buckets
    if (NULL == retval->pp_buckets)
    {
        free(retval);
        return NULL;
    }

    for (size_t index = 0; index < bucket_count; index++)
    {
        retval->pp_buckets[index] = calloc(1, sizeof(*retval->pp_buckets[index]));

        if (NULL == retval->pp_buckets[index])
        {
            for (size_t bucket = 0; bucket < index; bucket++)
            {
                free(retval->pp_buckets[bucket]);
            }

            free(retval->pp_buckets);
            free(retval);
            return NULL;
        }
    }

    retval->bucket_count = bucket_count;
    retval->key_compare = key_compare;
    retval->key_copy = key_copy;
    retval->key_delete = key_del;
    retval->hash = hash;

    return retval;
}

int hashtable_insert(hashtable_t *p_ht, const void *p_key, void *p_value)
{
    if ((NULL == p_ht) || (NULL == p_key) || (NULL == p_value))
    {
        return EXIT_FAILURE;
    }
    void *p_copy = p_ht->key_copy(p_key);

    if (NULL == p_copy)
    {
        return EXIT_FAILURE;
    }

    int retval = static_insert_without_resizing(p_ht, p_copy, p_value);

    if (EXIT_SUCCESS == retval)
    {
        static_resize_if_needed(p_ht);
    }

    if (2 == retval)
    {
        p_ht->key_delete(p_copy);
    }

    return retval;
}

void *hashtable_find(hashtable_t *p_ht, const void *p_key)
{
    void *p_retval = NULL;

    if ((NULL == p_ht) || (NULL == p_key))
    {
        return NULL;
    }

    node_t *p_node = static_find_node(p_ht, p_key);

    if (NULL != p_node)
    {
        p_retval = p_node->p_value;
    }

    return p_retval;
}

void *hashtable_remove(hashtable_t *p_ht, const void *p_key)
{

    if ((NULL == p_ht) || (NULL == p_key))
    {
        return NULL;
    }

    node_t *p_node = static_find_node(p_ht, p_key);

    if (NULL == p_node)
    {
        return NULL;
    }

    size_t bucket_num = static_bucket_index(p_ht, p_key);

    bucket_t *p_bucket = p_ht->pp_buckets[bucket_num];

    void *p_value = p_node->p_value;

    if ((p_node == p_bucket->head) && (p_node == p_bucket->tail))
    {
        p_bucket->head = NULL;
        p_bucket->tail = NULL;
    }
    else if (p_node == p_bucket->head)
    {
        p_bucket->head = p_node->p_next;
        p_bucket->head->p_previous = NULL;
    }
    else if (p_node == p_bucket->tail)
    {
        p_bucket->tail = p_node->p_previous;
        p_bucket->tail->p_next = NULL;
    }
    else
    {
        p_node->p_previous->p_next = p_node->p_next;
        p_node->p_next->p_previous = p_node->p_previous;
    }
    p_ht->key_delete(p_node->p_key);

    free(p_node);

    p_ht->size -= 1;

    return p_value;
}

void hashtable_destroy(hashtable_t **pp_ht, value_del_func value_del)
{
    if ((NULL == pp_ht) || (NULL == *pp_ht))
    {
        return;
    }

    hashtable_t *p_ht = *pp_ht;

    hashtable_clear(p_ht, value_del);

    for (size_t index = 0; index < p_ht->bucket_count; index++)
    {
        bucket_t *p_bucket = p_ht->pp_buckets[index];

        free(p_bucket);
    }

    free(p_ht->pp_buckets);
    free(p_ht);

    *pp_ht = NULL;
}

size_t hashtable_size(const hashtable_t *p_ht)
{
    if (NULL == p_ht)
    {
        return 0;
    }

    return p_ht->size;
}

void hashtable_clear(hashtable_t *p_ht, value_del_func value_del)
{
    if (NULL == p_ht)
    {
        return;
    }

    for (size_t index = 0; index < p_ht->bucket_count; index++)
    {
        bucket_t *p_bucket = p_ht->pp_buckets[index];

        node_t *p_current = p_bucket->head;

        while (NULL != p_current)
        {
            node_t *p_next = p_current->p_next;

            p_ht->key_delete((void *)p_current->p_key);

            if (NULL != value_del)
            {
                value_del(p_current->p_value);
            }

            free(p_current);

            p_current = p_next;
        }

        p_bucket->head = NULL;
        p_bucket->tail = NULL;
    }

    p_ht->size = 0;
}

bool hashtable_contains(hashtable_t *p_ht, const void *p_key)
{
    if ((NULL == p_ht) || (NULL == p_key))
    {
        return false;
    }

    if (NULL != static_find_node(p_ht, p_key))
    {
        return true;
    }

    return false;
}

void hashtable_iterate(hashtable_t *p_ht, iter_func iterate)
{
    if ((NULL == p_ht) || (NULL == iterate))
    {
        return;
    }

    for (size_t index = 0; index < p_ht->bucket_count; index++)
    {
        bucket_t *p_bucket = p_ht->pp_buckets[index];

        for (node_t *p_current = p_bucket->head; NULL != p_current; p_current = p_current->p_next)
        {
            iterate(p_current->p_key, p_current->p_value);
        }
    }
}

/* Static functions*/

static node_t *static_find_node(hashtable_t *p_ht, const void *p_key)
{
    node_t *p_retval = NULL;

    if ((NULL == p_ht) || (NULL == p_key))
    {
        return NULL;
    }

    size_t bucket_num = static_bucket_index(p_ht, p_key);

    bucket_t *p_bucket = p_ht->pp_buckets[bucket_num];

    for (node_t *p_current = p_bucket->head; NULL != p_current; p_current = p_current->p_next)
    {
        if (0 == p_ht->key_compare(p_current->p_key, p_key))
        {
            p_retval = p_current;
            break;
        }
    }

    return p_retval;
}

static size_t static_bucket_index(const hashtable_t *p_ht, const void *p_key)
{
    if ((NULL == p_ht) || (NULL == p_key))
    {
        return 0;
    }

    return p_ht->hash(p_key) % p_ht->bucket_count;
}

static bool static_is_prime(size_t check)
{
    if (1 >= check)
    {
        return false;
    }

    if (3 >= check)
    {
        return true;
    }

    if ((0 == check % 2) || (0 == check % 3))
    {
        return false;
    }

    for (size_t factor = 5; factor <= check / factor; factor = factor + 6)
    {
        if ((0 == check % factor) || (0 == check % (factor + 2)))
        {
            return false;
        }
    }

    return true;
}

static size_t static_next_prime(size_t p_current)
{
    if (1 >= p_current)
    {
        return 2;
    }

    size_t next = p_current;
    bool found = false;

    while (false == found)
    {
        next++;

        if (true == static_is_prime(next))
        {
            found = true;
        }
    }

    return next;
}

static bool static_resize_if_needed(hashtable_t *p_ht)
{
    if (NULL == p_ht)
    {
        return false;
    }

    if ((double)p_ht->size / (double)p_ht->bucket_count < 0.75)
    {
        return false;
    }

    if (SIZE_MAX / 2 < p_ht->bucket_count)
    {
        return false;
    }

    size_t new_bucket_count = static_next_prime(p_ht->bucket_count * 2);

    if (0 == new_bucket_count)
    {
        return false;
    }

    bucket_t **pp_new_buckets = calloc(new_bucket_count, sizeof(bucket_t *));

    if (NULL == pp_new_buckets)
    {
        return false;
    }

    // make new buckets
    for (size_t index = 0; index < new_bucket_count; index++)
    {
        pp_new_buckets[index] = calloc(1, sizeof(*pp_new_buckets[index]));

        if (NULL == pp_new_buckets[index])
        {
            while (index > 0)
            {
                free(pp_new_buckets[--index]);
            }

            free(pp_new_buckets);
            return false;
        }
    }

    // move nodes to new buckets
    for (size_t index = 0; index < p_ht->bucket_count; index++)
    {
        node_t *p_node = p_ht->pp_buckets[index]->head;

        while (p_node != NULL)
        {
            node_t *p_next = p_node->p_next;
            size_t new_index = p_ht->hash(p_node->p_key) % new_bucket_count;
            bucket_t *p_bucket = pp_new_buckets[new_index];

            p_node->p_next = NULL;
            p_node->p_previous = p_bucket->tail;

            if (p_bucket->tail != NULL)
            {
                p_bucket->tail->p_next = p_node;
            }
            else
            {
                p_bucket->head = p_node;
            }

            p_bucket->tail = p_node;

            p_node = p_next;
        }
    }

    // destroy old buckets
    for (size_t index = 0; index < p_ht->bucket_count; index++)
    {
        free(p_ht->pp_buckets[index]);
    }

    free(p_ht->pp_buckets);

    p_ht->pp_buckets = pp_new_buckets;
    p_ht->bucket_count = new_bucket_count;

    return true;
}

static int static_insert_without_resizing(hashtable_t *p_ht, const void *p_key, void *p_value)
{
    if ((NULL == p_ht) || (NULL == p_key) || (NULL == p_value))
    {
        return EXIT_FAILURE;
    }

    size_t bucket_num = static_bucket_index(p_ht, p_key);

    bucket_t *p_bucket = p_ht->pp_buckets[bucket_num];

    node_t *p_new = static_find_node(p_ht, p_key);

    if (NULL != p_new)
    {
        return 2; // return 2 if key already in use
    }

    p_new = calloc(1, sizeof(*p_new));

    if (NULL == p_new)
    {
        return EXIT_FAILURE;
    }

    p_new->p_key = (void *)p_key;
    p_new->p_value = p_value;

    if (NULL == p_bucket->head)
    {
        p_bucket->head = p_new;
        p_bucket->tail = p_new;
    }
    else
    {
        p_new->p_previous = p_bucket->tail;
        p_bucket->tail->p_next = p_new;
        p_bucket->tail = p_new;
    }

    p_ht->size++;
    return EXIT_SUCCESS;
}

/* End of file hash.c */
