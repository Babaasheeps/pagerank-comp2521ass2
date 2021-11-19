/**
 * @file scaledFootrule.h
 * @author Hussain Nawaz (z5361515)
 * @brief Header file for `scaledFootrule.c`
 * 
 * @date November 2021
 * 
 */

#ifndef SCALED_FOOTRULE_H
#define SCALED_FOOTRULE_H

/**
 * @brief Used to hold a singular permutation of all the ranks;
 * 
 */
struct rank_permutation
{
    double rank;            // Sum of scaledFootrule
    char **tokens;          // String array containing all urls
};

typedef struct rank_permutation *RankPerm;


/**
 * @brief Calculates the scanedFootRule sum for a permuation of urls
 * 
 * @param indexes   // Contains all the different ranking in one place.
 * As per spec, is the set `C`, containing all `\tao_i`
 * @param P         // Current permuation of urls
 * @return double   // footrule as per spec
 */
double scaledFootRuleSum(char ***indexes, char **P);


/**
 * @brief Calculates the scaledFootRule for a singluar `\tao` as
 * per spec.
 * 
 * @param tao           - array of strings (urls)
 * @param P             - Current permutation. 
 * @param total_urls    - number of total urls - in current permutation
 * @return double 
 */
double calculateScaledFootRule(char **tao, char **P, int total_urls);

/**
 * @brief Recursively permutes the given list of urls. Does so by fixing
 * the array up to a certain index and then creating a recursive call to
 * swap and permute elemennts ahead of that `fixed` index.
 * @credit: Adapted from strings solution to LeetCode 46. See tokens file
 * for a more barebones version.
 * 
 * 
 * @param tokens        - tokens to permute
 * @param token_len     - number of tokens
 * @param fixed         - index of array to fix up to
 * @param rankings      - matrix = {S} where {S} holds all the ranks given
 * @param min           - The permutation with the minimum footrank
 * @return RankPerm 
 */
RankPerm findMinimumScaledFootrulePermutation(char **tokens,
                                              int token_len,
                                              int fixed,
                                              char ***rankings,
                                              RankPerm min);


/**
 * @brief Frees all memory associated with the given RankPerm objec
 * 
 * @param r 
 */
void freeRankPerm(RankPerm r);


/**
 * @brief Compares two permutations and swaps them such that 
 * 
 * @param perm1 
 * @param perm2 
 * @return RankPerm 
 */
RankPerm permComparison(RankPerm curr_min, RankPerm new);
RankPerm newPermutationObject(char **tokens, double weight);
void printPermInfo(RankPerm r);

void swapPerms(RankPerm *a, RankPerm *b);

void printFinalOutput(RankPerm r);

#endif // !SCALED_FOOTRULE_H