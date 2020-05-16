#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <upo/client/splitter.h>

typedef struct ok_stats_s
{
    int total;
    double mean;
    double variance;
} ok_stats_t;

ok_stats_t get_ok_stats(char *response)
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

int get_ok_data(char *response)
{
    char *delim = " \n";
    char copy[UPO_PROTOCOL_MAX];
    strcpy(copy, response);
    char *status = strtok(copy, delim);
    char *type = strtok(NULL, delim);
    if (strcmp(status, "OK") == 0 && strcmp(type, "DATA") == 0)
        return atoi(strtok(NULL, delim));
    return -1;
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

const char *to_string(upo_protocol_response_t response)
{
    switch (response)
    {
    case OK_START:
        return "OK START";
    case OK_DATA:
        return "OK DATA";
    case OK_STATS:
        return "OK STATS";
    case ERR_SYNTAX:
        return "ERR SYNTAX";
    case ERR_DATA:
        return "ERR DATA";
    case ERR_STATS:
        return "ERR STATS";
    default:
        return "INVALID";
    }
}

bool client_input(upo_protocol_splitter_t *splitter)
{
    printf("\n");
    printf("Lo scopo di questo programma è inviare al server una sequenza di numeri interi positivi\n");
    printf("in modo che vengano restituiti i valori di media e varianza campionaria calcolati sull'insieme.\n");
    printf("Per inviare i valori occorre digitare il percorso (assoluto o relativo) di un file di testo\n");
    printf("contenente la sequenza di numeri separata da spazi.\n");
    printf("Ad esempio digitare (senza virgolette): 'data/example.txt'.\n");
    printf("Per uscire dal programma senza inviare nulla digitare q.\n");
    printf("\n");

    int error;
    char delim[] = " \n";
    char client[UPO_PROTOCOL_MAX];
    do
    {
        memset(client, '\0', UPO_PROTOCOL_MAX);
        error = 0;
        int n = 0;
        while ((client[n++] = getchar()) != '\n')
            ;

        char *client_input = strtok(client, delim);
        if (client_input == NULL)
        {
            error = 1;
            continue;
        }
        if (strcmp(client_input, "q") == 0)
        {
            printf("Arrivederci\n");
            return true;
        }

        *splitter = upo_protocol_splitter_create(client_input);
        if (*splitter == NULL)
        {
            printf("Impossibile accedere al file\n");
            error = 1;
            continue;
        }

        if (!upo_protocol_splitter_is_valid(*splitter))
        {
            printf("Il file contiene dati non corretti. Sono ammessi solo numeri interi positivi separati da spazi\n");
            error = 1;
            continue;
        }
    } while (error);
    return false;
}

void program(int socket)
{
    char input[UPO_PROTOCOL_MAX] = {0};
    char output[UPO_PROTOCOL_MAX] = {0};

    read(socket, input, sizeof(input));
    upo_protocol_response_t response = parse(input);
    if (response != OK_START)
    {
        printf("Il server non risponde correttamente\n");
        return;
    }

    printf("%s", &input[trim_response(response)]);
    upo_protocol_splitter_t splitter = NULL;
    bool end = client_input(&splitter);

    while (!end)
    {
        size_t sent = upo_protocol_splitter_next(splitter, output, UPO_PROTOCOL_MAX);
        
        write(socket, output, sizeof(output));
        memset(output, '\0', UPO_PROTOCOL_MAX);
        memset(input, '\0', UPO_PROTOCOL_MAX);
        read(socket, input, sizeof(input));

        upo_protocol_response_t response = parse(input);
        if (response == OK_DATA)
        {
            size_t received = get_ok_data(input);
            if (sent != received)
            {
                printf("Si è verificata una discrepanza tra il numero di dati inviati (%zu) e il numero di dati ricevuti dal server (%zu)\n", sent, received);
                strcpy(output, "0\n");
            }
        }
        else if (response == OK_STATS)
        {
            ok_stats_t stats = get_ok_stats(input);
            printf("\n");
            printf("Sono stati inviati al server dati per un totale di: %d\n", stats.total);
            printf("La media campionaria calcolata è: %.2f\n", stats.mean);
            printf("La varianza campionaria calcolata è: %.2f\n", stats.variance);
            printf("\n");
            end = true;
        }
        else
        {
            printf("Errore di tipo %s da parte del server:\n", to_string(response));
            printf("%s", &input[trim_response(response)]);
            printf("\n");
            end = true;
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
