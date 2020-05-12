#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <upo/splitter.h>

static void test_create_destroy();
static void get_resource(char *res_path, char *argv0, char *buffer);

int main(int argc, char const *argv[])
{
    printf("Test case 'create/destroy'... ");
    fflush(stdout);
    test_create_destroy(argv[0]);
    printf("OK\n");

    return 0;
}

void test_create_destroy(char *arg0)
{
    char resource[512];
    upo_protocol_splitter_t splitter = upo_protocol_splitter_create("");

    assert(splitter == NULL);

    upo_protocol_splitter_destroy(&splitter);

    get_resource("test_splitter.c", arg0, resource);
    splitter = upo_protocol_splitter_create(resource);

    assert(splitter != NULL);

    assert(!upo_protocol_splitter_is_valid(splitter));

    upo_protocol_splitter_destroy(&splitter);

    get_resource("../data/example.txt", arg0, resource);
    splitter = upo_protocol_splitter_create(resource);

    assert(splitter != NULL);

    assert(upo_protocol_splitter_is_valid(splitter));

    upo_protocol_splitter_destroy(&splitter);
}

void get_resource(char *res_path, char *argv0, char *buffer)
{
    memset(buffer, 0, 512);
    getcwd(buffer, 512);

    strcat(buffer, "/");

    char tmp[512] = {0};
    strcpy(tmp, argv0);
    char *token = strtok(tmp, "/");
    while (token != NULL && strcmp(token, "test_splitter") != 0)
    {
        if (strcmp(token, ".") != 0)
        {
            strcat(buffer, token);
            strcat(buffer, "/");
        }
        token = strtok(NULL, "/");
    }

    strcat(buffer, res_path);
}
