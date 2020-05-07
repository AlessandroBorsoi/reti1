#include <string.h>
#include <upo/protocol.h>

void upo_protocol(upo_store_t store, const char *input, char *output, upo_protocol_response_t *response)
{
    strcpy(output, "ERR SYNTAX Messaggio vuoto\n");
    *response = ERR_SYNTAX;
}

// if (false)
// {
//     char err[] = "ERR SYNTAX Comando non valido\n";
//     write(socket, err, sizeof(err));
//     close(socket);
//     upo_store_destroy(store);
//     return;
// }
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
