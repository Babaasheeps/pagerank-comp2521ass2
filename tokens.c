/**
 * @file tokens.h
 * @author Hussain Nawaz (z5361515)
 * @brief Implementaion for all `token` functions. A singular `token` is just
 * a string. `Tokens` refers to a NULL-terminated array of strings.
 * A `tokensArray` is a NULL terminated array of `tokens` objects.
 * 
 * @date November 2021
 * 
 */

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

void printTokensArray(char ***tokens, char *label, bool comma_seperate)
{
    if (tokens == NULL)
        return;

    if (label != NULL)
        printf("Printing Token Array: '%s':\n", label); 
    
    for (int i = 0; tokens[i] != NULL; i++)
        printTokens(tokens[i], comma_seperate);
    printf("[ NULL ] \n");
    if (label != NULL)
        printf("Finished Token Array '%s'.\n", label);
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


char **fetchUniqueTokensFromTokenArray(char ***tokens)
{
    // Count the total number of tokens in all arrays
    size_t max_tokens = 0;
    for (int i = 0; tokens[i] != NULL; i++)
        max_tokens += countTokens(tokens[i]);
    // Make enough space for worst case where every token is unique
    char **unique = malloc(sizeof(char *) * (max_tokens + 1));

    // For safety: Ensure always NULL-termed, even if extra terms added
    for (int i = 0; i < max_tokens; i++)
        unique[i] = NULL;

    size_t num_unique = 0;
    // For every token in array, check if its unique. If so, add to set of unique
    for (int i = 1; tokens[i] != NULL; i++)
    {
        for (int j = 0; tokens[i][j] != NULL; j++)
        {
            int token_index = findMatchingTokenIndex(unique, tokens[i][j]);
            if (token_index != -1)      // Match found. Not unique
                continue;
            // Add token to set of unique tokens. Create by value, not ref
            unique[num_unique] = copyString(tokens[i][j]);
            num_unique++;
            unique[num_unique] = NULL; // Keep NULL-terminated
        }
    }
    unique[num_unique] = NULL;
    return unique;
}

int findMatchingTokenIndex(char **tokens, char *target)
{
    if (target == NULL || tokens == NULL)
        return -1;

    for (int i = 0; tokens[i] != NULL; i++)
    {
        if (!strcmp(tokens[i], target))
            return i;
    }
    return -1;
}


char *copyString(char *str)
{
    if(str == NULL)
        return NULL;
    int new_len = strlen(str) + 1;
    char *new = malloc(sizeof(char) * (new_len));
    assert(new != NULL);
    strncpy(new, str, new_len);

    return new;
}

void sortTokens(char **tokens, bool ascending)
{
    int n = (int)countTokens(tokens);
    bool is_sorted = false;
    while (!is_sorted)
    {
        is_sorted = true;
        for (int i = 0; i < n - 1; i++)
        {
            if ((strcmp(tokens[i], tokens[i + 1]) < 0) != ascending)
            {
                swapTokens(tokens, i, i + 1);
                is_sorted = false;
            }
        }
    }
}

void swapTokens(char **tokens, int a, int b)
{
    char *temp = tokens[a];
    tokens[a] = tokens[b];
    tokens[b] = temp;
}


void generateTokenPermutation(char **tokens, int token_len, int fixed)
{
    // Have recursed to end. Elements are swapped in place, so need 
    // for an explicit backtrace. Can perform whatever action you want
    if (fixed >= token_len)
    {
        printTokens(tokens, true);
        return;
    }

    for (int i = fixed; i < token_len; i++)
    {
        swapTokens(tokens, fixed, i);
        generateTokenPermutation(tokens, token_len, fixed + 1);
        // Want to reset after a call s.t. the array is preserved
        swapTokens(tokens, fixed, i);
    }
}

size_t countArrayTokensRow(char ***tokens)
{
    if (tokens == NULL)
        return 0;

    size_t i;
    for (i = 0; tokens[i] != NULL; i++)
        ;
    return i;
}


char **duplicateTokens(char **tokens)
{
    int num_toks = countTokens(tokens);
    char **new = malloc(sizeof(char *) * (num_toks + 1));
    
    for (int i = 0; tokens[i] != NULL; i++)
    {
        new[i] = copyString(tokens[i]);
    }
    new[num_toks] = NULL;
    return new;
}
