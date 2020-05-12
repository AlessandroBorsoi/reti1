#include "splitter_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static bool is_valid_input(char *input);

upo_protocol_splitter_t upo_protocol_splitter_create(char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
        return NULL;

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *numbers = malloc(file_size + 1);
    fread(numbers, 1, file_size, file);
    fclose(file);
    numbers[file_size] = 0;

    upo_protocol_splitter_t splitter = malloc(sizeof(struct upo_protocol_splitter_s));
    if (is_valid_input(numbers))
    {
        splitter->input_file = numbers;
    }
    else
    {
        splitter->input_file = NULL;
        free(numbers);
    }
    splitter->size = file_size + 1;
    splitter->position = 0;
    return splitter;
}

void upo_protocol_splitter_destroy(upo_protocol_splitter_t *splitter)
{
    if (splitter != NULL && *splitter != NULL)
    {
        (*splitter)->size = 0;
        (*splitter)->position = 0;
        if (upo_protocol_splitter_is_valid(*splitter))
            free((*splitter)->input_file);
        free(*splitter);
        (*splitter) = NULL;
    }
}

bool upo_protocol_splitter_is_valid(upo_protocol_splitter_t splitter)
{
    if (splitter != NULL)
        return splitter->input_file != NULL;
    return false;
}

void upo_protocol_splitter_next(upo_protocol_splitter_t splitter, char *output, size_t output_size)
{
    strcpy(output, "0\n");
}

bool is_valid_input(char *input)
{
    for (int i = 0; input[i] != '\0'; i++)
        if (!isdigit(input[i]) && !isspace(input[i]))
            return false;
    return true;
}
