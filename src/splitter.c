#include "splitter_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static bool is_valid_input(char *input);
static size_t space(uint64_t *numbers, int number_count);
static uint64_t add(upo_protocol_splitter_t splitter, uint64_t *ptr);

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
    memset(output, '\0', output_size);
    if (upo_protocol_splitter_is_valid(splitter) && splitter->position == splitter->size - 1)
    {
        strcpy(output, "0\n");
        return;
    }
    uint64_t numbers_to_send[output_size / 2];
    numbers_to_send[0] = 0;
    int number_count = 1;
    while (true)
    {
        uint64_t tmp_position = splitter->position;
        numbers_to_send[number_count] = add(splitter, &tmp_position);
        if (space(numbers_to_send, number_count + 1) > output_size || splitter->position == splitter->size - 1)
        {
            strcpy(output, "10 1 2 3 4 5 6 7 8 9 10\n");
            return;
        }
        number_count++;
        splitter->position = tmp_position;
    }
}

size_t space(uint64_t *numbers, int number_count)
{
    int count_size = 0;
    for (int i = 0; i < number_count; i++)
    {
        uint64_t n = numbers[i];
        while (n != 0)
        {
            count_size++;
            n /= 10;
        }
        count_size++; // plus 1 for the space after or for the final \n
    }
    return count_size;
}

uint64_t add(upo_protocol_splitter_t splitter, uint64_t *ptr)
{
    while (isspace(splitter->input_file[*ptr]))
        (*ptr)++;

    char number[64] = {0};
    for (int i = 0; isdigit(splitter->input_file[*ptr]); i++, (*ptr)++)
        number[i] = splitter->input_file[*ptr];

    return strtoul(number, NULL, 10);
}

bool is_valid_input(char *input)
{
    for (int i = 0; input[i] != '\0'; i++)
        if (!isdigit(input[i]) && !isspace(input[i]))
            return false;
    return true;
}
