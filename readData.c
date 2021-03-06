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
    return urls;
}

char *fileToString(char *filename, long file_len)
{
    // Allocate memory to store entire file in string
    char *s = createString(file_len + 2);

    FILE *f = fopen(filename, "r");
    int i;
    for (i = 0; i < file_len; i++)
    {
        // Read char and turn all not alphanumeric to whitespace
        int c = fgetc(f);
        s[i] = (isalnum(c)) ? c : ' ';
    }
    fclose(f);
    return s;
}


char **outgoingLinks(char *file)
{
    // Create filename that is <file>.txt
    char *filename = createString(MAX_URL_LEN + 2);
    strcpy(filename, file);
    char *file_ext = ".txt";
    strcat(filename, file_ext);

    // Open File and Get all the token from it
    char **outgoing_urls = doReadOutgoingLinks(filename);
    return outgoing_urls;
}

char **doReadOutgoingLinks(char *filename)
{
    long file_len = getFileLength(filename);
    char *urlcontents = createString(file_len + 2);
    // char *urlcontents = malloc(sizeof(*urlcontents) * (file_len + 1));
    // urlcontents[0] = '\0';
    FILE *f = fopen(filename, "r");
    assert (f != NULL);
    // char *closing_line = "#end Section-1";
    char *opening_line = "#start Section-1";

    // Skip the first line, which is an opening Label
    char *temp = createString(strlen(opening_line) + 1);
    // char *temp = malloc(sizeof(*temp) * (strlen(opening_line) + 1));
    fgets(temp, file_len, f); // First line is "# Section 1" - can skip
    free(temp);

    // Scan input till closing line reached
    // printf("FILE: %s\n", filename);
    temp = createString(file_len + 1);
    while (true)
    {
        temp = fgets(temp, file_len, f);
        // '#' indicated end of section-1.
        if (temp[0] == '#')
            break;
        strcat(urlcontents, temp);
        // free(temp);
    }
    char **outgoing_urls = tokenize(urlcontents);
    free(urlcontents);
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
    char *sep = " \n";
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
        size_t len = strcspn(str, sep);
        // Allocate a copy of the token, and preserve it.
        char *tok = strndup(str, len);
        assert(tok != NULL);
        str += len;

        // Add this token.  (Should really use reallocarray(3) here.)
        tokens = realloc(tokens, ++n_tokens * sizeof *tokens);
        tokens[n_tokens - 1] = tok;
    }

    // Make the tokens null terminated and remove excess tokens.
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
            printf("%s , ", tokens[i]);
        else
            printf("\t%s\n", tokens[i]);
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

char *createString(int n)
{
    char *s = malloc(sizeof(char) * n);
    assert(s != NULL);
    for (int i = 0; i < n; i++)
        s[0] = '\0';
    return s;
}
