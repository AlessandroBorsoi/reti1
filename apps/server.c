#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 512

const char MESSAGE[] = "Hello UPO student!\n";

void func(int sockfd) 
{ 
    char buff[MAX]; 
    int n; 
    // infinite loop for chat 
    for (;;) { 
        bzero(buff, MAX); 
  
        // read the message from client and copy it in buffer 
        read(sockfd, buff, sizeof(buff)); 
        // print buffer which contains the client contents 
        printf("From client: %s\t To client : ", buff); 
        bzero(buff, MAX); 
        n = 0; 
        // copy server message in the buffer 
        while ((buff[n++] = getchar()) != '\n') 
            ; 
  
        // and send that buffer to client 
        write(sockfd, buff, sizeof(buff)); 
  
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } 
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

    struct sockaddr_in clientAddress = {0};
    socklen_t clientAddressLength = sizeof(clientAddress);

    /* wait here */

    int childSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);

    if (childSocket == -1)
    {
        fprintf(stderr, "Cannot accept connections!\n");
        close(serverSocket);
        exit(1);
    }

    func(childSocket); 
    close(childSocket);

    close(serverSocket);
    return 0;
}
