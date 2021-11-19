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


int main(int argc, char *argv[])
{
    // Collate all ranks to a single matrix of strings. Ignore 0-th element
    char ***ranks = filesToTokenMatrix(argc, argv);
    char **urls = fetchUniqueTokensFromTokenArray(ranks);

    printTokensArray(ranks, "RANKS", true);
    printTokens(urls, true);

    // freeTokens(urls);
    freeTokensArray(ranks);

    return 0;
}
