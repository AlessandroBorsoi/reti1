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

void program(int socket)
{
    char delim[] = " \n";
    char input[UPO_PROTOCOL_MAX];
    char client[UPO_PROTOCOL_MAX];
    char output[UPO_PROTOCOL_MAX];
    upo_protocol_splitter_t splitter;

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
            printf("%s", &input[9]);
            printf("=====\nTODO: Descrizione delle modalità di utilizzo\n('q' per uscire o path di un file di numeri separati da spazi da mandare al server)\n=====\n");
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
            upo_protocol_splitter_next(splitter, output, UPO_PROTOCOL_MAX);
            write(socket, output, sizeof(output));
            break;
        case OK_DATA:
            upo_protocol_splitter_next(splitter, output, UPO_PROTOCOL_MAX);
            printf("%s", &input[8]);
            write(socket, output, sizeof(output));
            break;
        case OK_STATS:
            printf("TODO: Pretty print dei risultati\n");
            printf("%s", &input[9]);
            return;
        case ERR_DATA:
            printf("Errore dati da parte del server:\n");
            printf("%s", &input[9]);
            return;
        case ERR_STATS:
            printf("Errore di calcolo statistiche da parte del server:\n");
            printf("%s", &input[10]);
            return;
        case ERR_SYNTAX:
            printf("Errore di sintassi da parte del server:\n");
            printf("%s", &input[11]);
            return;
        case INVALID:
            printf("Qualcosa è andato storto in maniera imprevista:\n");
            printf("%s\n", input);
            return;
        default:
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
