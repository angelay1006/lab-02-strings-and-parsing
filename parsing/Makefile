CC = gcc
CFLAGS = -g3 -std=gnu99 -Wall -Wextra -Werror
TESTS = run_tests

all: $(TESTS)

$(TESTS): parsing_tests.c parsing.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TESTS)

