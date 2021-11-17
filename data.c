#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"


// int main(void)
// {
//     getLinkCollection("collection.txt");
//     return 0;
// }

char **getLinkCollection(char *filename)
{
    // Get length of file to know how much space to allocate in buffer
    long file_length = getFileLength(filename);
    char *buffer = bufferFile(filename, file_length);
    char **urls = tokenize (buffer);
    free(buffer);
    return urls;
}

char *bufferFile(char *filename, long file_len)
{
    char *buffer = malloc(sizeof(char) * (file_len + 1));

    // Read entire file into the buffer created
    FILE *f = fopen(filename, "r");
    int i;
    for (i = 0; i < file_len; i++)
    {
        // Read char by char. Remove any non-alphanumeric chars.
        // Replace as whitespace
        int c = fgetc(f);
        buffer[i] = (isalnum(c)) ? c : ' ';
    }
    buffer[i] = '\0';
    fclose(f);
    return buffer;
}

char **getOutgoingLinks(char *file)
{
    // Add ".txt" to file name. Create space for extension and null 
    char *filename = createString(MAX_URL_LEN + 6);
    strcpy(filename, file);
    char *file_ext = ".txt";
    strcat(filename, file_ext);

    // Open the file and, tokenise all outgoing urls
    char **outgoing_urls = doReadOutgoingLinks(filename);
    free(filename);
    return outgoing_urls;
}

char **doReadOutgoingLinks(char *filename)
{
    // Create space to be able to buffer the file 
    long file_length = getFileLength(filename);
    char *file_buffer = createString(file_length + 1);

    FILE *f = fopen(filename, "r");
    assert(f != NULL);
    
    // Can ignore opening line.
    char *opening_line = createString(file_length + 1);
    opening_line = fgets(opening_line, file_length, f);
    free(opening_line);

    // Read input till the "#" to signify end of section 1 is reached.
    for (int i = 0; true; i++)
    {
        int c = fgetc(f);
        if (c == EOF || c == '#')
        {
            file_buffer[i] = '\0';
            break;
        }
        file_buffer[i] = (isalnum(c)) ? c : ' ';
    }
    // Turn the buffer holding section 1 into tokens w/ url names
    char **urls = tokenize(file_buffer);
    // Free buffer and close file 
    fclose(f);
    free(file_buffer);
    return urls;
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
