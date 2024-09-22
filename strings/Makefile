CC = gcc
CFLAGS = -g3 -std=gnu99 -Wall -Wextra -Werror -fno-builtin
LDFLAGS = -lm
TESTS = run_tests

SRCS = strings_tests.c strings_unit_tests.c strings.c
HEADERS = strings_unit_tests.h strings.h

all: $(TESTS)

$(TESTS): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

clean:
	rm -f $(TESTS)

