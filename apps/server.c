#include <stdio.h>
#include <stdbool.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <upo/store.h>

#define MAX 512

const char WELCOME[] = "OK START Benvenuto, mandami i tuoi dati\n";

void program(int socket)
{
    upo_store_t store = upo_store_create();
    char input[MAX];
    write(socket, WELCOME, sizeof(WELCOME));
    while (1)
    {
        memset(input, '\0', MAX);
        read(socket, input, sizeof(input));
        if (false)
        {
            char err[] = "ERR SYNTAX Comando non valido\n";
            write(socket, err, sizeof(err));
            close(socket);
            upo_store_destroy(store);
            return;
        }

        char *ptr;
        char *token = strtok(input, " ");
        uint64_t size = strtoul(token, &ptr, 10);
        if (size == 0 && errno != 0 )
        {
            char ok[100] = {0};
            snprintf(ok, 100, "OK STATS %zu %f %f\n", upo_store_size(store), upo_store_avg(store), upo_store_variance(store));
            write(socket, ok, sizeof(ok));
            close(socket);
            upo_store_destroy(store);
            return;
        }
        uint64_t numbers[size];
        uint64_t i = 0;
        while (token != NULL && errno != 0)
        {
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                if (i == size)
                {
                    char err[] = "ERR SYNTAX Comando non valido\n";
                    write(socket, err, sizeof(err));
                    close(socket);
                    upo_store_destroy(store);
                    return;
                }
                numbers[i] = strtoul(token, &ptr, 10);
                i++;
            }
        }
        if (i != size || errno == 0)
        {
            char err[] = "ERR SYNTAX Comando non valido\n";
            write(socket, err, sizeof(err));
            close(socket);
            upo_store_destroy(store);
            return;
        }

        for (uint64_t i = 0; i < size; i++)
            upo_store_insert(store, numbers[i]);
        char ok[100] = {0};
        snprintf(ok, 100, "OK DATA %llu\n", size);
        write(socket, ok, sizeof(ok));
    }
    upo_store_destroy(store);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in serverAddress;

    if (2 != argc)
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket < 0)
    {
        fprintf(stderr, "Could not create a socket!\n");
        exit(1);
    }
    else
    {
        fprintf(stderr, "Socket created!\n");
    }

    /* retrieve the port number for listening */
    int port = atoi(argv[1]);

    /* setup the address structure */
    /* use INADDR_ANY to bind to all local addresses  */
    memset(&serverAddress, '\0', sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(port);

    /*  bind to the address and port with our socket  */
    int bindStatus = bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    if (bindStatus < 0)
    {
        fprintf(stderr, "Could not bind to address!\n");
        close(serverSocket);
        exit(1);
    }
    else
    {
        fprintf(stderr, "Bind completed!\n");
    }

    /* lets listen on the socket for connections      */
    int listenStatus = listen(serverSocket, 5);

    if (listenStatus == -1)
    {
        fprintf(stderr, "Cannot listen on socket!\n");
        close(serverSocket);
        exit(1);
    }

    while (1)
    {
        struct sockaddr_in clientAddress = {0};
        socklen_t clientAddressLength = sizeof(clientAddress);

        int childSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);

        if (childSocket == -1)
        {
            fprintf(stderr, "Cannot accept connections!\n");
            close(serverSocket);
            exit(1);
        }

        program(childSocket);
        close(childSocket);
    }
    close(serverSocket);
    return 0;
}
