/**
 * @file scaledFootrule.h
 * @author Hussain Nawaz (z5361515)
 * @brief Header file for `scaledFootrule.c`
 * 
 * @date November 2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef SCALED_FOOTRULE_H
#define SCALED_FOOTRULE_H


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

void swapPerms(RankPerm *a, RankPerm *b);

void printFinalOutput(RankPerm r);

#endif // !SCALED_FOOTRULE_H