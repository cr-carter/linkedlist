#include "qsort.h"
#include "unity.h"

#include <stdio.h>
#include <stdlib.h>

int compare_int(const void *val_a, const void *val_b)
{
    return *(int *)val_a - *(int *)val_b;
}

void test_quicksort_random(void)
{
    int array[] = {5, 7, 1, 3, 9, 8, 2, 0, 6, 4};

    size_t size = sizeof(array[0]);
    size_t nmemb = sizeof(array) / size;

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
    }
    printf("\n");

    quicksort(array, nmemb, size, compare_int);

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
        if ((0 != index) && (array[index] < array[index - 1]))
        {
            TEST_FAIL();
        }
    }
    printf("\n");
}

void test_quicksort_sorted(void)
{
    int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    size_t size = sizeof(array[0]);
    size_t nmemb = sizeof(array) / size;

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
    }
    printf("\n");

    quicksort(array, nmemb, size, compare_int);

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
        if ((0 != index) && (array[index] < array[index - 1]))
        {
            TEST_FAIL();
        }
    }

    printf("\n");
}

void test_quicksort_backwards(void)
{
    int array[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    size_t size = sizeof(array[0]);
    size_t nmemb = sizeof(array) / size;

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
    }
    printf("\n");

    quicksort(array, nmemb, size, compare_int);

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
        if ((0 != index) && (array[index] < array[index - 1]))
        {
            TEST_FAIL();
        }
    }

    printf("\n");
}

void test_quicksort_large(void)
{
    int array[] = {5, 7, 1, 3, 9, 8, 2, 0, 6, 4, 5, 7, 1, 3, 9, 8, 2, 0, 6, 4, 5, 7, 1, 3, 9, 8, 2, 0, 6, 4, 5, 7, 1, 3,
                   9, 8, 2, 0, 6, 4, 5, 7, 1, 3, 9, 8, 2, 0, 6, 4, 5, 7, 1, 3, 9, 8, 2, 0, 6, 4, 5, 7, 1, 3, 9, 8, 2, 0,
                   6, 4, 5, 7, 1, 3, 9, 8, 2, 0, 6, 4, 5, 7, 1, 3, 9, 8, 2, 0, 6, 4, 5, 7, 1, 3, 9, 8, 2, 0, 6, 4};

    size_t size = sizeof(array[0]);
    size_t nmemb = sizeof(array) / size;

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
    }
    printf("\n");

    quicksort(array, nmemb, size, compare_int);

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
        if ((0 != index) && (array[index] < array[index - 1]))
        {
            TEST_FAIL();
        }
    }

    printf("\n");
}

void test_quicksort_all_zeros(void)
{
    int array[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    size_t size = sizeof(array[0]);
    size_t nmemb = sizeof(array) / size;

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
    }
    printf("\n");

    quicksort(array, nmemb, size, compare_int);

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
        if ((0 != index) && (array[index] < array[index - 1]))
        {
            TEST_FAIL();
        }
    }

    printf("\n");
}

void test_quicksort_single(void)
{
    int array[] = {100};

    size_t size = sizeof(array[0]);
    size_t nmemb = sizeof(array) / size;

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
    }
    printf("\n");

    quicksort(array, nmemb, size, compare_int);

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
        if ((0 != index) && (array[index] < array[index - 1]))
        {
            TEST_FAIL();
        }
    }

    printf("\n");
}

void test_quicksort_bad(void)
{
    quicksort(NULL, 0, 0, NULL);
    TEST_PASS();
}

void test_mergesort_random(void)
{
    int array[] = {5, 7, 1, 3, 9, 8, 2, 0, 6, 4};

    size_t size = sizeof(array[0]);
    size_t nmemb = sizeof(array) / size;

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
    }
    printf("\n");

    mergesort(array, nmemb, size, compare_int);

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
        if ((0 != index) && (array[index] < array[index - 1]))
        {
            TEST_FAIL();
        }
    }
    printf("\n");
}

void test_mergesort_sorted(void)
{
    int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    size_t size = sizeof(array[0]);
    size_t nmemb = sizeof(array) / size;

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
    }
    printf("\n");

    mergesort(array, nmemb, size, compare_int);

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
        if ((0 != index) && (array[index] < array[index - 1]))
        {
            TEST_FAIL();
        }
    }

    printf("\n");
}

void test_mergesort_backwards(void)
{
    int array[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    size_t size = sizeof(array[0]);
    size_t nmemb = sizeof(array) / size;

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
    }
    printf("\n");

    mergesort(array, nmemb, size, compare_int);

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
        if ((0 != index) && (array[index] < array[index - 1]))
        {
            TEST_FAIL();
        }
    }

    printf("\n");
}

void test_mergesort_large(void)
{
    int array[] = {5, 7, 1, 3, 9, 8, 2, 0, 6, 4, 5, 7, 1, 3, 9, 8, 2, 0, 6, 4, 5, 7, 1, 3, 9, 8, 2, 0, 6, 4, 5, 7, 1, 3,
                   9, 8, 2, 0, 6, 4, 5, 7, 1, 3, 9, 8, 2, 0, 6, 4, 5, 7, 1, 3, 9, 8, 2, 0, 6, 4, 5, 7, 1, 3, 9, 8, 2, 0,
                   6, 4, 5, 7, 1, 3, 9, 8, 2, 0, 6, 4, 5, 7, 1, 3, 9, 8, 2, 0, 6, 4, 5, 7, 1, 3, 9, 8, 2, 0, 6, 4};

    size_t size = sizeof(array[0]);
    size_t nmemb = sizeof(array) / size;

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
    }
    printf("\n");

    mergesort(array, nmemb, size, compare_int);

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
        if ((0 != index) && (array[index] < array[index - 1]))
        {
            TEST_FAIL();
        }
    }

    printf("\n");
}

void test_mergesort_all_zeros(void)
{
    int array[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    size_t size = sizeof(array[0]);
    size_t nmemb = sizeof(array) / size;

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
    }
    printf("\n");

    mergesort(array, nmemb, size, compare_int);

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
        if ((0 != index) && (array[index] < array[index - 1]))
        {
            TEST_FAIL();
        }
    }

    printf("\n");
}

void test_mergesort_single(void)
{
    int array[] = {100};

    size_t size = sizeof(array[0]);
    size_t nmemb = sizeof(array) / size;

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
    }
    printf("\n");

    mergesort(array, nmemb, size, compare_int);

    for (size_t index = 0; index < nmemb; index++)
    {
        printf("%i, ", array[index]);
        if ((0 != index) && (array[index] < array[index - 1]))
        {
            TEST_FAIL();
        }
    }

    printf("\n");
}

void test_mergesort_bad(void)
{
    mergesort(NULL, 0, 0, NULL);
    TEST_PASS();
}
