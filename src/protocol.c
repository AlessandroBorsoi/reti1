#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <inttypes.h>
#include <upo/server/protocol.h>

static bool is_invalid_number(char *str, uint64_t number, char *endptr)
{
    return str == endptr || errno != 0 || number == ULONG_MAX || (errno == 0 && str && *endptr != 0);
}

upo_protocol_response_t upo_protocol(upo_store_t store, const char *input, char *output)
{
    errno = 0;
    char delim[] = " \n";
    char input_copy[UPO_PROTOCOL_MAX + 1];
    strcpy(input_copy, input);

    char *token = strtok(input_copy, delim);
    if (token == NULL)
    {
        strcpy(output, "ERR SYNTAX Messaggio vuoto\n");
        return ERR_SYNTAX;
    }

    char *terminator = strchr(input, '\n');
    if (terminator == NULL)
    {
        strcpy(output, "ERR SYNTAX Mancanza del carattere di terminazione\n");
        return ERR_SYNTAX;
    }
    if (terminator[1] != '\0')
    {
        strcpy(output, "ERR SYNTAX Messaggio non valido\n");
        return ERR_SYNTAX;
    }

    char *endptr = NULL;
    uint64_t size = strtoul(token, &endptr, 10);
    if (is_invalid_number(token, size, endptr))
    {
        strcpy(output, "ERR SYNTAX Numero non valido\n");
        return ERR_SYNTAX;
    }

    token = strtok(NULL, delim);
    if (size == 0 && token == NULL)
    {
        size_t store_size = upo_store_size(store);
        if (store_size == 0)
        {
            strcpy(output, "ERR STATS Non posso calcolare la media di nessun campione\n");
            return ERR_STATS;
        }
        if (store_size == 1)
        {
            strcpy(output, "ERR STATS Non posso calcolare la varianza di 1 campione\n");
            return ERR_STATS;
        }
        snprintf(output, UPO_PROTOCOL_MAX, "OK STATS %zu %.2f %.2f\n", store_size, upo_store_sample_mean(store), upo_store_sample_variance(store));
        return OK_STATS;
    }

    uint64_t counter = 0;
    uint64_t numbers[size];
    while (token != NULL)
    {
        errno = 0;
        uint64_t data = strtoul(token, &endptr, 10);
        if (is_invalid_number(token, data, endptr))
        {
            strcpy(output, "ERR SYNTAX Numero non valido\n");
            return ERR_SYNTAX;
        }
        if (counter == size)
        {
            strcpy(output, "ERR DATA Il numero di dati immessi è maggiore della dimensione dichiarata\n");
            return ERR_DATA;
        }
        numbers[counter] = data;
        token = strtok(NULL, delim);
        counter++;
    }

    if (counter < size)
    {
        strcpy(output, "ERR DATA Il numero di dati immessi è minore della dimensione dichiarata\n");
        return ERR_DATA;
    }

    for (uint64_t i = 0; i < size; i++)
        upo_store_insert(store, numbers[i]);

    snprintf(output, UPO_PROTOCOL_MAX, "OK DATA %" PRIu64 "\n", size);
    return OK_DATA;
}
