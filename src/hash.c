#include "hash.h"

#include <stdlib.h>

typedef struct node node_t;

struct node
{
    void *p_key;
    void *p_data;
    node_t *p_next;
    node_t *p_previous;
};

typedef struct llist
{
    node_t *p_head;
    node_t *p_tail;
    int size;
} llist_t;

struct hashtable
{
    llist_t **pp_buckets;
    size_t bucket_count;
    int size;
    hashtable_hash_f hash_func;
    hashtable_cmp_f cmp_func;
};

static node_t *static_create_node(void *p_data);

static node_t *static_key_in_bucket(llist_t *p_bucket, const void *p_key, hashtable_cmp_f cmp_f);

static void static_llist_destroy(llist_t **pp_list, hashtable_key_del_f key_del_func, hashtable_del_f data_del_func);

hashtable_t *hashtable_create(size_t bucket_count, hashtable_hash_f hash_func, hashtable_cmp_f cmp_func)
{
    hashtable_t *retval = NULL;

    // Validate input, allocate table
    if ((NULL != hash_func) && (NULL != cmp_func) && (0 < bucket_count))
    {
        retval = calloc(1, sizeof(*retval));
    }

    // Validate table allocation, allocate array of buckets
    int allocation_failed = 0;
    if (NULL != retval)
    {
        retval->pp_buckets = calloc(bucket_count, sizeof(*(retval->pp_buckets)));
    }
    else
    {
        allocation_failed = 1;
    }

    // Validate array, allocate buckets

    if ((0 == allocation_failed) && (NULL == retval->pp_buckets))
    {
        free(retval);
        retval = NULL;
    }
    else
    {
        for (size_t index = 0; index < bucket_count; index++)
        {
            retval->pp_buckets[index] = calloc(1, sizeof(llist_t));

            if (NULL == retval->pp_buckets[index])
            {
                allocation_failed = 1;

                for (size_t clean_index = 0; clean_index < index; clean_index++)
                {
                    free(retval->pp_buckets[clean_index]);
                }
                break;
            }
        }
    }

    if (1 == allocation_failed)
    {
        if (NULL != retval)
        {
            free(retval->pp_buckets);
        }

        free(retval);
        retval = NULL;
    }
    else
    {
        retval->bucket_count = bucket_count;
        retval->cmp_func = cmp_func;
        retval->hash_func = hash_func;
    }

    return retval;
}

int hashtable_insert(hashtable_t *p_table, void *p_key, void *p_data)
{
    int retval = -1;

    if ((NULL != p_table) && (NULL != p_key) && (NULL != p_data))
    {
        size_t hash_value = p_table->hash_func(p_key);
        size_t bucket_num = hash_value % p_table->bucket_count;

        llist_t *p_bucket = p_table->pp_buckets[bucket_num];

        node_t *p_new = NULL;

        if (NULL == static_key_in_bucket(p_bucket, p_key, p_table->cmp_func))
        {
            p_new = static_create_node(p_data);
        }

        if (NULL != p_new)
        {
            if (NULL != p_bucket->p_tail)
            {
                p_new->p_previous = p_bucket->p_tail;
                p_bucket->p_tail->p_next = p_new;
            }
            else
            {
                p_bucket->p_head = p_new;
            }

            p_new->p_key = p_key;
            p_bucket->p_tail = p_new;
            p_bucket->size += 1;
            p_table->size += 1;
            retval = 1;
        }
    }

    return retval;
}

void *hashtable_find(hashtable_t *p_table, const void *p_key)
{
    void *p_retval = NULL;
    node_t *p_found = NULL;

    if ((NULL != p_table) && (NULL != p_key))
    {
        size_t hash_value = p_table->hash_func(p_key);
        size_t bucket_num = hash_value % p_table->bucket_count;

        llist_t *p_bucket = p_table->pp_buckets[bucket_num];

        p_found = static_key_in_bucket(p_bucket, p_key, p_table->cmp_func);
    }

    if (NULL != p_found)
    {
        p_retval = p_found->p_data;
    }

    return p_retval;
}

void *hashtable_remove(hashtable_t *p_table, const void *p_key)
{
    void *retval = NULL;
    node_t *p_remove = NULL;

    if ((NULL != p_table) && (NULL != p_key))
    {
        size_t hash_value = p_table->hash_func(p_key);
        size_t bucket_num = hash_value % p_table->bucket_count;

        llist_t *p_bucket = p_table->pp_buckets[bucket_num];

        if (NULL != p_bucket)
        {
            for (node_t *p_position = p_bucket->p_head; NULL != p_position; p_position = p_position->p_next)
            {
                if (0 == p_table->cmp_func(p_position->p_key, p_key))
                {
                    retval = p_position->p_data;
                    p_remove = p_position;
                    break;
                }
            }
        }

        if (NULL != p_remove)
        {

            if ((p_remove == p_bucket->p_head) && (p_bucket->p_head == p_bucket->p_tail))
            {
                p_bucket->p_tail = NULL;
            }

            if (p_remove == p_bucket->p_head)
            {
                if (NULL != p_remove->p_next)
                {
                    p_remove->p_next->p_previous = NULL;
                }

                p_bucket->p_head = p_remove->p_next;
            }
            else if (p_remove == p_bucket->p_tail)
            {
                if (NULL != p_remove->p_previous)
                {
                    p_remove->p_previous->p_next = NULL;
                }

                p_bucket->p_tail = p_remove->p_previous;
            }
            else
            {
                p_remove->p_previous->p_next = p_remove->p_next;
                p_remove->p_next->p_previous = p_remove->p_previous;
            }

            free(p_remove);
            p_bucket->size -= 1;
            p_table->size -= 1;
        }
    }

    return retval;
}

int hashtable_size(hashtable_t *p_table)
{
    int retval = -1;

    if (NULL != p_table)
    {
        retval = p_table->size;
    }

    return retval;
}

void hashtable_destroy(hashtable_t **pp_table, hashtable_key_del_f key_del_func, hashtable_del_f data_del_func)
{
    if ((NULL != pp_table) && (NULL != *pp_table))
    {
        for (size_t index = 0; index < (*pp_table)->bucket_count; index++)
        {
            static_llist_destroy(&((*pp_table)->pp_buckets[index]), key_del_func, data_del_func);
        }

        free((*pp_table)->pp_buckets);
        free(*pp_table);
        *pp_table = NULL;
    }
}

int hashtable_contains(hashtable_t *p_table, const void *p_key)
{
    int retval = -1;

    if ((NULL != p_table) && (NULL != p_key))
    {
        retval = 0;

        if (NULL != hashtable_find(p_table, p_key))
        {
            retval = 1;
        }
    }

    return retval;
}

static node_t *static_create_node(void *p_data)
{
    node_t *p_node = calloc(1, sizeof(*p_node));

    if (NULL != p_node)
    {
        p_node->p_data = p_data;
    }

    return p_node;
}

static node_t *static_key_in_bucket(llist_t *p_bucket, const void *p_key, hashtable_cmp_f cmp_f)
{
    node_t *retval = NULL;

    if ((NULL != p_bucket) && (NULL != p_key) && (NULL != cmp_f))
    {

        node_t *p_position = p_bucket->p_head;

        for (int index = 0; NULL != p_position; p_position = p_position->p_next, index++)
        {
            if (0 == cmp_f(p_key, p_position->p_key))
            {
                retval = p_position;
                break;
            }
        }
    }

    return retval;
}

static void static_llist_destroy(llist_t **pp_list, hashtable_key_del_f key_del_func, hashtable_del_f data_del_func)
{
    if ((NULL != pp_list) && (NULL != *pp_list))
    {
        node_t *p_current = (*pp_list)->p_head;

        while (NULL != p_current)
        {

            node_t *p_delete = p_current;

            if (NULL != data_del_func)
            {
                data_del_func(p_delete->p_data);
            }

            if (NULL != key_del_func)
            {
                key_del_func(p_delete->p_key);
            }

            p_current = p_current->p_next;

            if (NULL != p_delete)
            {
                free(p_delete);
            }
        }
        free(*pp_list);
        *pp_list = NULL;
    }
}

/* End of file hash.c */
