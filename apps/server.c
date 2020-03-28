#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wordexp.h>

#define MAX 512

typedef struct hist_s
{
    unsigned int digits[10];
    unsigned int uppers[26];
    unsigned int lowers[26];
} hist_t;

void insert(hist_t *hist, char **wordsv, int wordsc)
{
    for (int i = 0; i < wordsc; i++)
    {
        for (size_t j = 0; j < strlen(wordsv[i]); j++)
        {
            char c = wordsv[i][j];
            if (isdigit(c))
            {
                hist->digits[c - '0']++;
            }
            else if (isupper(c))
            {
                hist->uppers[c - 'A']++;
            }
            else if (islower(c))
            {
                hist->lowers[c - 'a']++;
            }
        }
    }
}

char *toString(hist_t *hist)
{
    int pos = 0;
    char *res = calloc(500, sizeof(char));
    char number = 0;
    for (int i = 0; i < 10; i++)
    {
        number = hist->digits[i];
        if (number > 0)
            pos += sprintf(&res[pos], "%c:%d ", i + '0', number);
    }
    for (int i = 0; i < 26; i++)
    {
        number = hist->uppers[i];
        if (number > 0)
            pos += sprintf(&res[pos], "%c:%d ", i + 'A', number);
    }
    for (int i = 0; i < 26; i++)
    {
        number = hist->lowers[i];
        if (number > 0)
            pos += sprintf(&res[pos], "%c:%d ", i + 'a', number);
    }
    return res;
}

const char MESSAGE[] = "OK START Benvenuto, mandami i tuoi dati\n";

void print(wordexp_t *message)
{
    for (size_t i = 0; i < message->we_wordc; i++)
        printf("%s\n", message->we_wordv[i]);
}

bool isValidCommandType(wordexp_t *message)
{
    char *command = message->we_wordv[0];
    return (strcmp(command, "TEXT") == 0 ||
            strcmp(command, "HIST") == 0 ||
            strcmp(command, "EXIT") == 0 ||
            strcmp(command, "QUIT") == 0);
}

int countAlphanum(char **words, int wordNumber)
{
    int count = 0;
    for (int i = 0; i < wordNumber; i++)
        for (size_t j = 0; j < strlen(words[i]); j++)
            if (isalnum(words[i][j]))
                count++;
    return count;
}

void func(int socket)
{
    char input[MAX];
    write(socket, MESSAGE, sizeof(MESSAGE));
    hist_t hist = {0};
    while (1)
    {
        wordexp_t message;
        bzero(input, MAX);
        read(socket, input, sizeof(input));
        if (strcmp(input, "\n") == 0) continue;
        char *mex = strtok(input, "\n"); // TODO: fix segmentation fault when the input is only the \n
        switch (wordexp(mex, &message, 0))
        {
        case 0:
            break;
        case WRDE_NOSPACE:
            printf("WRDE_NOSPACE\n");
            wordfree(&message);
            return;
        case WRDE_BADCHAR:
            printf("WRDE_BADCHAR\n");
            break;
        case WRDE_BADVAL:
            printf("WRDE_BADVAL\n");
            break;
        case WRDE_CMDSUB:
            printf("WRDE_CMDSUB\n");
            break;
        case WRDE_SYNTAX:
            printf("WRDE_SYNTAX\n");
            break;
        default:
            printf("Error\n");
            wordfree(&message);
            return;
        }
        if (!isValidCommandType(&message))
        {
            char err[] = "ERR SYNTAX Comando non valido\n";
            write(socket, err, sizeof(err));
            wordfree(&message);
            close(socket);
            return;
        }
        char *command = message.we_wordv[0];
        if (strcmp(command, "TEXT") == 0)
        {
            if (message.we_wordc < 3)
            {
                char err[] = "ERR TEXT Comando non valido\n";
                write(socket, err, sizeof(err));
                wordfree(&message);
                return;
            }
            int number = atoi(message.we_wordv[message.we_wordc - 1]);
            if (number == 0)
            {
                char err[] = "ERR TEXT Comando non valido\n";
                write(socket, err, sizeof(err));
                wordfree(&message);
                return;
            }
            int charCount = countAlphanum(&message.we_wordv[1], message.we_wordc - 2);
            if (number != charCount)
            {
                char err[] = "ERR TEXT Count non valido\n";
                write(socket, err, sizeof(err));
                wordfree(&message);
                return;
            }
            else
            {
                char ok[12];
                snprintf(ok, 12, "OK TEXT %d\n", number);
                write(socket, ok, sizeof(ok));
            }
            insert(&hist, &message.we_wordv[1], message.we_wordc - 2);
        }
        else if (strcmp(command, "HIST") == 0)
        {
            char ok[MAX];
            char *res = toString(&hist);
            snprintf(ok, MAX, "OK HIST %s\n", res);
            write(socket, ok, sizeof(ok));
            free(res);
        }
        else if (strcmp(command, "EXIT") == 0)
        {
            char ok[MAX];
            char *res = toString(&hist);
            snprintf(ok, MAX, "OK HIST %s\n", res);
            write(socket, ok, sizeof(ok));
            char exit[] = "OK EXIT Arrivederci\n";
            write(socket, exit, sizeof(exit));
            free(res);
            wordfree(&message);
            return;
        }
        else
        {
            char *quit = "OK QUIT Arrivederci\n";
            write(socket, quit, sizeof(quit));
            wordfree(&message);
            return;
        }

        wordfree(&message);
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

        func(childSocket);
        close(childSocket);
    }
    close(serverSocket);
    return 0;
}
