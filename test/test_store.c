#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <upo/store.h>

#define EPSILON 0.00001

static void test_create_destroy();
static void test_insert_size();
static void test_average();
static void test_variance();
static bool fequal(double a, double b);

int main()
{
    printf("Test case 'create/destroy'... ");
    fflush(stdout);
    test_create_destroy();
    printf("OK\n");

    printf("Test case 'insert/size'... ");
    fflush(stdout);
    test_insert_size();
    printf("OK\n");

    printf("Test case 'average'... ");
    fflush(stdout);
    test_average();
    printf("OK\n");

    printf("Test case 'variance'... ");
    fflush(stdout);
    test_variance();
    printf("OK\n");

    return 0;
}

void test_create_destroy()
{
    upo_store_t store = upo_store_create();

    assert(store != NULL);

    upo_store_destroy(store);
}

void test_insert_size()
{
    upo_store_t store = upo_store_create();

    for (uint64_t i = 0; i < 1000; i++)
        upo_store_insert(store, i);

    assert(upo_store_size(store) == 1000);

    for (uint64_t i = 0; i < 100; i++)
        upo_store_insert(store, i);

    assert(upo_store_size(store) == 1100);

    upo_store_destroy(store);

    assert(upo_store_size(store) == 0);
}

void test_average()
{
    upo_store_t store = upo_store_create();

    assert(fequal(upo_store_sample_mean(store), -1));

    upo_store_insert(store, 1);

    assert(fequal(upo_store_sample_mean(store), 1));

    upo_store_insert(store, 3);

    assert(fequal(upo_store_sample_mean(store), 2));

    upo_store_insert(store, 5);

    assert(fequal(upo_store_sample_mean(store), 3));

    upo_store_destroy(store);

    store = upo_store_create();

    assert(fequal(upo_store_sample_mean(store), -1));

    for (uint64_t i = 1; i <= 100; i++)
        upo_store_insert(store, i);

    assert(fequal(upo_store_sample_mean(store), 50.5));

    upo_store_destroy(store);

    store = upo_store_create();

    for (uint64_t i = 0; i < 100000; i++)
        upo_store_insert(store, 99999);

    assert(fequal(upo_store_sample_mean(store), 99999));

    upo_store_destroy(store);
}

void test_variance()
{
    upo_store_t store = upo_store_create();

    assert(fequal(upo_store_sample_variance(store), -1));

    upo_store_insert(store, 1);

    assert(fequal(upo_store_sample_variance(store), -1));

    upo_store_insert(store, 1);

    assert(fequal(upo_store_sample_variance(store), 0));

    upo_store_destroy(store);

    store = upo_store_create();

    for (uint64_t i = 1; i <= 10; i++)
        upo_store_insert(store, i);

    assert(fequal(upo_store_sample_variance(store), 9.166667));

    upo_store_destroy(store);

    store = upo_store_create();

    upo_store_insert(store, 10);
    upo_store_insert(store, 12);
    upo_store_insert(store, 20);

    assert(fequal(upo_store_sample_variance(store), 28.0));

    upo_store_destroy(store);
}

bool fequal(double a, double b)
{
    return fabs(a - b) < EPSILON;
}
