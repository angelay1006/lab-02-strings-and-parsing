#include <stddef.h>

#ifndef STRINGS_H
#define STRINGS_H

/* String function declarations. */
size_t strlen(const char *s);
size_t strspn(const char *s, const char *accept);
size_t strcspn(const char *s, const char *reject);
int strncmp(const char *s1, const char *s2, size_t n);
char *strstr(const char *haystack, const char *needle);

#endif  // STRINGS_H
