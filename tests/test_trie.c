#include "trie.h"
#include "unity.h"

#include <stdio.h>
#include <stdlib.h>

// ant, and, dad, do, dog

void test_trie(void)
{
    trie_t *p_trie = trie_create();
    if (NULL == p_trie)
    {
        TEST_FAIL();
    }

    int check = trie_insert(p_trie, (char *)"ant");
    if (EXIT_FAILURE == check)
    {
        TEST_FAIL();
    }

    check = trie_insert(p_trie, (char *)"and");
    if (EXIT_FAILURE == check)
    {
        TEST_FAIL();
    }

    check = trie_insert(p_trie, (char *)"dad");
    if (EXIT_FAILURE == check)
    {
        TEST_FAIL();
    }

    check = trie_insert(p_trie, (char *)"do");
    if (EXIT_FAILURE == check)
    {
        TEST_FAIL();
    }

    check = trie_insert(p_trie, (char *)"dog");
    if (EXIT_FAILURE == check)
    {
        TEST_FAIL();
    }

    trie_print(p_trie);

    check = trie_search(p_trie, (char *)"do");
    if (EXIT_FAILURE == check)
    {
        TEST_FAIL();
    }

    check = trie_search(p_trie, (char *)"an");
    if (EXIT_FAILURE != check)
    {
        TEST_FAIL();
    }

    check = trie_search(p_trie, (char *)"ant");
    if (EXIT_FAILURE == check)
    {
        TEST_FAIL();
    }

    trie_delete(p_trie, (char *)"ant");

    check = trie_search(p_trie, (char *)"ant");
    if (EXIT_FAILURE != check)
    {
        TEST_FAIL();
    }

    check = trie_search(p_trie, (char *)"dot");
    if (EXIT_FAILURE != check)
    {
        TEST_FAIL();
    }

    check = trie_search(p_trie, (char *)"b");
    if (EXIT_FAILURE != check)
    {
        TEST_FAIL();
    }

    trie_destroy(p_trie);
}
