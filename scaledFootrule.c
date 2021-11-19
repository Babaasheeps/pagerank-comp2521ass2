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

struct rank_permutation
{
    double rank;
    char **tokens;
};
typedef struct rank_permutation *RankPerm;

double scaledFootRuleSum(char ***indexes, char **P);
double calculateScaledFootRule(char **tao, char **P, int total_urls);
RankPerm findMinimumScaledFootrulePermutation(char **tokens,
                                              int token_len,
                                              int fixed,
                                              char ***rankings,
                                              RankPerm min);

void freeRankPerm(RankPerm r);
RankPerm permComparison(RankPerm perm1, RankPerm perm2);
RankPerm newPermutationObject(char **tokens, double weight);
void printPermInfo(RankPerm r);





int main(int argc, char *argv[])
{
    // Collate all ranks to a single matrix of strings. Ignore 0-th element
    char ***ranks = filesToTokenMatrix(argc, argv);
    char **urls = fetchUniqueTokensFromTokenArray(ranks);
    int num_urls = countTokens(urls);

    printf("TOKEN LENght is %d\n", num_urls);
    sortTokens(urls, true);

    printTokensArray(ranks, "RANKS", true);
    printf("Sorted version is:\n");
    printTokens(urls, true);

    printf("GENERATE_PERMUTATION!\n");
    // Want to set an initially minimum array. Will be replace by true min
    double ordered_W = scaledFootRuleSum(ranks, urls);
    RankPerm min = newPermutationObject(urls, ordered_W);

    min = findMinimumScaledFootrulePermutation(urls, num_urls, 0, ranks, min);

    printf("---\n---\nThe FINAL min object is:\n");
    printPermInfo(min);

    printf("\nAFTER PERMS:\n");
    printTokens(urls, true);
    freeTokens(urls);
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

        printTokens(tokens, true);
        printf("Total FootRule is %lf\n", W);
        printf("FOR THE MIN:");
        
        RankPerm temp = newPermutationObject(tokens, W);
        printPermInfo(min);
        min = permComparison(min, temp);

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
    printTokensArray(indexes, "TAO ARRAY", true);
    int num_indexes = countArrayTokensRow(indexes);
    int total_urls = countTokens(P);

    double W = 0.0;
    // For all Tao pages in set of pages, sum their footRoot
    for (int i = 0; i < num_indexes; i++)
    {
        printf("SEEEKIGN FROM:\n");
        printTokens(indexes[i], true);
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

RankPerm permComparison(RankPerm perm1, RankPerm perm2)
{
    // Choose to keep permutation with a lower score
    RankPerm min = (perm1->rank < perm2->rank) ? perm1 : perm2;
    RankPerm loser = (perm1->rank > perm2->rank) ? perm1 : perm2;
    // Free the loser and return the min
    freeRankPerm(loser);

    return min;
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
    RankPerm *temp = *a;
    *a = *b;
    *b = temp;
}

// void swapDoublePointers(double **a, double **b)
// {
//     double *temp = *a;
//     *a = *b;
//     *b = temp;
// }


