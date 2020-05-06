#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 512

const char WELCOME[] = "OK START Benvenuto, mandami i tuoi dati\n";

typedef struct upo_list_s
{
    uint64_t list[100];
    uint64_t size;
} upo_list_t;

void upo_list_add(upo_list_t *list, uint64_t number)
{
    list->list[list->size] = number;
    list->size++;
}

uint64_t upo_list_size(upo_list_t *list)
{
    return list->size;
}

double upo_list_avg(upo_list_t *list)
{
    if (list->size == 0)
        return 0;
    else
    {
        uint64_t sum = 0;
        for (uint64_t i = 0; i < list->size; i++)
        {
            sum += list->list[i];
        }
        return sum / (double)list->size;
    }
}

double upo_list_variance(upo_list_t *list)
{
    return list->size;
}

void program(int socket)
{
    upo_list_t upo_list = {{0}, 0};
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
            return;
        }

        char *ptr;
        char *token = strtok(input, " ");
        uint64_t size = strtoul(token, &ptr, 10);
        if (size == 0)
        {
            char ok[100] = {0};
            snprintf(ok, 100, "OK STATS %llu %f\n", upo_list_size(&upo_list), upo_list_avg(&upo_list));
            write(socket, ok, sizeof(ok));
            close(socket);
            return;
        }
        uint64_t numbers[size];
        int i = 0;
        while (token != NULL)
        {
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                numbers[i] = strtoul(token, &ptr, 10);
                i++;
            }
        }

        for (uint64_t i = 0; i < size; i++)
            upo_list_add(&upo_list, numbers[i]);
    }
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
