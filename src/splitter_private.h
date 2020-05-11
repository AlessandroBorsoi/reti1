#pragma once

#include <upo/splitter.h>

struct upo_protocol_splitter_s
{
    char *input_file;
    size_t size;
    uint64_t position;
};
