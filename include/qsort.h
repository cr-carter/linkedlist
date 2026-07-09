#include <stdlib.h>

/**
 * @name quicksort
 *
 * @brief Sort an array with average time O(n log(n))
 *
 * @details The quicksort() function sorts an array with @p nmemb elements of size @p size. The @p base argument points
 * to the start of the array.
 *
 * The contents of the array are sorted in ascending order according to a comparison function pointed to by @p compar,
 * which is called with two arguments that point to the objects being compared.
 *
 * The comparison function must return an integer less than, equal to, or greater than zero if the first argument is
 * considered to be less than, equal to, or greater than the second, respectively. If two members are equal, their order
 * in the sorted array is undefined.
 *
 * @param base Points to the start of the array, must not be NULL.
 * @param nmemb Number of elements in the array, must be greater than 0.
 * @param size The size of each element, must be greater than 0.
 * @param compar The comparison function used during sorting, must not be NULL.
 *
 * @return The quicksort() function returns no value.
 *
 * @note To compare C strings, the comparison function can call strcmp().
 */
void quicksort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));

/**
 * @name mergesort
 *
 * @brief Sort an array with average time O(n log(n))
 *
 * @details The mergesort() function sorts an array with @p nmemb elements of size @p size. The @p base argument points
 * to the start of the array.
 *
 * The contents of the array are sorted in ascending order according to a comparison function pointed to by @p compar,
 * which is called with two arguments that point to the objects being compared.
 *
 * The comparison function must return an integer less than, equal to, or greater than zero if the first argument is
 * considered to be less than, equal to, or greater than the second, respectively. If two members are equal, their order
 * in the sorted array is undefined.
 *
 * @param base Points to the start of the array, must not be NULL.
 * @param nmemb Number of elements in the array, must be greater than 0.
 * @param size The size of each element, must be greater than 0.
 * @param compar The comparison function used during sorting, must not be NULL.
 *
 * @return The mergesort() function returns no value.
 *
 * @note To compare C strings, the comparison function can call strcmp().
 */
void mergesort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
