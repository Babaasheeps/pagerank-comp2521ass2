// COMP2521 Assignment 2

// Written by:
// Date:

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "searchdata.h"

typedef struct url_data
{
    char *url;
    long out_degree;
    double **weighted_rank;
    long matching_searches;
} url_data;
typedef struct url_data *UrlData;

// UrlData *

int main(int argc, char *argv[])
{
    // Read inverted index - line_tokens is a set of tokens where each index
    // is a string represeting a line
    char **line_tokens = fileToLineTokens("invertedIndex.txt");
    long num_search_terms = fileNumLines("invertedIndex.txt");
    // We want to retokenize to a 2d array of strings so, for index[i][j]
    // `i` allows access to a particular line ands `j` allows for words in that
    // lines to be accesssed. [i][0] will represent search terms
    printf("Arrived to main\n");
    printTokens(line_tokens, true);
    char ***index = reTokenize(line_tokens, " ");
    printf("--\n--\n--\n\nRETOKKED AS:\n");
    for (int i = 0; i < num_search_terms; i++)
        printTokens(index[i], true);
    free(line_tokens);
}




