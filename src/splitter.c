#include "splitter_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static bool is_valid_input(const char *input);
static int get_number_count(const char *input, int file_size);
static upo_protocol_splitter_t create(char *input, int file_size);
static int numbers_to_send(const upo_protocol_splitter_t splitter, const size_t output_size);

upo_protocol_splitter_t upo_protocol_splitter_create(char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
        return NULL;

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *input = malloc(file_size + 1);
    fread(input, 1, file_size, file);
    fclose(file);
    input[file_size] = 0;

    upo_protocol_splitter_t splitter = create(input, file_size + 1);

    free(input);
    return splitter;
}

void upo_protocol_splitter_destroy(upo_protocol_splitter_t *splitter)
{
    if (splitter != NULL && *splitter != NULL)
    {
        (*splitter)->size = 0;
        (*splitter)->current = 0;
        if (upo_protocol_splitter_is_valid(*splitter))
            free((*splitter)->numbers);
        free(*splitter);
        (*splitter) = NULL;
    }
}

bool upo_protocol_splitter_is_valid(upo_protocol_splitter_t splitter)
{
    if (splitter != NULL)
        return splitter->numbers != NULL;
    return false;
}

void upo_protocol_splitter_next(upo_protocol_splitter_t splitter, char *output, size_t output_size)
{
    memset(output, '\0', output_size);
    if (upo_protocol_splitter_is_valid(splitter) && splitter->current == splitter->size)
    {
        strcpy(output, "0\n");
        return;
    }

    int to_send = numbers_to_send(splitter, output_size);

    int index = 0;
    index += sprintf(&output[index], "%d ", to_send);
    int start = splitter->current;
    int end = start + to_send;
    for (int i = start; i < end; i++)
        if (i == end - 1)
            index += sprintf(&output[index], "%llu\n", splitter->numbers[i]);
        else
            index += sprintf(&output[index], "%llu ", splitter->numbers[i]);
    splitter->current = end;
}

int get_number_count(const char *input, int file_size)
{
    int count = 0;
    for (int i = 0; i < file_size; i++)
    {
        if (isdigit(input[i]))
        {
            count++;
            while (isdigit(input[i]))
                i++;
        }
    }
    return count;
}

upo_protocol_splitter_t create(char *input, int file_size)
{
    upo_protocol_splitter_t splitter = malloc(sizeof(struct upo_protocol_splitter_s));
    splitter->numbers = NULL;
    splitter->size = 0;
    splitter->current = 0;
    if (is_valid_input(input))
    {
        int count = get_number_count(input, file_size);
        splitter->numbers = malloc(count * sizeof(uint64_t));
        splitter->size = count;

        int i = 0;
        char *delim = " \n";
        char *token = strtok(input, delim);
        while (token != NULL)
        {
            splitter->numbers[i] = strtoul(token, NULL, 10);
            token = strtok(NULL, delim);
            i++;
        }
    }
    return splitter;
}

int digits(uint64_t n)
{
    int digits = 1;
    while (n > 9)
    {
        n /= 10;
        digits++;
    }
    return digits;
}

int numbers_to_send(const upo_protocol_splitter_t splitter, const size_t output_size)
{
    int to_send = 0;
    size_t buffer_size = 0;
    int i = splitter->current;
    while (buffer_size + digits(to_send) <= output_size && to_send < splitter->size)
    {
        to_send++;
        buffer_size += digits(splitter->numbers[i]);
        i++;
    }
    return to_send;
}

bool is_valid_input(const char *input)
{
    for (int i = 0; input[i] != '\0'; i++)
        if (!isdigit(input[i]) && !isspace(input[i]))
            return false;
    return true;
}
