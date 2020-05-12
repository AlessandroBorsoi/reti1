#include <stdio.h>
#include <assert.h>
#include <upo/splitter.h>

static void test_create_destroy();

int main()
{
    printf("Test case 'create/destroy'... ");
    fflush(stdout);
    test_create_destroy();
    printf("OK\n");

    return 0;
}

void test_create_destroy()
{
    upo_protocol_splitter_t splitter = upo_protocol_splitter_create("");

    assert(splitter == NULL);

    upo_protocol_splitter_destroy(&splitter);

    splitter = upo_protocol_splitter_create("test_splitter.c");

    assert(splitter != NULL);

    assert(!upo_protocol_splitter_is_valid(splitter));

    upo_protocol_splitter_destroy(&splitter);

    splitter = upo_protocol_splitter_create("/Users/aborsoi/formazione/reti1/data/example.txt");

    assert(splitter != NULL);

    assert(upo_protocol_splitter_is_valid(splitter));

    upo_protocol_splitter_destroy(&splitter);
}
