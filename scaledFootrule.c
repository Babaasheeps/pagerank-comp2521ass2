/**
 * @file scaledFootrule.c
 * @author Hussain Nawaz
 * @brief For UNSW COMP2521T3 ass2. Does Rank Aggregation to minimise
 * scaledFootRule
 * 
 * @date 2021-11-19
 * 
 */



#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "tokens.h"
#include "file_utility.h"
#include "scaledFootrule.h"

int main(int argc, char *argv[])
{
    // Collate all ranks to a single matrix of strings. Ignore 0-th element
    char ***ranks = filesToTokenMatrix(argc, argv);
    char **urls = fetchUniqueTokensFromTokenArray(ranks);
    int num_urls = countTokens(urls);
    sortTokens(urls, true);

    // Want to set an initially minimum array. Will be replace by true min
    double ordered_W = scaledFootRuleSum(ranks, urls);
    RankPerm min = newPermutationObject(urls, ordered_W);
    // Find best permutation
    min = findMinimumScaledFootrulePermutation(urls, num_urls, 0, ranks, min);
    
    printFinalOutput(min);

    freeTokens(urls);
    freeRankPerm(min);
    freeTokensArray(ranks);

    return 0;
}

// void generateTokenPermutation(char **tokens, int token_len, int fixed)
RankPerm findMinimumScaledFootrulePermutation(char **tokens,
                                              int token_len,
                                              int fixed,
                                              char ***rankings,
                                              RankPerm min)
{
    // Have recursed to end. Elements are swapped in place, so need
    // for an explicit backtrace. Check for scaled footrule.
    // If current tokens better than min, then replace min with current;
    if (fixed >= token_len)
    {
        double W = scaledFootRuleSum(rankings, tokens);
        
        RankPerm temp = newPermutationObject(tokens, W);
        min = permComparison(temp, min);
        return min;
    }

    for (int i = fixed; i < token_len; i++)
    {
        swapTokens(tokens, fixed, i);
        min = findMinimumScaledFootrulePermutation(tokens, token_len, fixed + 1, rankings, min);
        // Want to reset after a call s.t. the array is preserved
        swapTokens(tokens, fixed, i);
    }
    return min;
}

double scaledFootRuleSum(char ***indexes, char **P)
{
    int num_indexes = countArrayTokensRow(indexes);
    int total_urls = countTokens(P);

    double W = 0.0;
    // For all Tao pages in set of pages, sum their footRoot
    for (int i = 0; i < num_indexes; i++)
    {
        W += calculateScaledFootRule(indexes[i], P, total_urls);
    }

    return W;
}

double calculateScaledFootRule(char **tao, char **P, int total_urls)
{
    char size_tao = countTokens(tao);
    double W = 0.0;
    // For all elements in tao, sum their dist from place in P
    for (int i = 0; tao[i] != NULL; i++)
    {
        // Find position of tao[i] in P
        int pos_in_urls = findMatchingTokenIndex(P, tao[i]);

        if (pos_in_urls < 0) // Not found in urls.
            continue;

        // Calculate scaledFootRule for current tao[i] in tao
        double curr = fabs( // C division is terrible. Typecast everything
            ((double)i / (double)size_tao) -
            ((double)pos_in_urls / (double)total_urls));
        // printf("Was seeing '%s")
        W += curr;
    }

    return W;
}

RankPerm newPermutationObject(char **tokens, double weight)
{
    RankPerm new = malloc(sizeof(*new));
    new->tokens = duplicateTokens(tokens);
    new->rank = weight;
    return new;
}

RankPerm permComparison(RankPerm curr_min, RankPerm new)
{
    // Choose to keep permutation with a lower score
    if (new->rank < curr_min->rank)
        swapPerms(&curr_min, &new);
        
    return curr_min;
}

void freeRankPerm(RankPerm r)
{
    freeTokens(r->tokens);
    free(r);
}

void printPermInfo(RankPerm r)
{
    printf("The following rank has a score of: (%lf)\n", r->rank);
    printTokens(r->tokens, true);
}

void swapPerms(RankPerm *a, RankPerm *b)
{
    RankPerm temp = *a;
    *a = *b;
    *b = temp;
}

void printFinalOutput(RankPerm r)
{
    printf("%.6lf\n", r->rank);
    for (int i = 0; r->tokens[i] != NULL; i++)
        printf("%s\n", r->tokens[i]);
}