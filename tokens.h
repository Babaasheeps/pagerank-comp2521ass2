/**
 * @file tokens.h
 * @author Hussain Nawaz (z5361515)
 * @brief Interface for all `token` functions. A singular `token` is just
 * a string. `Tokens` refers to a NULL-terminated array of strings.
 * A `tokensArray` is a NULL terminated array of `tokens` objects.
 * 
 * @date November 2021
 * 
 */

#ifndef TOKENS_H
#define TOKENS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/**
 * @brief Free all memory associated w/ the given set of NULL-terminated tokens
 * 
 * @param tokens - tokens to free
 */
void freeTokens(char **tokens);


/**
 * @brief Free all memory associated w/ the given array of tokens
 * Uses `freeTokens` to free individual elements
 * 
 * @param tokens - token array to free
 */
void freeTokensArray(char ***tokens);


/**
 * @brief Print the given set of tokens (similar formatting to a python list)
 * 
 * @param tokens - token to print
 * @param is_comma_seperated - elements seperated by comma if true. Else newline
 */
void printTokens(char **tokens, bool is_comma_seperated);


/**
 * @brief Given a string, tokenise it, delimited by the characters in `sep`.
 * Most of the code in this function was starter code from
 * "Unsw COMP1521 21T2"
 * 
 * @ex: Given: str="Hello     world. Beep boop"; sep=" "
 *      Outputs: ["Hello", "world.", "Beep", "boop", NULL]
 * 
 * @param str  - string to tokenise
 * @param sep  - Elements to delimit the token w/ 
 * @return char**  - Null terminated array of token strings
 */
char **tokenize (char *str, char *sep);


/**
 * @brief Given a set of NULL-termianted tokens, tokenize each token again.
 * @ex: Given: tokens=["Hello there ", "How are", "  you  ", NULL]; sep=" ";
 *      Outputs: [
 *                  ["Hello", "there", NULL],
 *                  ["How", "are", NULL],
 *                  ["you", NULL],
 *                  [NULL]
 *               ]
 * 
 * @param tokens - tokens to retokenize
 * @param sep    - Characters to delimit the token w/
 * @return char***  - Retokenized tokens
 */
char ***reTokenize(char **tokens, char *sep);


/**
 * @brief  Given a set of null terminated tokens, count the number of tokens
 * before the NULL element
 * 
 * @param tokens - tokens to count
 * @return size_t  - number of tokens
 */
size_t countTokens(char **tokens);


/**
 * @brief Counts the number of `tokens` sets in the NULL-terminated
 * array of tokens
 * 
 * @param tokens - Array to count
 * @return size_t  - Number of `char **tokens` elements in the array
 */
size_t countArrayTokensRow(char ***tokens);


/**
 * @brief Creates a copy of a string by value.
 * 
 * @param str  - string to copy
 * @return char* - copy of string
 */
char *copyString(char *str);


/**
 * @brief Prints an array of tokens w/ printTokens enacted on each element
 * of the array
 * 
 * @param tokens            - Tokens to print
 * @param label             - A Label to print beofre and after printing
 * @param comma_seperate    - Passed to printTokens to determine if shoudl
 * be comma seperated
 */
void printTokensArray(char ***tokens, char *label, bool comma_seperate);


/**
 * @brief Given an array of tokens and a target, returns the index of
 * the target within the tokens
 * 
 * @param tokens    - Tokens to search from
 * @param target    - Token to search for
 * @return int      - Index of tokesn
 */
int findMatchingTokenIndex(char **tokens, char *target);


/**
 * @brief Returns tokens containing only the unique elements from
 * an array of tokens 
 * 
 * @param tokens 
 * @return char** 
 */
char **fetchUniqueTokensFromTokenArray(char ***tokens);


/**
 * @brief Given a set of tokens, swap the tokens at the index given
 * 
 * @param tokens    - Set of tokens 
 * @param a         - index to swap
 * @param b         - index to swap
 */
void swapTokens(char **tokens, int a, int b);

/**
 * @brief in-place Bubble-sort for a set of tokens. Sorts using `strcmp`
 * 
 * @param tokens        - tokens to sort
 * @param ascending     - true: ascending order; false: descending
 */
void sortTokens(char **tokens, bool ascending);


/**
 * @brief generates and prints all permutations of a given set of elements
 * with no repeats. Does so `fixing` the first section of the array,
 * which recieves no changes. Then iteratively making swaps for the next
 * element of the array and recursing a `fixed` level deeper
 * 
 * @param tokens        - tokens to permuate
 * @param token_len     - number of tokens
 * @param fixed         - how many elements to `fix` in the array
 */
void generatePermutation(char **tokens, int token_len, int fixed);


/**
 * @brief Duplicates a set of tokens. Done by mallocing a newset and
 * iteratively using copyString
 * 
 * @param tokens        - tokens to duplicate
 * @return char**       - the new, duplicated set of tokens.
 */
char **duplicateTokens(char **tokens);


#endif // !TOKENS_H