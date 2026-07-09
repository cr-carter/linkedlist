#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef NDEBUG
#define EPRINTF(fmt, ...) ((void)0)
#else
#define EPRINTF(fmt, ...) fprintf(stderr, "[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

static size_t hoare_partition(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
{
    char *arr = base;

    // Step 1: choose a pivot (first element)
    char *pivot = calloc(1, size);

    memcpy(pivot, arr, size);
    char *temp = calloc(1, size);

    // Step 2: partition the array. Re arrange the array around the pivot. All elements smaller than the pivot will be
    // on the left, all elements greater than the pivot will be on the right
    size_t left_index = 0;
    size_t right_index = nmemb - 1;

    for (;;)
    {
        // find next element larger than pivot
        // from the left
        while ((left_index < nmemb) && (0 > compar(arr + left_index * size, pivot)))
        {
            left_index++;
        }

        // find next element smaller than pivot
        // from the right
        while ((__SIZE_MAX__ != right_index) && (0 < compar(arr + right_index * size, pivot)))
        {
            right_index--;
        }

        // if left and right crosses each other
        // no swapping required
        if (left_index >= right_index)
        {
            free(pivot);
            free(temp);
            return right_index;
        }

        // swap larger and smaller elements
        memcpy(temp, arr + left_index * size, size);
        memcpy(arr + left_index * size, arr + right_index * size, size);
        memcpy(arr + right_index * size, temp, size);

        left_index++;
        right_index--;
    }
}

void quicksort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
{
    if ((NULL == base) || (NULL == compar) || (0 == nmemb) || (0 == size))
    {
        EPRINTF("%s", "quicksort() passed invalid parameters");
        return;
    }

    char *arr = base;

    // Base case: the recursion stops when there is only one element left in the sub-array, as a single element is
    // already sorted
    if (1 >= nmemb)
    {
        return;
    }

    size_t partition_index = hoare_partition(base, nmemb, size, compar);

    // Recursively call: Recursively apply the same process to the two partitioned sub-arrays
    // left_index + 1 is base of right array
    // base is still base of right array

    quicksort(arr, partition_index + 1, size, compar);
    quicksort(arr + ((partition_index + 1) * size), nmemb - (partition_index + 1), size, compar);
}

static void merge(char *left, char *right, size_t left_nmemb, size_t right_nmemb, size_t size,
                  int (*compar)(const void *, const void *))
{
    char *result = calloc(left_nmemb + right_nmemb, size);

    size_t left_index = 0;
    size_t right_index = 0;
    size_t result_index = 0;

    while ((left_index < left_nmemb) && (right_index < right_nmemb))
    {
        if (0 >= compar(left + (left_index * size), right + (right_index * size)))
        {
            memcpy(result + (result_index * size), left + (left_index * size), size);
            left_index++;
        }

        else
        {
            memcpy(result + (result_index * size), right + (right_index * size), size);
            right_index++;
        }
        result_index++;
    }

    while (left_index < left_nmemb)
    {
        memcpy(result + (result_index * size), left + (left_index * size), size);
        result_index++;
        left_index++;
    }

    while (right_index < right_nmemb)
    {
        memcpy(result + (result_index * size), right + (right_index * size), size);
        result_index++;
        right_index++;
    }
    memcpy(left, result, (left_nmemb + right_nmemb) * size);
    free(result);
}

void mergesort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
{
    if ((NULL == base) || (NULL == compar) || (0 == nmemb) || (0 == size))
    {
        EPRINTF("%s", "mergesort() passed invalid parameters");
        return;
    }

    // Split into left and right arrays
    char *arr = base;

    size_t left_nmemb = nmemb / 2;
    size_t right_nmemb = nmemb - left_nmemb;
    char *left_arr = arr;
    char *right_arr = arr + (left_nmemb * size);

    // Recursively call: Recursively split into smaller arrays until

    if (1 < left_nmemb)
    {
        mergesort(left_arr, left_nmemb, size, compar);
    }

    if (1 < right_nmemb)
    {
        mergesort(right_arr, right_nmemb, size, compar);
    }

    // Base case: the recursion stops when there is only one element left in the sub-array, as a single element is
    // already sorted
    // Begin merging arrays
    merge(left_arr, right_arr, left_nmemb, right_nmemb, size, compar);
}

/* End of file qsort.c */
