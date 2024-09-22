#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "parsing.h"

// ANSI UI Stuff:
#define DEFAULT "\033[0;0m"
#define BOLD(str) "\033[0;1m" str DEFAULT
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"

#define CHECK 0x2713
#define EX 0x2715

#define PRINT_HR()                                                             \
    puts(                                                                      \
        "====================================================================" \
        "============")
#define PRINT_HD()                                                             \
    puts(                                                                      \
        "--------------------------------------------------------------------" \
        "------------")

#define BUFFER_LEN 1024
#define MAX_TOKENS 512

typedef struct token_test {
    char *test_name;
    char *input;
    char *expected_tokens[2][MAX_TOKENS];
    char *expected_execv[2][MAX_TOKENS];
    int has_quotes;
} token_test_t;

void print_tokens(char *prefix, char **tokens, int max) {
    int i = 0;
    printf("%s [", prefix);
    while (tokens[i] && i < max) {
        if (i == 0)
            printf("\"%.*s\"", BUFFER_LEN, tokens[i]);
        else
            printf(", \"%.*s\"", BUFFER_LEN, tokens[i]);
        i++;
    }

    if (i == max) {
        printf("]\nMax tokens exceeded! Did you NULL terminate?\n");
    } else {
        printf("%sNULL]\n", i == 0 ? "" : ", ");
    }
}

int check_tokens(char **a, char **b) {
    char **cura = a;
    char **curb = b;

    while (1) {
        if (!*cura && !*curb) {
            // Both NULL: True
            return 1;
        } else if (!*cura || !*curb) {
            // Otherwise, one NULL but not the other: False
            return 0;
        } else if (strcmp(*cura, *curb) != 0) {
            // String mismatch False
            return 0;
        }

        cura++;
        curb++;
    }
}

int run_test(token_test_t test, int idx) {
    printf(BOLD("TEST %d: %s\n"), idx, test.test_name);
    PRINT_HR();
    printf(BOLD("INPUT: \"%s\"\n"), test.input);

    char buffer[BUFFER_LEN];
    memset(buffer, 0, BUFFER_LEN);
    char *tokens[MAX_TOKENS];
    memset(tokens, 0, MAX_TOKENS * sizeof(char *));
    char *execv[MAX_TOKENS];
    memset(execv, 0, MAX_TOKENS * sizeof(char *));
    strncpy(buffer, test.input, MAX_TOKENS);

    // Run student parsing command
    parse(buffer, tokens, execv);

    printf(BOLD("\nExecv:\n"));
    PRINT_HD();

    print_tokens("Expected:", test.expected_execv[0], MAX_TOKENS);
    if (test.has_quotes)
        print_tokens("      OR:", test.expected_execv[1], MAX_TOKENS);
    print_tokens("Received:", execv, MAX_TOKENS);

    printf(BOLD("\nTokens:\n"));
    PRINT_HD();

    print_tokens("Expected:", test.expected_tokens[0], MAX_TOKENS);
    if (test.has_quotes)
        print_tokens("      OR:", test.expected_tokens[1], MAX_TOKENS);
    print_tokens("Received:", tokens, MAX_TOKENS);

    int result =
        (check_tokens(test.expected_execv[0], execv) &&
         check_tokens(test.expected_tokens[0], tokens)) ||
        (test.has_quotes && check_tokens(test.expected_execv[1], execv) &&
         check_tokens(test.expected_tokens[1], tokens));

    if (result) {
        printf(BOLD("\nResult: " GREEN "PASSED %lc\n\n\n"), CHECK);
    } else {
        printf(BOLD("\nResult: " RED "FAILED %lc\n\n\n"), EX);
    }
    return result;
}

#define NUM_TESTS 12
int main() {
    setlocale(LC_ALL, "");

    token_test_t tests[12] = {
        {.test_name = "Empty string",
         .input = "",
         .expected_tokens = {{NULL}},
         .expected_execv = {{NULL}},
         .has_quotes = 0},
        {.test_name = "Only whitespace",
         .input = " ",
         .expected_tokens = {{NULL}},
         .expected_execv = {{NULL}},
         .has_quotes = 0},
        {.test_name = "Tab",
         .input = "\techo\t",
         .expected_tokens = {{"echo", NULL}},
         .expected_execv = {{"echo", NULL}},
         .has_quotes = 0},
        {.test_name = "Whitespace before",
         .input = " echo",
         .expected_tokens = {{"echo", NULL}},
         .expected_execv = {{"echo", NULL}},
         .has_quotes = 0},
        {.test_name = "Tab and whitespace",
         .input = "\t echo ",
         .expected_tokens = {{"echo", NULL}},
         .expected_execv = {{"echo", NULL}},
         .has_quotes = 0},
        {.test_name = "Single argument",
         .input = "cd directory",
         .expected_tokens = {{"cd", "directory", NULL}},
         .expected_execv = {{"cd", "directory", NULL}},
         .has_quotes = 0},
        {.test_name = "Multiple arguments",
         .input = "echo trader toms snacks",
         .expected_tokens = {{"echo", "trader", "toms", "snacks", NULL}},
         .expected_execv = {{"echo", "trader", "toms", "snacks", NULL}},
         .has_quotes = 0},
        {.test_name = "Binary name handling for execv",
         .input = "/bin/ls",
         .expected_tokens = {{"/bin/ls", NULL}},
         .expected_execv = {{"ls", NULL}},
         .has_quotes = 0},
        {.test_name = "Binary name for execv with arg",
         .input = "/bin/cd directory",
         .expected_tokens = {{"/bin/cd", "directory", NULL}},
         .expected_execv = {{"cd", "directory", NULL}},
         .has_quotes = 0},
        {.test_name = "Handle long binary path for execv",
         .input = "/course/cs0330/test",
         .expected_tokens = {{"/course/cs0330/test", NULL}},
         .expected_execv = {{"test", NULL}},
         .has_quotes = 0},
        {.test_name = "Handle invalid binary path for execv",
         .input = "/bin/ls/",
         .expected_tokens = {{"/bin/ls/", NULL}},
         .expected_execv = {{"", NULL}},
         .has_quotes = 0},
        {.test_name = "Using quotes in args",
         .input = "/bin/echo 'Hello World!'",
         .expected_tokens = {{"/bin/echo", "'Hello", "World!'", NULL},
                             {"/bin/echo", "Hello World!", NULL}},
         .expected_execv = {{"echo", "'Hello", "World!'", NULL},
                            {"echo", "Hello World!", NULL}},
         .has_quotes = 1}};

    int passed = 0;
    for (int i = 0; i < NUM_TESTS; i++) {
        if (run_test(tests[i], i + 1)) {
            passed++;
        }
    }

    printf("%d/%d tests passed.\n", passed, NUM_TESTS);

    return 0;
}