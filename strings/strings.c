/*      _        _
 *  ___| |_ _ __(_)_ __   __ _ ___
 * / __| __| '__| | '_ \ / _` / __|
 * \__ \ |_| |  | | | | | (_| \__ \
 * |___/\__|_|  |_|_| |_|\__, |___/
 *                       |___/
 */

/* XXX: Preprocessor instruction to enable basic macros; do not modify. */
#include <stddef.h>

/*
 * strlen() - abbreviated from "string length".
 *
 * - Description: determine the number of characters in a character string.
 *
 * - Arguments: a pointer to a null-terminated character string.
 *
 * - Return value: the number of characters using a special unsigned numerical
 *   type, which is intended specifically for pointer arithmetic, called size_t.
 *
 * Examples:
 * 1. >  strlen("") = 0
 * 2. >  strlen("ilovecs33") = 9
 *
 * - Hint: the length of a C string does _not_ include the null ('\0')
 *   character that terminates it.
 */
size_t strlen(const char *s) {
    size_t length = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        length++;
    }
    return length;
}

/*
 * strspn() - abbreviated from "string span".
 *
 * - Description: determine the number of consecutive characters from the start
 *   of a character string onwards that consist of characters in accept
 *
 * - Arguments: a pointer to a null-terminated character string,
 *   and a pointer to a null-terminated string of characters to accept
 *
 * - Return value: the number of consecutive characters at the beginning of s
 *   that are also characters in accept.
 *
 * Examples:
 * 1. >  strspn("abcde", "ac") = 1
 * 2. >  strspn("abcde", "ba") = 2
 * 3. >  strspn("123456", "ab") = 0
 * 4. >  strspn("hello", "hel") = 4
 */
size_t strspn(const char *s, const char *accept) {
    size_t count = 0; 
    // for each character in s
    for (int i = 0; s[i] != '\0'; i++) {
        int match_found = 0;
        // for each char in accept
        for (int j = 0; accept[j] != '\0'; j++) {
            // match found
            if (s[i] == accept[j]) {
                match_found = 1;
                break; // don't need to keep searching
            }
        }

        // if no match found, break out of loop
        if (!match_found) {
            break;
        }

        count++;
    }
    return count;
}

/*
 * strcspn() - abbreviated from "string span (complement)".
 *
 * - Description: determine the number of consecutive characters from the start
 *   of a character string onwards that consist of characters not in reject
 *
 * - Arguments: a pointer to a null-terminated character string,
 *   and a pointer to a null-terminated string of characters to reject
 *
 * - Return value: the number of consecutive characters at the beginning of s
 *   that are not characters in reject.
 *
 * Examples:
 * 1. >  strcspn("abcde", "ed") = 3
 * 2. >  strcspn("123456", "12") = 0
 * 3. >  strcspn("hello", "xyz") = 5

 */
size_t strcspn(const char *s, const char *reject) {
    // loop thru chars in s 
    size_t count = 0;

    for (int i = 0; s[i] != '\0'; i++) {
        // see if curr char is in reject
        for (int j = 0; reject[j] != '\0'; j++) {
            if (s[i] == reject[j]) {
                return count;
            }
        }
        count++;
    }
    return count;
}

/*
 * strncmp() - abbreviated from "string compare (n bytes)".
 *
 * - Description: compares up to the first n bytes of two strings, s1 and s2.
 *   strncmp() should order the functions as if they appeared in a dictionary.
 *   If s1 comes alphabetically before s2, then s1 < s2. Empty (null) characters
 *   are always less than non-empty, non-null characters.
 *
 * - Arguments: two null-terminated strings (s1 and s2) and the maximum number
 *   of bytes to compare (n).
 *
 * - Return value: negative number if s1 < s2, 0 if s1 == s2, positive number if
 *   s1 > s2
 *
 * Examples:
 * 1. >  strncmp("abcde", "abdde", 3) = some negative number
 * 2. >  strncmp("abcde", "abdde", 2) = 0
 * 3. >  strncmp("1234567", "123", 100) = some positive number
 */
int strncmp(const char *s1, const char *s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        // both strings are the same length and we haven't returned yet
        if (s1[i] == '\0' && s2[i] == '\0') {
            return 0;
        }

        if (s1[i] < s2[i]) {
            return -1;
        }
        if (s1[i] > s2[i]) {
            return 1;
        }

        // to account for strings of different length
        if (s1[i] == '\0') {
            return -1; // s1 ends first so is smaller
        }
        if (s2[i] == '\0') {
            return 1; // s2 ends first so is smaller
        }
    }
    return 0;
}

/*
 * strstr() - abbreviated from "string (find) string".
 *
 * - Description: Find the string "needle" in the string "haystack". This is
 *   not some joke from the TAs but the actual terminology used in the Linux
 *   version of strstr(). Don't believe us? Type "man strstr" into a terminal,
 *   and see for yourself! For another example of systems programming humor,
 *   you may also appreciate "man strfry".
 *
 * - Arguments: a pointer to a null-terminated character string to find
 *   (needle), and a pointer to a null-terminated character string in which to
 *   find the string (haystack)
 *
 * - Return value: a pointer to the beginning of the needle string within the
 *   haystack string
 *
 * Examples:
 * 1. >  strstr("abcde", "bcd"); should return pointer to string starting @ b
 * 2. >  strstr("123456", "4"); should return pointer to string starting @ 4
 * 3. >  strstr("hello", "xyz") = NULL
 *
 * - Hint: Use strncmp().
 */
char *strstr(const char *haystack, const char *needle) {
    // check for empty needle
    if (!needle) {
        return (char *)haystack; // since input haystack is constant
    }

    // loop through haystack
    size_t needle_length = strlen(needle);
    for (int i = 0; haystack[i] != '\0'; i++) {
        // use strncmp
        if (strncmp(&haystack[i], needle, needle_length) == 0) {
            // return a ptr to the specific address of the match
            return (char*)&haystack[i];
        }
    }

    return NULL;
}
