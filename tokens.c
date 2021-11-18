#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "tokens.h"

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