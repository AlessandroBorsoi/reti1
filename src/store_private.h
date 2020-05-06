#pragma once

#include <stddef.h>
#include <upo/store.h>

struct upo_store_node_s
{
    uint64_t number;
    struct upo_store_node_s *next;
};

typedef struct upo_store_node_s upo_store_node_t;

struct upo_store_s
{
    upo_store_node_t *top;
    size_t size;
};
