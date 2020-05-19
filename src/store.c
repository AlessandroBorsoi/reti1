#include "store_private.h"
#include <stdlib.h>
#include <stdio.h>

upo_store_t upo_store_create()
{
    upo_store_t store = malloc(sizeof(struct upo_store_s));
    if (store == NULL)
    {
        fprintf(stderr, "Unable to create a store!\n");
        abort();
    }
    store->size = 0;
    store->sum = 0;
    store->pow = 0;
    return store;
}

void upo_store_destroy(upo_store_t *store)
{
    if (store != NULL && *store != NULL)
    {
        free(*store);
        *store = NULL;
    }
}

void upo_store_insert(upo_store_t store, uint64_t number)
{
    if (store == NULL)
        store = upo_store_create();
    store->size++;
    store->sum += number;
    store->pow += number * number;
}

uint64_t upo_store_size(const upo_store_t store)
{
    if (store == NULL)
        return 0;
    return store->size;
}

double upo_store_sample_mean(const upo_store_t store)
{
    size_t size = upo_store_size(store);
    if (size == 0)
        return -1;
    else
        return store->sum / (double)size;
}

double upo_store_sample_variance(const upo_store_t store)
{
    size_t size = upo_store_size(store);
    if (size < 2)
        return -1;
    else
    {
        double mean = upo_store_sample_mean(store);
        return (store->pow - size * mean * mean) / (size - 1);
    }
}
