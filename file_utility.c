/**
 * @file file_utility.h
 * @author Hussain Nawaz - (z5361515)
 * @brief Implementaion file for `file_utility.c`. These are general purpose
 * utility functions for file(s) reading and, sanitisation of file data.
 * @date November 2021
 * 
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "tokens.h"
#include "file_utility.h"


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

char ***filesToTokenMatrix(int num_files, char *files[])
{
    // Create space for the token matrix
    char ***file_tokens = malloc(sizeof(char **) * (num_files + 1));

    // Let each token in the matrix hold line tokens for a file
    int i;
    for (i = 0; i < num_files; i++)
    {
        file_tokens[i] = fileToLineTokens(files[i]);
    }
    // Null terminate the tokens
    file_tokens[i] = NULL;

    return file_tokens;
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
