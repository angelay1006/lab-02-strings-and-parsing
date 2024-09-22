/* XXX: Preprocessor instruction to enable basic macros; do not modify. */
#include <stddef.h>
#include <string.h>

/*
 * parse()
 *
 * - Description: creates the token and argv arrays from the buffer character
 *   array
 *
 * - Arguments: buffer: a char array representing user input, tokens: the
 *   tokenized input, argv: the argument array eventually used for execv()
 *
 * - Usage:
 *
 *      For the tokens array:
 *
 *      cd dir -> [cd, dir]
 *      [tab]mkdir[tab][space]name -> [mkdir, name]
 *      /bin/echo 'Hello world!' -> [/bin/echo, 'Hello, world!']
 *
 *      For the argv array:
 *
 *       char *argv[4];
 *       argv[0] = echo;
 *       argv[1] = 'Hello;
 *       argv[2] = world!';
 *       argv[3] = NULL;
 *
 * - Hint: for this part of the assignment, you are allowed to use the built
 *   in string functions listed in the handout
 */
void parse(char buffer[1024], char *tokens[512], char *argv[512]) {
    // use strtok to split buffer into tokens, put them in tokens
    char *token = strtok(buffer, " \t\n"); // first call
    int i = 0;
    while (token != NULL) { // stop when no more tokens are available
        tokens[i] = token;
        i++;
        token = strtok(NULL, " \t\n"); // returns next token from where it previously left off
    }
    tokens[i] = NULL; // to show where tokens array ends

    // process command 
    // argv[0] - depends on whether we have full path or not
    if (tokens[0] != NULL) {
        // tokens[0] is a full path (like /bin/echo)
        char *rightmost_slash = strrchr(tokens[0], '/');
        if (rightmost_slash == NULL) {
            // no slashes, whole token is command
            argv[0] = tokens[0];
        } else {
            // the command name comes after the slash
            argv[0] = rightmost_slash + 1;
        }
    }

    // process arguments
    int j = 1;
    while (tokens[j] != NULL) {
        argv[j] = tokens[j];
        j++;
    }

    argv[j] = NULL;
}
