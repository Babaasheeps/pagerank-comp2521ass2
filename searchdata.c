#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

// #include "readData.h"
// #include "data.h"
#include "Graph.h"
// #include "pagerank.h"
#include "searchdata.h"


char **fileToLineTokens(char *filename)
{
    // File size metadata
    long file_len = getFileLength(filename);

    char *file_buffer = bufferFile(filename, file_len, false);
    // Tokenise file on newline. equiv to file_buffer.split('\n') in python
    char **line_tokens = tokenize(file_buffer, "\n");

    // Free the buffer
    free(file_buffer);
    return line_tokens;
}

long fileNumLines(char *filename)
{
    FILE *f = fopen(filename, "r");
    assert (f != NULL);

    // Read file char by char, searching for newlines
    long newlines = 0; 
    while (true)
    {
        int c = fgetc(f);
        if (c == EOF)
            break;
        if (c == '\n')
            newlines++;
    }
    fclose(f);
    return newlines;
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



void freeTokens (char **tokens)
{
    for (int i = 0; tokens != NULL && tokens[i] != NULL; i++)
        free (tokens[i]);
    free (tokens);
}

void freeTokensArray(char ***tokens)
{
    for (int i = 0; tokens[i] != NULL; i++)
        freeTokens(tokens[i]);
    free(tokens);
}

char **tokenize (char *str, char *sep)
{
    size_t n_tokens = 0;
    char **tokens = NULL;
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

        // Add to tokens
        tokens = realloc(tokens, ++n_tokens * sizeof *tokens);
        tokens[n_tokens - 1] = tok;
    }

    // Make the tokens null terminated and remove excess tokens.
    tokens = realloc (tokens, ++n_tokens * sizeof *tokens);
    tokens[n_tokens - 1] = NULL;

    return tokens;
}

char *bufferFile(char *filename, long file_len, bool alnum_only)
{
    char *buffer = malloc(sizeof(char) * (file_len + 1));

    // Read entire file into the buffer created
    FILE *f = fopen(filename, "r");
    int i;
    for (i = 0; i < file_len; i++)
    {
        // Read char by char. Remove any non-alphanumeric chars if specified
        // Replace as whitespace. 
        int c = fgetc(f);
        buffer[i] = (alnum_only && !isalnum(c)) ? ' ' : c;
    }
    buffer[i] = '\0';
    fclose(f);
    return buffer;
}

char ***reTokenize(char **tokens, char *sep)
{
    long lines = (long)countTokens(tokens);
    char ***new_tok = malloc(sizeof(char **) * (lines + 1));

    int i;
    // Retokenize the each line
    for(i = 0; i < lines; i++)
        new_tok[i] = tokenize(tokens[i], sep);
        
    new_tok[i] = NULL;
    return new_tok;
}

void printTokens(char **tokens, bool is_comma_seperated)
{
    printf("[ ");
    if (!is_comma_seperated)
        printf("\n");

    for (int i = 0; tokens[i] != NULL; i++)
    {
        if (is_comma_seperated)
            printf("'%s' , ", tokens[i]);
        else
            printf("\t'%s'\n", tokens[i]);
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


