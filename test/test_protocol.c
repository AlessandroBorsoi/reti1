/*
I: 2 10 20\n
O: OK DATA 2\n
I: 1 12\n
O: OK DATA 1\n
I: 0\n
O: OK STATS 3 14.0 28.0\n

I: 1 10\n
O: OK DATA 1\n
I: 0\n
O: ERR STATS Non posso calcolare la varianza di 1 campione\n

I: 0\n
O: ERR STATS Non posso calcolare la media di nessun campione\n

I: test\n
O: ERR SYNTAX Numero non valido\n

I: 2 1 test\n
O: ERR SYNTAX Numero non valido\n

I: 2 1 3.5\n
O: ERR SYNTAX Numero non valido\n

I: 2 1 3
O: ERR SYNTAX Mancanza del carattere di terminazione\n

I: 2 1 3 4\n
O: ERR DATA Il numero di dati immessi non è coerente con la dimensione dichiarata\n

I: 6 1 3 4\n
O: ERR DATA Il numero di dati immessi non è coerente con la dimensione dichiarata\n

I: 0 1 3\n
O: ERR DATA Il numero di dati immessi non è coerente con la dimensione dichiarata\n
*/
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <upo/protocol.h>

static void test_err_syntax_empty_message();
static void test_err_data();
static void test_err_stats();
static void test_ok_data();
static void test_ok_stats();

int main()
{
    printf("Test case 'ERR SYNTAX'... ");
    fflush(stdout);
    test_err_syntax_empty_message();
    printf("OK\n");

    printf("Test case 'ERR DATA'... ");
    fflush(stdout);
    test_err_data();
    printf("OK\n");

    printf("Test case 'ERR STATS'... ");
    fflush(stdout);
    test_err_stats();
    printf("OK\n");

    printf("Test case 'OK DATA'... ");
    fflush(stdout);
    test_ok_data();
    printf("OK\n");

    printf("Test case 'OK STATS'... ");
    fflush(stdout);
    test_ok_stats();
    printf("OK\n");

    return 0;
}

/*
I: \n
O: ERR SYNTAX Messaggio vuoto\n
*/
void test_err_syntax_empty_message()
{
    upo_store_t store = upo_store_create();
    char input[] = "\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Messaggio vuoto\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(store);
}

void test_err_data() {}

void test_err_stats() {}

void test_ok_data() {}

void test_ok_stats() {}
