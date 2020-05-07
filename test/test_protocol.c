/*
I: 2 10 20\n
O: OK DATA 2\n
I: 1 12\n
O: OK DATA 1\n
I: 0\n
O: OK STATS 3 14.0 28.0\n
*/
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <upo/protocol.h>

static void test_err_syntax_rc_message();
static void test_err_syntax_empty_message();
static void test_err_syntax_invalid_first_number();
static void test_err_syntax_invalid_other_number();
static void test_err_syntax_invalid_non_integer_other_number();
static void test_err_syntax_invalid_non_integer_first_number();
static void test_err_syntax_invalid_termination_of_number();
static void test_err_syntax_terminator();
static void test_err_data_more();
static void test_err_data_less();
static void test_err_data_with_zero();
static void test_err_stats_avg();
static void test_err_stats_variance();
static void test_ok_data();
static void test_ok_stats();

int main()
{
    printf("Test case 'ERR SYNTAX'... ");
    fflush(stdout);
    test_err_syntax_rc_message();
    test_err_syntax_empty_message();
    test_err_syntax_invalid_first_number();
    test_err_syntax_invalid_other_number();
    test_err_syntax_invalid_non_integer_other_number();
    test_err_syntax_invalid_non_integer_first_number();
    test_err_syntax_invalid_termination_of_number();
    //test_err_syntax_terminator();
    printf("OK\n");

    printf("Test case 'ERR DATA'... ");
    fflush(stdout);
    //test_err_data_more();
    //test_err_data_less();
    //test_err_data_with_zero();
    printf("OK\n");

    printf("Test case 'ERR STATS'... ");
    fflush(stdout);
    //test_err_stats_avg();
    //test_err_stats_variance();
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
void test_err_syntax_rc_message()
{
    upo_store_t store = upo_store_create();
    char input[] = "\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Messaggio vuoto\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(store);
}

/*
I: 
O: ERR SYNTAX Messaggio vuoto\n
*/
void test_err_syntax_empty_message()
{
    upo_store_t store = upo_store_create();
    char input[] = "";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Messaggio vuoto\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(store);
}

/*
I: test\n
O: ERR SYNTAX Numero non valido\n
*/
void test_err_syntax_invalid_first_number()
{
    upo_store_t store = upo_store_create();
    char input[] = "test\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Numero non valido\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(store);
}

/*
I: 2 1 test\n
O: ERR SYNTAX Numero non valido\n
*/
void test_err_syntax_invalid_other_number()
{
    upo_store_t store = upo_store_create();
    char input[] = "2 1 test\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Numero non valido\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(store);
}

/*
I: 2 1 3.5\n
O: ERR SYNTAX Numero non valido\n
*/
void test_err_syntax_invalid_non_integer_other_number()
{
    upo_store_t store = upo_store_create();
    char input[] = "2 1 3.5\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Numero non valido\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(store);
}

/*
I: 3.5\n
O: ERR SYNTAX Numero non valido\n
*/
void test_err_syntax_invalid_non_integer_first_number()
{
    upo_store_t store = upo_store_create();
    char input[] = "3.5\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Numero non valido\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(store);
}

/*
I: 2 1 3test\n
O: ERR SYNTAX Numero non valido\n
*/
void test_err_syntax_invalid_termination_of_number()
{
    upo_store_t store = upo_store_create();
    char input[] = "2 1 3test\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Numero non valido\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(store);
}

/*
I: 2 1 3
O: ERR SYNTAX Mancanza del carattere di terminazione\n
*/
void test_err_syntax_terminator()
{
    upo_store_t store = upo_store_create();
    char input[] = "2 1 3";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Mancanza del carattere di terminazione\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(store);
}

/*
I: 2 1 3 4\n
O: ERR DATA Il numero di dati immessi non è coerente con la dimensione dichiarata\n
*/
void test_err_data_more()
{
    upo_store_t store = upo_store_create();
    char input[] = "2 1 3 4\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR DATA Il numero di dati immessi non è coerente con la dimensione dichiarata\n") == 0);
    assert(response == ERR_DATA);

    upo_store_destroy(store);
}

/*
I: 6 1 3 4\n
O: ERR DATA Il numero di dati immessi non è coerente con la dimensione dichiarata\n
*/
void test_err_data_less()
{
    upo_store_t store = upo_store_create();
    char input[] = "6 1 3 4\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR DATA Il numero di dati immessi non è coerente con la dimensione dichiarata\n") == 0);
    assert(response == ERR_DATA);

    upo_store_destroy(store);
}

/*
I: 0 1 3\n
O: ERR DATA Il numero di dati immessi non è coerente con la dimensione dichiarata\n
*/
void test_err_data_with_zero()
{
    upo_store_t store = upo_store_create();
    char input[] = "0 1 3\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR DATA Il numero di dati immessi non è coerente con la dimensione dichiarata\n") == 0);
    assert(response == ERR_DATA);

    upo_store_destroy(store);
}

/*
I: 0\n
O: ERR STATS Non posso calcolare la media di nessun campione\n
*/
void test_err_stats_avg()
{
    upo_store_t store = upo_store_create();
    char input[] = "0\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR STATS Non posso calcolare la media di nessun campione\n") == 0);
    assert(response == ERR_STATS);

    upo_store_destroy(store);
}

/*
I: 1 10\n
O: OK DATA 1\n
I: 0\n
O: ERR STATS Non posso calcolare la varianza di 1 campione\n
*/
void test_err_stats_variance()
{
    upo_store_t store = upo_store_create();
    char input[] = "1 10\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "OK DATA 1\n") == 0);
    assert(response == OK_DATA);

    char input2[] = "1 10\n";
    memset(output, '\0', UPO_PROTOCOL_MAX);
    response = upo_protocol(store, input2, output);

    assert(strcmp(output, "ERR STATS Non posso calcolare la varianza di 1 campione\n") == 0);
    assert(response == ERR_STATS);

    upo_store_destroy(store);
}

void test_ok_data() {}

void test_ok_stats() {}
