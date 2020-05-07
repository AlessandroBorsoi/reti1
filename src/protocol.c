#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <upo/protocol.h>

upo_protocol_response_t upo_protocol(upo_store_t store, const char *input, char *output)
{
    char input_copy[UPO_PROTOCOL_MAX];
    strcpy(input_copy, input);

    char delim[] = " ";
    char *token = strtok(input_copy, delim);

    if (token == NULL || strcmp(token, "\n") == 0)
    {
        strcpy(output, "ERR SYNTAX Messaggio vuoto\n");
        return ERR_SYNTAX;
    }

    char *endptr = NULL;
    uint64_t size = strtoul(token, &endptr, 10);
    if (token == endptr || errno != 0)
    {
        strcpy(output, "ERR SYNTAX Numero non valido\n");
        return ERR_SYNTAX;
    }

    uint64_t i = 0;
    while (token != NULL && i < size)
    {
        token = strtok(NULL, delim);
        uint64_t data = strtoul(token, &endptr, 10);
        if (token == endptr || errno != 0)
        {
            strcpy(output, "ERR SYNTAX Numero non valido\n");
            return ERR_SYNTAX;
        }
        upo_store_insert(store, data);
        i++;    
    }
    if (strcmp(token, "\n") != 0)
    {
        strcpy(output, "ERR SYNTAX Mancanza del carattere di terminazione\n");
        return ERR_SYNTAX;
    }

    strcpy(output, "TODO\n");
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
