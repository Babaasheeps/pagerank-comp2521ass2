#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readData.h"


char **getLinkCollection(char *filename)
{
    // Get length of the file
    long file_length = getFileLength(filename);
    // Convert filename to a string so that it can be tokenized
    char *file_str = fileToString(filename, file_length);
    char **urls = tokenize(file_str);
    free(file_str);
    printTokens(urls, true);
    return urls;
}

char *fileToString(char *filename, long file_len)
{
    // Allocate memory to store entire file in string
    char *s = malloc(sizeof(*s) * (file_len + 1));
    assert (s != NULL);

    FILE *f = fopen(filename, "r");
    int i;
    for (i = 0; i < file_len; i++)
    {
        // Read char and turn all not alphanumeric to whitespace
        int c = fgetc(f);
        printf("%d\t%c\n", i, c);
        s[i] = (isalnum(c)) ? c : ' ';
    }
    printf("%s\n", s);
    fclose(f);
    return s;
}


char **outgoingLinks(char *file)
{
    // Create filename that is <file>.txt
    char *filename = malloc(sizeof(*filename) * (MAX_URL_LEN + 1));
    assert (filename != NULL);
    // filename[0] = '\0';
    strcpy(filename, file);
    char *file_ext = ".txt";
    strcat(filename, file_ext);
    printf("Scanning:\t %s\n", filename);

    // Open File and Get all the token from it
    char **outgoing_urls = doReadOutgoingLinks(filename);
    return outgoing_urls;
}

char **doReadOutgoingLinks(char *filename)
{
    long file_len = getFileLength(filename);
    char *urlcontents = malloc(sizeof(*urlcontents) * (file_len + 1));
    assert (urlcontents != NULL);
    FILE *f = fopen(filename, "r");
    assert (f != NULL);
    // char *closing_line = "#end Section-1";
    char *opening_line = "#start Section-1";

    // Skip the first line, which is an opening Label
    char *temp = malloc(sizeof(*temp) * (strlen(opening_line) + 1));
    fgets(temp, file_len, f);

    // Scan input till closing line reached
    while (true)
    {
        temp = fgets(temp, file_len, f);
        if (temp[0] == '#')
            break;
        strcpy(urlcontents, temp);
    }
    char **outgoing_urls = tokenize(urlcontents);
    free(urlcontents);
    free(temp);
    return outgoing_urls;
}

long getFileLength(char *filename)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
        exit(EXIT_FAILURE);
    fseek(f, 0, SEEK_END);              // seek to end of file
    long file_len = ftell(f);           // len = offset(end) - start
    fclose(f);
    return file_len;
}


char **tokenize (char *str)
{
    size_t n_tokens = 0;
    char **tokens = NULL;
    char *sep = " ";
    while (*str != '\0')
    {
        // We are pointing at zero or more of any of the separators.
        // Skip all leading instances of the separators.
        str += strspn(str, sep);

        // Trailing separators after the last token mean that, at this
        // point, we are looking at the end of the string, so:
        if (*str == '\0')
            break;

        // Now, `s' points at one or more characters we want to keep.
        // The number of non-separator characters is the token length.
        // Allocate a copy of the token, and preserve it.
        size_t len = strcspn(str, sep);
        char *tok = strndup(str, len);
        assert(tok != NULL);
        str += len;

        // Add this token.  (Should really use reallocarray(3) here.)
        tokens = realloc(tokens, ++n_tokens * sizeof *tokens);
        tokens[n_tokens - 1] = tok;
    }

    // Add the final `NULL'.
    tokens = realloc (tokens, ++n_tokens * sizeof *tokens);
    tokens[n_tokens - 1] = NULL;

    return tokens;
}

void freeTokens (char **tokens)
{
        for (int i = 0; tokens != NULL && tokens[i] != NULL; i++)
                free (tokens[i]);

        free (tokens);
}



void printTokens(char **tokens, bool is_comma_seperated)
{
    printf("[ ");
    if (!is_comma_seperated)
        printf("\n");

    for (int i = 0; tokens[i] != NULL; i++)
    {
        if (is_comma_seperated)
        {
            printf("%s , ", tokens[i]);
        }
        else
        {
            printf("\t%s\n", tokens[i]);
        }
    }
    if (is_comma_seperated)
        printf(" NULL ]\n");
    else
        printf("\t NULL\n]\n");
}

size_t countTokens(char **tokens)
{
    if (tokens == NULL)
        return 0;
    size_t i;
    for (i = 0; tokens[i] != NULL; i++)
        ;
    return i;
}
