#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <upo/server/protocol.h>

static void test_err_syntax_rc_message();
static void test_err_syntax_empty_message();
static void test_err_syntax_invalid_size_number();
static void test_err_syntax_invalid_data_number();
static void test_err_syntax_invalid_non_integer_data_number();
static void test_err_syntax_invalid_non_integer_size_number();
static void test_err_syntax_invalid_termination_of_number();
static void test_err_syntax_invalid_data_negative_number();
static void test_err_syntax_invalid_size_negative_number();
static void test_err_syntax_terminator();
static void test_err_syntax_char_after_terminator();
static void test_err_data_more();
static void test_err_data_less();
static void test_err_data_with_zero_size();
static void test_err_stats_avg();
static void test_err_stats_variance();
static void test_ok_data();
static void test_ok_stats();
static void test_ok();
static void test_ok_with_full_buffer();

int main()
{
    printf("Test case 'ERR SYNTAX'... ");
    fflush(stdout);
    test_err_syntax_rc_message();
    test_err_syntax_empty_message();
    test_err_syntax_invalid_size_number();
    test_err_syntax_invalid_data_number();
    test_err_syntax_invalid_non_integer_data_number();
    test_err_syntax_invalid_non_integer_size_number();
    test_err_syntax_invalid_termination_of_number();
    test_err_syntax_invalid_data_negative_number();
    test_err_syntax_invalid_size_negative_number();
    test_err_syntax_terminator();
    test_err_syntax_char_after_terminator();
    printf("OK\n");

    printf("Test case 'ERR DATA'... ");
    fflush(stdout);
    test_err_data_more();
    test_err_data_less();
    test_err_data_with_zero_size();
    printf("OK\n");

    printf("Test case 'ERR STATS'... ");
    fflush(stdout);
    test_err_stats_avg();
    test_err_stats_variance();
    printf("OK\n");

    printf("Test case 'OK DATA'... ");
    fflush(stdout);
    test_ok_data();
    printf("OK\n");

    printf("Test case 'OK STATS'... ");
    fflush(stdout);
    test_ok_stats();
    printf("OK\n");

    printf("Test case 'OK'... ");
    fflush(stdout);
    test_ok();
    test_ok_with_full_buffer();
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

    upo_store_destroy(&store);
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

    upo_store_destroy(&store);
}

/*
I: test\n
O: ERR SYNTAX Numero non valido\n
*/
void test_err_syntax_invalid_size_number()
{
    upo_store_t store = upo_store_create();
    char input[] = "test\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Numero non valido\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(&store);
}

/*
I: 2 1 test\n
O: ERR SYNTAX Numero non valido\n
*/
void test_err_syntax_invalid_data_number()
{
    upo_store_t store = upo_store_create();
    char input[] = "2 1 test\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Numero non valido\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(&store);
}

/*
I: 2 1 3.5\n
O: ERR SYNTAX Numero non valido\n
*/
void test_err_syntax_invalid_non_integer_data_number()
{
    upo_store_t store = upo_store_create();
    char input[] = "2 1 3.5\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Numero non valido\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(&store);
}

/*
I: 3.5\n
O: ERR SYNTAX Numero non valido\n
*/
void test_err_syntax_invalid_non_integer_size_number()
{
    upo_store_t store = upo_store_create();
    char input[] = "3.5\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Numero non valido\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(&store);
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

    upo_store_destroy(&store);
}

/*
I: 2 -1 3\n
O: ERR SYNTAX Numero non valido\n
*/
void test_err_syntax_invalid_data_negative_number()
{
    upo_store_t store = upo_store_create();
    char input[] = "2 -1 3\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Numero non valido\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(&store);
}

/*
I: -1\n
O: ERR SYNTAX Numero non valido\n
*/
void test_err_syntax_invalid_size_negative_number()
{
    upo_store_t store = upo_store_create();
    char input[] = "-1\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Numero non valido\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(&store);
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

    upo_store_destroy(&store);
}

/*
I: 3 1 3\n 4
O: ERR SYNTAX Messaggio non valido\n
*/
void test_err_syntax_char_after_terminator()
{
    upo_store_t store = upo_store_create();
    char input[] = "3 1 3\n 4";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR SYNTAX Messaggio non valido\n") == 0);
    assert(response == ERR_SYNTAX);

    upo_store_destroy(&store);
}

/*
I: 2 1 3 4\n
O: ERR DATA Il numero di dati immessi è maggiore della dimensione dichiarata\n
*/
void test_err_data_more()
{
    upo_store_t store = upo_store_create();
    char input[] = "2 1 3 4\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR DATA Il numero di dati immessi è maggiore della dimensione dichiarata\n") == 0);
    assert(response == ERR_DATA);

    upo_store_destroy(&store);
}

/*
I: 4 1 3 4\n
O: ERR DATA Il numero di dati immessi è minore della dimensione dichiarata\n
*/
void test_err_data_less()
{
    upo_store_t store = upo_store_create();
    char input[] = "4 1 3 4\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR DATA Il numero di dati immessi è minore della dimensione dichiarata\n") == 0);
    assert(response == ERR_DATA);

    upo_store_destroy(&store);

    store = upo_store_create();
    char input2[] = "1\n";

    response = upo_protocol(store, input2, output);

    assert(strcmp(output, "ERR DATA Il numero di dati immessi è minore della dimensione dichiarata\n") == 0);
    assert(response == ERR_DATA);

    upo_store_destroy(&store);
}

/*
I: 0 1 3\n
O: ERR DATA Il numero di dati immessi è maggiore della dimensione dichiarata\n
*/
void test_err_data_with_zero_size()
{
    upo_store_t store = upo_store_create();
    char input[] = "0 1 3\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR DATA Il numero di dati immessi è maggiore della dimensione dichiarata\n") == 0);
    assert(response == ERR_DATA);

    upo_store_destroy(&store);
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

    upo_store_destroy(&store);
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
    upo_store_insert(store, 10);
    char input[] = "0\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "ERR STATS Non posso calcolare la varianza di 1 campione\n") == 0);
    assert(response == ERR_STATS);

    upo_store_destroy(&store);
}

/*
I: 10 1 2 3 4 5 6 7 8 9 10\n
O: OK DATA 10\n
I: 3 1 2 3\n
O: OK DATA 3\n
*/
void test_ok_data()
{
    upo_store_t store = upo_store_create();
    char input[] = "10 1 2 3 4 5 6 7 8 9 10\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "OK DATA 10\n") == 0);
    assert(response == OK_DATA);

    char input2[] = "3 1 2 3\n";
    memset(output, '\0', UPO_PROTOCOL_MAX);
    response = upo_protocol(store, input2, output);

    assert(strcmp(output, "OK DATA 3\n") == 0);
    assert(response == OK_DATA);

    upo_store_destroy(&store);
}

/*
I: 2 10 20\n
O: OK DATA 2\n
I: 1 12\n
O: OK DATA 1\n
I: 0\n
O: OK STATS 3 14.00 18.67\n
*/
void test_ok_stats()
{
    upo_store_t store = upo_store_create();
    upo_store_insert(store, 10);
    upo_store_insert(store, 20);
    upo_store_insert(store, 12);
    char input[] = "0\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "OK STATS 3 14.00 28.00\n") == 0);
    assert(response == OK_STATS);

    upo_store_destroy(&store);
}

/*
I: 2 10 20\n
O: OK DATA 2\n
I: 1 12\n
O: OK DATA 1\n
I: 0\n
O: OK STATS 3 14.00 18.67\n
*/
void test_ok()
{
    upo_store_t store = upo_store_create();
    char input[] = "10 1 2 3 4 5 6 7 8 9 10\n";
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "OK DATA 10\n") == 0);
    assert(response == OK_DATA);

    char input2[] = "3 1 2 3\n";
    memset(output, '\0', UPO_PROTOCOL_MAX);
    response = upo_protocol(store, input2, output);

    assert(strcmp(output, "OK DATA 3\n") == 0);
    assert(response == OK_DATA);

    char input3[] = "0\n";
    memset(output, '\0', UPO_PROTOCOL_MAX);
    response = upo_protocol(store, input3, output);

    assert(strcmp(output, "OK STATS 13 4.69 9.40\n") == 0);
    assert(response == OK_STATS);

    upo_store_destroy(&store);
}

void test_ok_with_full_buffer()
{
    upo_store_t store = upo_store_create();
    char input[UPO_PROTOCOL_MAX + 1] = {0};
    strcpy(input, "154 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154\n");
    char output[UPO_PROTOCOL_MAX] = {0};

    upo_protocol_response_t response = upo_protocol(store, input, output);

    assert(strcmp(output, "OK DATA 154\n") == 0);
    assert(response == OK_DATA);

    char input2[] = "0\n";
    memset(output, '\0', UPO_PROTOCOL_MAX);
    response = upo_protocol(store, input2, output);

    assert(strcmp(output, "OK STATS 154 77.50 1989.17\n") == 0);
    assert(response == OK_STATS);

    upo_store_destroy(&store);
}
