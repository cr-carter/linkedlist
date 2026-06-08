
#include "llist.h"
#include "unity.h"

#include <stdio.h>
#include <stdlib.h>

void test_llist_create_node_good(void)
{
    printf("\nTesting llist_create_node with good values...\n");
    node_t *p_temp = llist_create_node(0);

    if ((NULL != p_temp) && (0 == p_temp->value) && (NULL == p_temp->p_next))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }

    p_temp = llist_create_node(-1);

    if ((NULL != p_temp) && (-1 == p_temp->value) && (NULL == p_temp->p_next))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_insert_head_good(void)
{
    printf("\nTesting llist_insert_head with good values...\n");
    node_t *p_head = llist_create_node(0);
    node_t *p_new_head = llist_create_node(1);
    node_t *p_check = p_head;

    int test_val = llist_insert_head(&p_head, p_new_head);

    if ((1 == test_val) && (p_check == p_new_head->p_next))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_insert_head_bad(void)
{
    printf("\nTesting llist_insert_head with bad values...\n");
    node_t *p_head = llist_create_node(0);
    node_t *p_new_head = llist_create_node(1);

    int test_val1 = llist_insert_head(NULL, p_new_head);
    int test_val2 = llist_insert_head(&p_head, NULL);
    int test_val3 = llist_insert_head(NULL, NULL);

    if ((0 == test_val1) && (0 == test_val2) && (0 == test_val3))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_insert_tail_good(void)
{
    printf("\nTesting llist_insert_tail with good values...\n");
    node_t *p_head = llist_create_node(0);
    node_t *p_new_tail = llist_create_node(1);

    int test_val = llist_insert_tail(p_head, p_new_tail);

    if ((1 == test_val) && (p_new_tail == p_head->p_next))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_insert_tail_bad(void)
{
    printf("\nTesting llist_insert_tail with bad values...\n");
    node_t *p_head = llist_create_node(0);
    node_t *p_new_head = llist_create_node(1);

    int test_val1 = llist_insert_tail(NULL, p_new_head);
    int test_val2 = llist_insert_tail(p_head, NULL);
    int test_val3 = llist_insert_tail(NULL, NULL);

    if ((0 == test_val1) && (0 == test_val2) && (0 == test_val3))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_insert_after_good(void)
{
    printf("\nTesting llist_insert_after with good values...\n");
    node_t *p_head = llist_create_node(0);
    node_t *p_tail = llist_create_node(2);
    node_t *p_insert = llist_create_node(1);

    llist_insert_tail(p_head, p_tail);
    int test_val = llist_insert_after(p_head, p_insert, 0);

    if ((1 == test_val) && (p_insert == p_head->p_next) && (p_tail == p_insert->p_next))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_insert_after_bad(void)
{
    printf("\nTesting llist_insert_after with bad values...\n");
    node_t *p_head = llist_create_node(0);
    node_t *p_tail = llist_create_node(2);
    node_t *p_insert = llist_create_node(1);

    llist_insert_tail(p_head, p_tail);
    int test_val1 = llist_insert_after(NULL, p_insert, 0);
    int test_val2 = llist_insert_after(p_head, NULL, 0);
    int test_val3 = llist_insert_after(NULL, NULL, 0);
    int test_val4 = llist_insert_after(p_head, p_insert, 1);

    if ((0 == test_val1) && (0 == test_val2) && (0 == test_val3) && (0 == test_val4))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_find_node_good(void)
{
    printf("\nTesting llist_find_node with good values...\n");
    node_t *p_head = llist_create_node(0);
    node_t *p_insert = llist_create_node(1);
    node_t *p_tail = llist_create_node(2);

    llist_insert_tail(p_head, p_insert);
    llist_insert_tail(p_head, p_tail);

    node_t *p_check1 = llist_find_node(p_head, 1);
    node_t *p_check2 = llist_find_node(p_head, -1);

    if ((p_check1 == p_insert) && (NULL == p_check2))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_find_node_bad(void)
{
    printf("\nTesting llist_find_node with bad values...\n");
    node_t *p_head = llist_create_node(0);
    node_t *p_insert = llist_create_node(1);
    node_t *p_tail = llist_create_node(2);

    llist_insert_tail(p_head, p_insert);
    llist_insert_tail(p_head, p_tail);

    node_t *p_check1 = llist_find_node(NULL, 1);

    if (p_check1 == NULL)
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_remove_node_good(void)
{
    printf("\nTesting llist_remove_node with good values...\n");
    node_t *p_head = llist_create_node(0);
    node_t *p_insert = llist_create_node(1);
    node_t *p_tail = llist_create_node(2);

    llist_insert_tail(p_head, p_insert);
    llist_insert_tail(p_head, p_tail);

    node_t *p_check1 = llist_remove_node(&p_head, 1);
    node_t *p_check2 = llist_remove_node(&p_head, -1);

    if ((p_check1 == p_insert) && (NULL == p_check2) && (p_tail == p_head->p_next) && (1 == p_check1->value))
    {
        TEST_PASS();
    }
    else
    {
        TEST_FAIL();
    }
}

void test_llist_remove_node_bad(void)
{
    printf("\nTesting llist_remove_node with bad values...\n");
}

void test_llist_delete_node_good(void)
{
    printf("\nTesting llist_delete_node with good values...\n");
}

void test_llist_delete_node_bad(void)
{
    printf("\nTesting llist_delete_node with bad values...\n");
}

void test_llist_size_good(void)
{
    printf("\nTesting llist_size with good values...\n");
}

void test_llist_size_bad(void)
{
    printf("\nTesting llist_size with bad values...\n");
}

/* End of file test_llist.c */
