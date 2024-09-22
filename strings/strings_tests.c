#ifdef BUILTIN_STRINGS
#include <string.h>
#else
#include "./strings.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "./strings_unit_tests.h"

#define USAGE                                                                \
    "./run_tests <number of times to run test, default to 1000000> <all | "  \
    "names of tests to run>"                                                 \
    "\n   Ex. \"./run_tests 10000 all\" runs all tests 10000 times each"     \
    "\n   Ex. \"./run_tests 100 strlen strchr\" runs the strlen and strchr " \
    "tests 100 times each"                                                   \
    "\n   Ex. \"./run_tests strlen\" runs the strlen test 1000000 times"

#define SUPER_LONG_STRING                                                    \
    "./run_tests <number of times to run test, default to 1000000>"          \
    "<all | names of tests to run>"                                          \
    "\n   Ex. \"./run_tests 10000 all\" runs all tests 10000 times each"     \
    "\n   Ex. \"./run_tests 100 strlen strchr\" runs the strlen and strchr " \
    "tests 100 times each"                                                   \
    "\n   Ex. \"./run_tests strtok\" runs the strtok test 1000000 times"     \
    "./run_tests <number of times to run test, default to 1000000> <all | "  \
    "names of tests to run>"                                                 \
    "\n   Ex. \"./run_tests 10000 all\" runs all tests 10000 times each"     \
    "\n   Ex. \"./run_tests 100 strlen strchr\" runs the strlen and strchr " \
    "tests 100 times each"                                                   \
    "\n   Ex. \"./run_tests strtok\" runs the strtok test 1000000 times"     \
    "./run_tests <number of times to run test, default to 1000000> <all | "  \
    "names of tests to run>"                                                 \
    "\n   Ex. \"./run_tests 10000 all\" runs all tests 10000 times each"     \
    "\n   Ex. \"./run_tests 100 strlen strchr\" runs the strlen and strchr " \
    "tests 100 times each"                                                   \
    "\n   Ex. \"./run_tests strtok\" runs the strtok test 1000000 times"

#define DEFAULT_REPEAT 1000000

int num_repeat;

// test functions
// size_t strlen(const char *s);
void test_strlen() {
    char s[350];
    snprintf(s, sizeof(s) / sizeof(char), "strlen(\"%s\")", USAGE);

    assert_equal_int(0, strlen(""), "strlen(\"\")");
    assert_equal_int(4, strlen("asdf"), "strlen(\"asdf\")");
    assert_equal_int(5, strlen("asdf "), "strlen(\"asdf \")");
    assert_equal_int(2, strlen("\n\n"), "strlen(\"\n\n\")");
    assert_equal_int(304, strlen(USAGE), s);
}

// size_t strspn(const char *s, const char *accept);
void test_strspn() {
    assert_equal_int(2, strspn("professor", "rpf"),
                     "strspn(\"professor\", \"rpf\")");
    assert_equal_int(0, strspn("professor", ""), "strspn(\"professor\", \"\")");

    char long_str_err[1000];
    snprintf(long_str_err, sizeof(long_str_err) / sizeof(char),
             "strspn(\"%s\", \"r/.\")", SUPER_LONG_STRING);
    assert_equal_int(3, strspn(SUPER_LONG_STRING, "r/."), long_str_err);
}

// size_t strcspn(const char *s, const char *reject);
void test_strcspn() {
    assert_equal_int(0, strcspn("professor", "rpf"),
                     "strcspn(\"professor\", \"rpf\")");
    assert_equal_int(9, strcspn("professor", ""),
                     "strcspn(\"professor\", \"\")");
    assert_equal_int(4, strcspn("professor", "es"),
                     "strcspn(\"professor\", \"es\")");

    char long_str_err[1000];
    snprintf(long_str_err, sizeof(long_str_err) / sizeof(char),
             "strcspn(\"%s\", \"r/.\")", SUPER_LONG_STRING);
    assert_equal_int(0, strcspn(SUPER_LONG_STRING, "r/."), long_str_err);
}

// int strncmp(const char *s1, const char *s2, size_t n);
void test_strncmp() {
    assert_same_sign(-2, strncmp("professor", "rpf", 3),
                     "strncmp(\"professor\", \"rpf\", 3)");
    assert_same_sign(17, strncmp("test", "case", 1),
                     "strncmp(\"test\", \"case\", 1)");
    assert_same_sign(0, strncmp("abc", "abc", 5),
                     "strncmp(\"abc\", \"abc\", 5)");
    assert_same_sign(0, strncmp("asdf", "asde", 3),
                     "strncmp(\"asdf\", \"asde\", 3)");
    assert_same_sign(114, strncmp("rpf", "", 3), "strncmp(\"rpf\", \"\", 3)");
    assert_same_sign(0, strncmp("rpf", "", 0), "strncmp(\"rpf\", \"\", 0)");

    // New test where some amount of the checked characters are identical, but
    // the rest are not 
    // Should fail -- this tests implementations that don't check beyond the 
    // first character
    assert_same_sign(-10, strncmp("first", "fsecond", 5),
                     "strcmp(\"first\", \"fsecond\")");

    char long_str_err[750];
    char slightly_different_long_str[350];
    snprintf(slightly_different_long_str,
             sizeof(slightly_different_long_str) / sizeof(char), "%sx", USAGE);
    snprintf(long_str_err, sizeof(long_str_err) / sizeof(char),
             "strncmp(\"%s\", \"%s\", 5)", USAGE, slightly_different_long_str);
    assert_same_sign(0, strncmp(USAGE, slightly_different_long_str, 5),
                     long_str_err);
}

// char *strstr(const char *haystack, const char *needle);
void test_strstr() {
    char *eg = "eg";
    char *dg = "dg";
    char *f = "f";
    assert_equal_str("defg", strstr("abcdefg", "de"),
                     "strstr(\"abcdefg\", \"de\")");
    assert_equal_str("babab", strstr("abbabab", "bab"),
                     "strstr(\"abbabab\", \"bab\")");
    assert_equal_str("eg", strstr("abcdeg", eg), "strstr(\"abcdeg\", \"eg\")");
    assert_equal_str(NULL, strstr("abcdeg", dg), "strstr(\"abcdeg\", \"dg\")");
    assert_equal_str(NULL, strstr("abcdeg", f), "strstr(\"abcdeg\", \"f\")");
    assert_equal_str("eg", strstr("eg", eg), "strstr(\"eg\", \"eg\")");
}

typedef struct {
    void (*test_func)();
    const char *func_name;
    int num_tests;
} strings_test_t;

strings_test_t tests[5] = {{&test_strlen, "strlen", 0},
                           {&test_strspn, "strspn", 0},
                           {&test_strcspn, "strcspn", 0},
                           {&test_strncmp, "strncmp", 0},
                           {&test_strstr, "strstr", 0}};

int functions_tested = 0;
int functions_passed = 0;

void correctness_test(strings_test_t *test) {
    reset_counts();

    test->test_func();
    printf("%s: ", test->func_name);
    print_test_summary();

    functions_tested += 1;
    functions_passed += (num_correct == total_tests);
    test->num_tests = total_tests;
}

void time_test(strings_test_t *test) {
    // don't want to print/count while testing performance
    // too many tests and the same errors over and over
    STOP_PRINTING
    STOP_COUNTING

    int pid, status;
    if (!(pid = fork())) {
        // warm up CPU+caches so timing is more consistent
        for (int i = 0; i < (num_repeat / 10); ++i)
            test->test_func();

        clock_t start = clock();
        for (int i = 0; i < num_repeat; ++i)
            test->test_func();

        int diff = 1000 * (clock() - start) / CLOCKS_PER_SEC;
        printf("%s: Performed %d tests repeated %d times in %ds %dms\n",
               test->func_name, test->num_tests, num_repeat, diff / 1000,
               diff % 1000);
        exit(0);
    }
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status))
        printf(
            "%s: Performance test failed (process was unexpectedly terminated "
            "by signal %d: %d)\n",
            test->func_name, WTERMSIG(status), status);

    RESUME_COUNTING
    RESUME_PRINTING
}

void foreach_test(int num_names, char const *test_names[],
                  void (*wrapper)(strings_test_t *)) {
    int num_tests = sizeof(tests) / sizeof(strings_test_t);
    for (int i = 0; i < num_names; ++i) {
        if (!strcmp_test(test_names[i], "all")) {
            for (int j = 0; j < num_tests; j++)
                wrapper(&tests[j]);
        } else {
            int found_test = 0;
            for (int j = 0; j < num_tests; j++)
                if (!strcmp_test(test_names[i], tests[j].func_name)) {
                    wrapper(&tests[j]);
                    found_test = 1;
                }
            if (!found_test)
                printf("Unknown test: %s\n", test_names[i]);
        }
    }
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("USAGE: %s\n", USAGE);
        return 1;
    }

    int num_args = argc - 1;
    char const **test_names = argv + 1;
    if (sscanf(argv[1], "%d", &num_repeat) != 1)
        num_repeat = DEFAULT_REPEAT;
    else {
        num_args--;
        test_names++;
    }

    printf("Testing for correctness...\n");
    foreach_test(num_args, test_names, &correctness_test);

    printf("\nTesting Performance...\n");
    foreach_test(num_args, test_names, &time_test);

    printf("\n%d/%d functions correctly implemented\n", functions_passed,
           functions_tested);
    return 0;
}
