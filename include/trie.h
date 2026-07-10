#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct trie trie_t;

/**
 * @name trie_create
 *
 * @brief
 *
 * @details
 *
 * @param base Points to the start of the array, must not be NULL.
 *
 * @return
 *
 * @note
 */
trie_t *trie_create();

/**
 * @name trie_insert
 *
 * @brief
 *
 * @details
 *
 * @param base Points to the start of the array, must not be NULL.
 *
 * @return
 *
 * @note
 */
int trie_insert(trie_t *p_trie, char *text);

/**
 * @name trie_search
 *
 * @brief
 *
 * @details
 *
 * @param base Points to the start of the array, must not be NULL.
 *
 * @return
 *
 * @note
 */
int trie_search(trie_t *p_trie, char *text);

/**
 * @name trie_delete
 *
 * @brief
 *
 * @details
 *
 * @param base Points to the start of the array, must not be NULL.
 *
 * @return
 *
 * @note
 */
int trie_delete(trie_t *p_trie, char *text);

/**
 * @name trie_print
 *
 * @brief
 *
 * @details
 *
 * @param base Points to the start of the array, must not be NULL.
 *
 * @return
 *
 * @note
 */
void trie_print(trie_t *p_trie);

/**
 * @name trie_destroy
 *
 * @brief
 *
 * @details
 *
 * @param base Points to the start of the array, must not be NULL.
 *
 * @return
 *
 * @note
 */
void trie_destroy(trie_t *p_trie);

/* End of file trie.h */
