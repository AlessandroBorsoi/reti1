#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <upo/protocol.h>

upo_protocol_response_t upo_protocol(upo_store_t store, const char *input, char *output)
{
    errno = 0;
    char delim[] = " \n";
    char input_copy[UPO_PROTOCOL_MAX];
    strcpy(input_copy, input);

    char *token = strtok(input_copy, delim);

    if (token == NULL || strcmp(token, "\n") == 0)
    {
        strcpy(output, "ERR SYNTAX Messaggio vuoto\n");
        return ERR_SYNTAX;
    }

    char *endptr = NULL;
    uint64_t size = strtoul(token, &endptr, 10);
    if (token == endptr || errno != 0 || (errno == 0 && token && *endptr != 0))
    {
        strcpy(output, "ERR SYNTAX Numero non valido\n");
        return ERR_SYNTAX;
    }

    token = strtok(NULL, delim);
    if (token == NULL)
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
        snprintf(output, UPO_PROTOCOL_MAX, "OK STATS %zu %.2f %.2f\n", store_size, upo_store_avg(store), upo_store_variance(store));
        return OK_STATS;
    }

    while (token != NULL)
    {
        errno = 0;
        uint64_t data = strtoul(token, &endptr, 10);
        if (token == endptr || errno != 0 || (errno == 0 && token && *endptr != 0))
        {
            strcpy(output, "ERR SYNTAX Numero non valido\n");
            return ERR_SYNTAX;
        }
        token = strtok(NULL, delim);
    }

    snprintf(output, UPO_PROTOCOL_MAX, "OK DATA %llu\n", size);
    return OK_DATA;
}

// char *ptr;
// char *token = strtok(input, " ");
// uint64_t size = strtoul(token, &ptr, 10);
// if (size == 0 && errno != 0 )
// {
//     char ok[100] = {0};
//     snprintf(ok, 100, "OK STATS %zu %f %f\n", upo_store_size(store), upo_store_avg(store), upo_store_variance(store));
//     write(socket, ok, sizeof(ok));
//     close(socket);
//     upo_store_destroy(store);
//     return;
// }
// uint64_t numbers[size];
// uint64_t i = 0;
// while (token != NULL && errno != 0)
// {
//     token = strtok(NULL, " ");
//     if (token != NULL)
//     {
//         if (i == size)
//         {
//             char err[] = "ERR SYNTAX Comando non valido\n";
//             write(socket, err, sizeof(err));
//             close(socket);
//             upo_store_destroy(store);
//             return;
//         }
//         numbers[i] = strtoul(token, &ptr, 10);
//         i++;
//     }
// }
// if (i != size || errno == 0)
// {
//     char err[] = "ERR SYNTAX Comando non valido\n";
//     write(socket, err, sizeof(err));
//     close(socket);
//     upo_store_destroy(store);
//     return;
// }

// for (uint64_t i = 0; i < size; i++)
//     upo_store_insert(store, numbers[i]);
// char ok[100] = {0};
// snprintf(ok, 100, "OK DATA %llu\n", size);
// write(socket, ok, sizeof(ok));
