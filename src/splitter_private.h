#pragma once

#include <upo/client/splitter.h>

struct upo_protocol_splitter_s
{
    uint64_t *numbers;
    size_t size;
    size_t current;
};
