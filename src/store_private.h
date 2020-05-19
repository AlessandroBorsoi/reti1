#pragma once

#include <upo/server/store.h>

struct upo_store_s
{
    uint64_t size;
    uint64_t sum;
    uint64_t pow;
};
