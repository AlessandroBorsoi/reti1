#include "splitter_private.h"
#include <stdio.h>
#include <stdlib.h>

upo_protocol_splitter_t upo_protocol_splitter_create(char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
        return NULL;

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file) + 1;
    fseek(file, 0, SEEK_SET);

    char *numbers = malloc(fsize);
    fread(numbers, 1, fsize, file);
    fclose(file);

    numbers[fsize] = 0;
    upo_protocol_splitter_t splitter = NULL;
    splitter->input_file = numbers;
    splitter->size = fsize;
    splitter->position = 0;
    return splitter;
}

void upo_protocol_splitter_destroy(upo_protocol_splitter_t *splitter)
{
    if (splitter != NULL && *splitter != NULL)
    {
        (*splitter)->size = 0;
        (*splitter)->position = 0;
        free((*splitter)->input_file);
        (*splitter) = NULL;
    }
}
