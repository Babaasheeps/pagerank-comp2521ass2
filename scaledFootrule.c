// COMP2521 Assignment 2

// Written by: Hussain Nawaz (z5361515)
// Date: November 2021

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"
#include "file_utility.h"

void generatePermutation(char **tokens, int token_len, int fixed);

int main(int argc, char *argv[])
{
    // Collate all ranks to a single matrix of strings. Ignore 0-th element
    char ***ranks = filesToTokenMatrix(argc, argv);
    char **urls = fetchUniqueTokensFromTokenArray(ranks);
    int num_urls = countTokens(urls);
    sortTokens(urls, true);

    printTokensArray(ranks, "RANKS", true);
    printf("Sorted version is:\n");
    printTokens(urls, true);

    printf("GENERATE_PERMUTATION!\n");
    generatePermutation(urls, num_urls, 0);

    freeTokens(urls);
    freeTokensArray(ranks);


    return 0;
}


// Fix the First

void generatePermutation(char **tokens, int token_len, int fixed)
{
    if (fixed >= token_len + 1)
    {
        printTokens(tokens, true);
        return;
    }

    // printTokens(tokens);
    for (int i = fixed + 1; i < token_len; i++)
    {
        swapTokens(tokens, i, fixed);
        generatePermutation(tokens, token_len, fixed + 1);
        swapTokens(tokens, i, fixed + 1);
    }
}



