#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <upo/protocol.h>
#include <upo/splitter.h>

typedef struct ok_stats_s
{
    int total;
    double mean;
    double variance;
} ok_stats_t;

ok_stats_t get_stats(char *response)
{
    ok_stats_t stats = {0, 0, 0};
    char *delim = " \n";
    char copy[UPO_PROTOCOL_MAX];
    strcpy(copy, response);
    char *status = strtok(copy, delim);
    char *type = strtok(NULL, delim);
    if (strcmp(status, "OK") == 0 && strcmp(type, "STATS") == 0)
    {
        stats.total = atoi(strtok(NULL, delim));
        stats.mean = atof(strtok(NULL, delim));
        stats.variance = atof(strtok(NULL, delim));
    }
    return stats;
}

upo_protocol_response_t parse(char *input)
{
    char delim[] = " \n";
    char input_copy[UPO_PROTOCOL_MAX];
    strcpy(input_copy, input);

    char *status = strtok(input_copy, delim);
    if (status == NULL)
        return INVALID;

    char *type = strtok(NULL, delim);
    if (type == NULL)
        return INVALID;

    if (strcmp(status, "OK") == 0 && strcmp(type, "START") == 0)
        return OK_START;
    if (strcmp(status, "OK") == 0 && strcmp(type, "DATA") == 0)
        return OK_DATA;
    if (strcmp(status, "OK") == 0 && strcmp(type, "STATS") == 0)
        return OK_STATS;
    if (strcmp(status, "ERR") == 0 && strcmp(type, "DATA") == 0)
        return ERR_DATA;
    if (strcmp(status, "ERR") == 0 && strcmp(type, "STATS") == 0)
        return ERR_STATS;
    if (strcmp(status, "ERR") == 0 && strcmp(type, "SYNTAX") == 0)
        return ERR_SYNTAX;
    return INVALID;
}

int trim_response(upo_protocol_response_t response)
{
    switch (response)
    {
    case OK_DATA:
        return 8;
    case OK_START:
    case OK_STATS:
    case ERR_DATA:
        return 9;
    case ERR_STATS:
        return 10;
    case ERR_SYNTAX:
        return 11;
    default:
        return 0;
    }
}

void program(int socket)
{
    char delim[] = " \n";
    char input[UPO_PROTOCOL_MAX];
    char client[UPO_PROTOCOL_MAX];
    char output[UPO_PROTOCOL_MAX];
    upo_protocol_splitter_t splitter;
    size_t sent = 0;

    while (1)
    {
        memset(input, '\0', UPO_PROTOCOL_MAX);
        memset(client, '\0', UPO_PROTOCOL_MAX);
        memset(output, '\0', UPO_PROTOCOL_MAX);
        read(socket, input, sizeof(input));
        upo_protocol_response_t response = parse(input);
        switch (response)
        {
        case OK_START:
            printf("%s", &input[trim_response(response)]);
            printf("Lo scopo di questo programma è inviare al server una sequenza di numeri interi positivi\n");
            printf("in modo che vengano restituiti i valori di media e varianza campionaria.\n");
            printf("Per inviare i valori occorre digitare il percorso (assoluto o relativo) di un file di testo\n");
            printf("contenente la sequenza di numeri separata da caratteri bianchi (spazi, a capo, tab...).\n");
            printf("Ad esempio digitare (senza virgolette): 'data/example.txt'.\n");
            printf("Per uscire dal programma senza inviare nulla digitare q.\n");
            int error;
            do
            {
                error = 0;
                int n = 0;
                while ((client[n++] = getchar()) != '\n')
                    ;

                char *client_input = strtok(client, delim);
                if (strcmp(client_input, "q") == 0)
                {
                    printf("Arrivederci\n");
                    return;
                }

                splitter = upo_protocol_splitter_create(client_input);

                if (splitter == NULL)
                {
                    printf("Impossibile accedere al file\n");
                    error = 1;
                    continue;
                }

                if (!upo_protocol_splitter_is_valid(splitter))
                {
                    printf("Il file contiene dati non corretti. Sono ammessi solo numeri interi positivi separati da spazi\n");
                    error = 1;
                    continue;
                }
            } while (error);
            sent = upo_protocol_splitter_next(splitter, output, UPO_PROTOCOL_MAX);
            write(socket, output, sizeof(output));
            break;
        case OK_DATA:
            sent = upo_protocol_splitter_next(splitter, output, UPO_PROTOCOL_MAX);
            write(socket, output, sizeof(output));
            break;
        case OK_STATS:
        {
            ok_stats_t stats = get_stats(input);
            printf("Sono stati inviati al server un totale di %d dati\n", stats.total);
            printf("La media campionaria calcolata è: %.2f\n", stats.mean);
            printf("La varianza campionaria calcolata è: %.2f\n", stats.variance);
            return;
        }
        case ERR_DATA:
            printf("Errore dati da parte del server:\n");
            printf("%s", &input[trim_response(response)]);
            return;
        case ERR_STATS:
            printf("Errore di calcolo statistiche da parte del server:\n");
            printf("%s", &input[trim_response(response)]);
            return;
        case ERR_SYNTAX:
            printf("Errore di sintassi da parte del server:\n");
            printf("%s", &input[trim_response(response)]);
            return;
        default:
            printf("Qualcosa è andato storto in maniera imprevista:\n");
            printf("%s\n", input);
            return;
        }
    }
    upo_protocol_splitter_destroy(&splitter);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in serverAddress;

    if (3 != argc)
    {
        fprintf(stderr, "Usage: %s <server> <port>\n", argv[0]);
        exit(1);
    }

    int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket == -1)
    {
        fprintf(stderr, "Could not create a socket!\n");
        exit(1);
    }

    int port = atoi(argv[2]);

    memset(&serverAddress, '\0', sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddress.sin_port = htons(port);

    int connectStatus = connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    if (connectStatus != 0)
    {
        fprintf(stderr, "Could not connect to address!\n");
        close(clientSocket);
        exit(1);
    }

    program(clientSocket);
    close(clientSocket);
    return 0;
}
