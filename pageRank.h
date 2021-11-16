#ifndef PAGE_RANK_H 
#define PAGE_RANK_H

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readData.h"
#include "Graph.h"


// Struct to couple together key parts of data.
typedef struct rank_data
{
    char *url;
    int out_degree;
    double rank;
} rank_data;
typedef rank_data *Rank;

/**
 * @brief Expects three arguments in argv. Checks type and range. Exits if
 * the inputs are invalid. Otherwise, reads them to an argument.
 * 
 * @param argc Self-explanatory
 * @param argv - Command Line arguments
 * @param damping - Holds the damping factor
 * @param diffPR - Holds diffPR
 * @param max_iterations - Hold max_iterations
 */
void sanitiseArgs(int argc, char *argv[], double *damping, double *diffPR, int *max_iterations);


/**
 * @brief Exits the program due to improper input. 
 * 
 */
void exitBadInput();


/**
 * @brief Loops through vertex data to initialise edges in the graph
 * 
 * @param g - Graph to make changes to 
 * @param urls - string tokens containing urls
 */
void fillGraph(Graph g, char **urls);

/**
 * @brief Fills out all outgoing edges from a given `src` vertex.
 * 
 * @param g - Graph to fill in
 * @param all_urls - Tokenised names of every url
 * @param outgoing_urls - Outgoing urls from src
 * @param src_index - Index of the src vertex
 */
void fillOutgoingLinks(Graph g, char **all_urls, char **outgoing_urls, int src_index);


/**
 * @brief Given a url, return its index in the graph.
 * 
 * @param urls - List of all URLS
 * @param target - Url being searched for
 * @return int - Index of the target url
 */
int urlToIndex(char **urls, char *target);


/**
 * @brief Calculate the diff value as per pageRank spec
 * diff = \sum_{i=1}^{n} |PR(p_i, t + 1) - PR(p_i, t)|
 * Since absolute value is taken, order of arrays passed in
 * does not matter.
 * 
 * @param arr1 - PR - old  
 * @param arr2 - PR - new
 * @param n    - number of elemens / length of arr1 and arr2
 * @return double  - diff value
 */
double calculateDiff(double *arr1, double *arr2, int n);


/**
 * @brief Swaps two points to a double
 * 
 * @param a - pointer to double - swapped with b
 * @param b - pointer to double - swapped with a
 */
void swapDoublePointers(double **a, double **b);


/**
 * @brief Updates Calculates weighted page rank.
 * 
 * @param g - Graph to get edges from
 * @param damp - damping factor
 * @param diffPR - lower bound for diff. Stop when diffPR < diff
 * @param max_iterations - max_number of times to iterate
 * @return double*  - Array of page ranks. Indexes correspond with indexes
 * in the graph.
 */
double *pageRankW(Graph g, double damp, double diffPR, int max_iterations);

/**
 * @brief Prints out an array of doubles. Same format as python list
 * 
 * @param arr  - Array to print
 * @param n    - number of elements in the array to print
 */
void printDoubleArr(double *arr, int n);

/**
 * @brief Prints out an array of integers. Same format as python list
 * 
 * @param arr  - Array to print
 * @param n    - number of elements in the array to print
 */
void printIntArr(int *arr, int n);


/**
 * @brief sanitise, and sort the output, then print it.
 * 
 * @param urls - array of names of urls
 * @param num_urls - number of urls to priont
 * @param out_degrees  - out_degrees of urls. 
 * @param ranks - ranks of urls
 */
void printFinalRanks(char **urls, int num_urls, int *out_degrees, double *ranks);


/**
 * @brief Create a new pointer to a Rank struct. 
 * 
 * @param url - string of url to make. A duplicate of this string will be
 * made.
 * @param out_degree
 * @param rank 
 * @return Rank  - pointer to a rank struct
 */
Rank newRank(char *url, int out_degree, double rank);


/**
 * @brief Couples the urls, outdegrees and ranks to a single struct array
 * 
 * @param num_urls - number of urls to couple
 * @param urls  - strings for names of url
 * @param out_degrees - out_degree
 * @param ranks - weighted ranks of urls
 * @return Rank*  - Array of Rank structs
 */
Rank *sanitiseRankData(int num_urls, char **urls, int *out_degrees, double *ranks);


/**
 * @brief prints ranksin the format below
 *  <url_name>, <out_degree>, <weighted_rank>
 * 
 * @param ranks Array of ranks to print
 * @param n - number of ranks
 */
void printRankData(Rank *ranks, int n);


/**
 * @brief Compares the weighted ranks of Rank structs
 * 
 * @param a - Rank to comapre
 * @param b - Rank to compare
 * @return int - Functionally a bool, if a < b
 */
int compare_rank(const void * a, const void * b);


/**
 * @brief Given a string, mallocs a copy of it and returns it
 * 
 * @param str  - string to copy
 * @return char* - copy of string
 */
char *copyString(char *str);

/**
 * @brief Calculate Page Rank as per PageRank Spec
 *
 * PR(p_i, t + 1) = \frac{1 - d}{N} + d \ast \sum_{p_j \in M(p_i)} PR(p_j,
 * t) \ast W_{(p_j, p_i)}^{in} \ast W_{(p_j, p_i)}^{out}
 *
 * @param g - graph containing edges wrt links between pages
 * @param num_urls - number of urls. Is the length of the PR_prev
 * @param damp  - damping factor
 * @param i     - Which url to calculate a rank for 
 * @param PR_prev  - Array of ranks from the previous iteration
 * @param W_ins    - W_in array as per spec
 * @param W_outs   - W_out array as per spec
 * @return double  - Rank for given url `i`
 */
double calculatePR(Graph g,
                   int num_urls,
                   double damp,
                   Vertex i,
                   double *PR_prev,
                   double **W_ins,
                   double **W_outs);


/**
 * @brief Calcuate weight sums (j, i)
 * 
 * @param g - graph of edges for urls
 * @param i  
 * @param j 
 * @param PR_prev - previous page rank array
 * @param out_degrees - array of out_degrees
 * @param in_degrees - array of in_degrees
 * @return double 
 */
double calculateWeightSum(Graph g, Vertex i, Vertex j, double *PR_prev, int *out_degrees, int *in_degrees);


/**
 * @brief Creates 2d array of W_in as per spec
 * 
 * @param g - graph to base links off
 * @param in_degrees  - array of in_degrees
 * @return double** - array of out_degrees
 */
double **calculateW_ins(Graph g, int *in_degrees);


/**
 * @brief 
 * 
 * @param g 
 * @param out_degrees 
 * @return double** 
 */
double **calculateW_outs(Graph g, int *out_degrees);

/**
 * @brief Calculate W_out (i, j)
 * 
 * @param g  - graph to base links off
 * @param i 
 * @param j 
 * @param out_degrees  - array of out_degrees
 * @return double   - W_out
 */
double calculateWeightOut(Graph g, Vertex i, Vertex j, int *out_degrees);


/**
 * @brief Calculate W_in(i, j)
 * 
 * @param g  - graph to base links off
 * @param i 
 * @param j 
 * @param in_degrees - array of in_degrees
 * @return double   - W_out
 */
double calculateWeightIn(Graph g, Vertex i, Vertex j, int *in_degrees);


/**
 * @brief creates a 2d array of doubles
 * 
 * @param h - height of the array
 * @param w - width of the array
 * @param initial  - which value to initialize each element of the array
 * @return double** - 2d array of doubles 
 */
double **double2dArray(int h, int w, int initial);


/**
 * @brief Prints 2d array of double
 * 
 * @param arr - array to priont
 * @param h - height of the array
 * @param w - width of the array
 */
void printDoubleArr2d(double **arr, int h, int w);

/**
 * @brief Free a 2d array of doubles
 * 
 * @param arr - array to free
 * @param h - height of array
 */
void freeDouble2dArray(double **arr, int h);

/**
 * @brief create an array of doubles
 * 
 * @param n - numbner of elements in array
 * @param init_value - initial value of each element
 * @return double* 
 */
double *initArrayDoubles(int n, double init_value);


/**
 * @brief Given an array of Ranks, swaps the elements at
 * index i and j 
 * 
 * @param ranks - array of Ranks
 * @param i 
 * @param j 
 */
void swapRanks(Rank *ranks, int i, int j);


/**
 * @brief Free all mmeory associated with a rank
 * 
 * @param r - rank to free
 */
void freeRank(Rank r);


/**
 * @brief Free an array of ranks 
 * 
 * @param ranks - rank array to free
 * @param n - number of elements in array
 */
void freeRanks(Rank *ranks, int n);

/**
 * @brief sort an array of ranks. - Standard bubblesort
 * 
 * @param ranks - array to sort
 * @param n  - number of elements in the array 
 * @param compare  - function to compare elements
 */
void sortRankData(Rank *ranks, int n, int (*compare)(Rank, Rank));

/**
 * @brief Compares elements by url_name. Functionally just a wrapper
 * for strcmp
 * 
 * @param r1 elemnt to compare
 * @param r2 element to compare
 * @return int - strcmp of r1 and r2's names
 */
int compareRankUrl(Rank r1, Rank r2);


/**
 * @brief compares Ranks by their weighter rank value
 * 
 * @param r1 
 * @param r2 
 * @return int  - true iff r1 < r2
 */
int compareRanks(Rank r1, Rank r2);

#endif //  PAGE_RANK_H!