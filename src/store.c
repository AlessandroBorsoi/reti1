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
    store->top = NULL;
    store->size = 0;
    return store;
}

void upo_store_destroy(upo_store_t store)
{
    if (store != NULL)
    {
        while (store->top != NULL)
        {
            upo_store_node_t *node = store->top;
            store->top = store->top->next;
            free(node);
        }
        store->size = 0;
        free(store);
    }
}

void upo_store_insert(upo_store_t store, uint64_t number)
{
    upo_store_node_t *newNode;
    if (store == NULL)
        store = upo_store_create();
    newNode = malloc(sizeof(struct upo_store_node_s));
    if (newNode == NULL)
    {
        fprintf(stderr, "Unable to insert the element!\n");
        abort();
    }
    newNode->number = number;
    newNode->next = store->top;
    store->top = newNode;
    store->size++;
}

size_t upo_store_size(const upo_store_t store)
{
    if (store != NULL)
        return store->size;
    return 0;
}

double upo_store_sample_mean(const upo_store_t store)
{
    if (store == NULL || upo_store_size(store) == 0)
        return -1;
    else
    {
        uint64_t sum = 0;
        upo_store_node_t *node = store->top;
        while (node != NULL)
        {
            sum += node->number;
            node = node->next;
        }
        return sum / (double)upo_store_size(store);
    }
}

double upo_store_sample_variance(const upo_store_t store)
{
    if (store == NULL || store->size < 2)
        return -1;
    else
    {
        double sm = upo_store_sample_mean(store);
        double sum = 0;
        upo_store_node_t *node = store->top;
        while (node != NULL)
        {
            double diff = node->number - sm;
            sum += diff * diff;
            node = node->next;
        }
        return sum / (upo_store_size(store) - 1);
    }
}
