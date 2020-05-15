#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <upo/client/splitter.h>

char resource[512];
static void test_create_destroy(const char *arg0);
static void test_next_with_example_txt(const char *arg0);
static void test_next_with_big_example_txt(const char *arg0);
static void test_next_test_txt(const char *arg0);
static void get_resource(char *resource, const char *argv0, char *buffer);
static void populate_file(char *resource);
static void clean_file(char *resource);

int main(int argc, char const *argv[])
{
    (void)argc; // To silence compiler warning: unused parameter 'argc' [-Wunused-parameter]
    printf("Test case 'create/destroy'... ");
    fflush(stdout);
    test_create_destroy(argv[0]);
    printf("OK\n");

    printf("Test case 'next'... ");
    fflush(stdout);
    test_next_with_example_txt(argv[0]);
    test_next_with_big_example_txt(argv[0]);
    test_next_test_txt(argv[0]);
    printf("OK\n");

    return 0;
}

void test_create_destroy(const char *arg0)
{
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

void test_next_with_example_txt(const char *arg0)
{
    get_resource("../data/example.txt", arg0, resource);
    upo_protocol_splitter_t splitter = upo_protocol_splitter_create(resource);

    char output[30] = {0};
    upo_protocol_splitter_next(splitter, output, 30);

    assert(strcmp(output, "10 1 2 3 4 5 6 7 8 9 10\n") == 0);

    upo_protocol_splitter_next(splitter, output, 30);

    assert(strcmp(output, "0\n") == 0);

    upo_protocol_splitter_destroy(&splitter);

    assert(splitter == NULL);

    splitter = upo_protocol_splitter_create(resource);

    assert(splitter != NULL);

    char output2[10] = {0};
    upo_protocol_splitter_next(splitter, output2, 10);

    assert(strcmp(output2, "4 1 2 3 4\n") == 0);

    upo_protocol_splitter_next(splitter, output2, 10);

    assert(strcmp(output2, "4 5 6 7 8\n") == 0);

    upo_protocol_splitter_next(splitter, output2, 10);

    assert(strcmp(output2, "2 9 10\n") == 0);

    upo_protocol_splitter_next(splitter, output2, 10);

    assert(strcmp(output2, "0\n") == 0);

    upo_protocol_splitter_destroy(&splitter);
}

void test_next_with_big_example_txt(const char *arg0)
{
    get_resource("../data/big_example.txt", arg0, resource);

    populate_file(resource);

    upo_protocol_splitter_t splitter = upo_protocol_splitter_create(resource);

    assert(splitter != NULL);

    assert(upo_protocol_splitter_is_valid(splitter));

    char output[512] = {0};

    int i = 0;
    while (strcmp(output, "0\n") != 0)
    {
        upo_protocol_splitter_next(splitter, output, 512);
        i++;
        if (i == 13669)
            assert(strcmp(output, "14 999987 999988 999989 999990 999991 999992 999993 999994 999995 999996 999997 999998 999999 1000000\n") == 0);
    }

    assert(i == 13670);

    upo_protocol_splitter_destroy(&splitter);

    clean_file(resource);
}

void test_next_test_txt(const char *arg0)
{
    get_resource("../data/test.txt", arg0, resource);
    upo_protocol_splitter_t splitter = upo_protocol_splitter_create(resource);

    char output[512] = {0};
    upo_protocol_splitter_next(splitter, output, 512);
    upo_protocol_splitter_next(splitter, output, 512);

    assert(strcmp(output, "1 1035\n") == 0);

    upo_protocol_splitter_next(splitter, output, 512);

    assert(strcmp(output, "0\n") == 0);

    upo_protocol_splitter_destroy(&splitter);
}

void get_resource(char *resource, const char *argv0, char *buffer)
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

    strcat(buffer, resource);
}

void populate_file(char *resource)
{
    FILE *fptr = fopen(resource, "w");
    for (int i = 1; i <= 1000000; i++)
        fprintf(fptr, "%d ", i);
    fclose(fptr);
}

void clean_file(char *resource)
{
    FILE *fptr = fopen(resource, "w");
    fprintf(fptr, "");
    fclose(fptr);
}
