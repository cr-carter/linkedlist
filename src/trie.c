/**
 * @file trie.c
 * @author Chase Carter
 * @brief Trie Abstract Data Type.
 */

#include "trie.h"

#include <stdio.h>
#include <stdlib.h>

#define ALPHA_RANGE 26

typedef struct tnode tnode_t;

struct tnode
{
    bool end;
    tnode_t *next_letter[ALPHA_RANGE];
};

struct trie
{
    tnode_t *root;
    size_t word_count;
};

static tnode_t *static_create_node()
{
    tnode_t *retval = calloc(1, sizeof(*retval));

    return retval;
}

static void static_recursive_print(tnode_t *temp, char *sub, size_t depth);

static void static_recursive_delete(tnode_t **temp);

static int static_validate_word(char *text);

trie_t *trie_create()
{
    trie_t *retval = calloc(1, sizeof(*retval));

    if (NULL == retval)
    {
        return NULL;
    }

    retval->root = calloc(1, sizeof(*retval->root));

    if (NULL == retval->root)
    {
        free(retval);
        return NULL;
    }

    return retval;
}

int trie_insert(trie_t *p_trie, char *text)
{
    if ((NULL == p_trie) || (NULL == p_trie->root))
    {
        return EXIT_FAILURE;
    }

    int check = static_validate_word(text);

    if (1 == check)
    {
        return EXIT_FAILURE;
    }

    size_t word_len = strlen(text);

    tnode_t *temp = p_trie->root;

    size_t letter_index = 0;

    for (size_t index = 0; index < word_len; index++)
    {
        letter_index = (size_t)text[index] - (size_t)'a';

        if (NULL == temp->next_letter[letter_index])
        {
            temp->next_letter[letter_index] = static_create_node();
        }

        temp = temp->next_letter[letter_index];
    }

    temp->end = true;
    return EXIT_SUCCESS;
}

int trie_search(trie_t *p_trie, char *text)
{
    if ((NULL == p_trie) || (NULL == p_trie->root))
    {
        return EXIT_FAILURE;
    }

    int check = static_validate_word(text);

    if (1 == check)
    {
        printf("Word not valid\n");
        return EXIT_FAILURE;
    }

    size_t word_len = strlen(text);

    tnode_t *temp = p_trie->root;

    size_t letter_index = 0;

    for (size_t index = 0; index < word_len; index++)
    {
        letter_index = (size_t)text[index] - (size_t)'a';

        if (NULL == temp->next_letter[letter_index])
        {
            return EXIT_FAILURE;
        }

        temp = temp->next_letter[letter_index];
    }

    if (true == temp->end)
    {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

void trie_print(trie_t *p_trie)
{
    if ((NULL == p_trie) || (NULL == p_trie->root))
    {
        return;
    }

    tnode_t *temp = p_trie->root;
    char buffer[17] = {0}; // Max word len 16 + 1 for NULL byte
    static_recursive_print(temp, buffer, 0);
}

int trie_delete(trie_t *p_trie, char *text)
{
    if ((NULL == p_trie) || (NULL == p_trie->root))
    {
        return EXIT_FAILURE;
    }

    int check = static_validate_word(text);

    if (1 == check)
    {
        printf("Word not valid\n");
        return EXIT_FAILURE;
    }

    size_t word_len = strlen(text);

    tnode_t *temp = p_trie->root;

    size_t letter_index = 0;

    for (size_t index = 0; index < word_len; index++)
    {
        letter_index = (size_t)text[index] - (size_t)'a';

        if (NULL == temp->next_letter[letter_index])
        {
            return EXIT_FAILURE;
        }

        temp = temp->next_letter[letter_index];
    }

    if (true == temp->end)
    {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

void trie_destroy(trie_t *p_trie)
{
    if ((NULL == p_trie) || (NULL == p_trie->root))
    {
        return;
    }

    tnode_t *temp = p_trie->root;
    static_recursive_delete(&temp);

    free(p_trie);
}

/* Static functions */

static void static_recursive_delete(tnode_t **temp)
{
    if ((NULL == temp) || (NULL == *temp))
    {
        return;
    }

    for (size_t index = 0; index < ALPHA_RANGE; index++)
    {
        if (NULL != (*temp)->next_letter[index])
        {
            static_recursive_delete(&(*temp)->next_letter[index]);
        }
    }

    free(*temp);
    *temp = NULL;
}

static void static_recursive_print(tnode_t *temp, char *sub, size_t depth)
{
    if ((NULL == temp) || (NULL == sub))
    {
        return;
    }

    if (true == temp->end)
    {
        sub[depth] = '\0';
        printf("%s\n", sub);
    }

    for (size_t index = 0; index < ALPHA_RANGE; index++)
    {
        if (NULL != temp->next_letter[index])
        {
            sub[depth] = 'a' + (char)index;
            static_recursive_print(temp->next_letter[index], sub, depth + 1);
        }
    }
}

static int static_validate_word(char *text)
{
    if (NULL == text)
    {
        return EXIT_FAILURE;
    }

    size_t word_len = strlen(text);

    if (0 == word_len)
    {
        return EXIT_FAILURE;
    }

    for (size_t index = 0; index < word_len; index++)
    {
        if ((text[index] < 'a') || (text[index] > 'z'))
        {
            if ((text[index] < 'A') || (text[index] > 'Z'))
            {
                text[index] += 32;
            }
            else
            {
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}

/* End of file trie.c */
