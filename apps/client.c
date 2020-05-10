#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 512

void program(int socket)
{
    char input[MAX];
    char output[MAX];
    while (1)
    {
        memset(input, '\0', MAX);
        memset(output, '\0', MAX);
        read(socket, input, sizeof(input));
        printf("%s", input);
        int n = 0;
        while ((output[n++] = getchar()) != '\n')
            ;
        write(socket, output, sizeof(output));
    }
}

int main(int argc, char *argv[])
{
    struct sockaddr_in serverAddress;

    if (3 != argc)
    {
        fprintf(stderr, "Usage: %s <server> <port>\n", argv[0]);
        exit(1);
    }

    /* create a streaming socket      */
    int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket == -1)
    {
        fprintf(stderr, "Could not create a socket!\n");
        exit(1);
    }
    else
    {
        fprintf(stderr, "Socket created!\n");
    }

    /* retrieve the port number for connecting */
    int port = atoi(argv[2]);

    /* setup the address structure */
    /* use the IP address sent as an argument for the server address  */
    memset(&serverAddress, '\0', sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddress.sin_port = htons(port);

    /*  connect to the address and port with our socket  */
    int connectStatus = connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    if (connectStatus == 0)
    {
        fprintf(stderr, "Connect successful!\n");
    }
    else
    {
        fprintf(stderr, "Could not connect to address!\n");
        close(clientSocket);
        exit(1);
    }

    program(clientSocket);
    close(clientSocket);
    return 0;
}
