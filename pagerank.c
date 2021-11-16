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

#include "readData.h"
// #include "PQ.h"
#include "Graph.h"

typedef struct rank_data
{
    char *url;
    int out_degree;
    double rank;
} rank_data;
typedef rank_data *Rank;

void sanitiseArgs(int argc, char *argv[], double *damping, double *diffPR, int *max_iterations);
void exitBadInput();
void fillGraph(Graph g, char **urls);
void fillOutgoingLinks(Graph g, char **all_urls, char **outgoing_urls, int src_index);
int urlToIndex(char **urls, char *target);

double calculateDiff(double *arr1, double *arr2, int n);
void swapDoublePointers(double **a, double **b);
double *pageRankW(Graph g, double damp, double diffPR, int max_iterations);

void printDoubleArr(double *arr, int n);
void printIntArr(int *arr, int n);
void printFinalRanks(char **urls, int num_urls, int *out_degrees, double *ranks);
Rank newRank(char *url, int out_degree, double rank);
Rank *sanitiseRankData(int num_urls, char **urls, int *out_degrees, double *ranks);
void printRankData(Rank *ranks, int n);

int compare_rank(const void * a, const void * b);

char *copyString(char *str);

double calculatePR(Graph g,
                   int num_urls,
                   double damp,
                   Vertex i,
                   double *PR_prev,
                   double **W_ins,
                   double **W_outs);

double calculateWeightSums(Graph g,
                           int num_urls,
                           Vertex i,
                           double *PR_prev,
                           int *out_degrees,
                           int *in_degrees);
double calculateWeightSum(Graph g, Vertex i, Vertex j, double *PR_prev, int *out_degrees, int *in_degrees);
double calculateWeightOut(Graph g, Vertex i, Vertex j, int *out_degrees);
double calculateWeightIn(Graph g, Vertex i, Vertex j, int *in_degrees);

double **calculateW_ins(Graph g, int *in_degrees);
double **calculateW_outs(Graph g, int *out_degrees);
double **double2dArray(int h, int w, int initial);
void printDoubleArr2d(double **arr, int h, int w);
void freeDouble2dArray(double **arr, int h);

double *initArrayDoubles(int n, double init_value);
void swapRanks(Rank *ranks, int i, int j);
void freeRank(Rank r);
void freeRanks(Rank *ranks, int n);

void sortRankData(Rank *ranks, int n, int (*compare)(Rank, Rank));
int compareRankUrl(Rank r1, Rank r2);
int compareRanks(Rank r1, Rank r2);

int main(int argc, char *argv[])
{
    // Sanitise input from command line
    double damping;
    double diffPR;
    int max_iterations;
    sanitiseArgs(argc, argv, &damping, &diffPR, &max_iterations);

    // Read pages
    char **urls = getLinkCollection("collection.txt");
    int num_urls = (int)countTokens(urls);

    // Fill graph
    Graph g = GraphNew(num_urls);
    fillGraph(g, urls);

    // Run the pagerank algorithm
    double *ranks = pageRankW(g, damping, diffPR, max_iterations);
    int *out_degrees = outDegreeArray(g);

    // Sort Ranks and output
    printFinalRanks(urls, num_urls, out_degrees, ranks);

    // Free Memory
    GraphFree(g);

    return 0;
}

double *pageRankW(Graph g, double damp, double diffPR, int max_iterations)
{
    // Set up arrays to hold rankings
    int num_urls = GraphNumVertices(g);
    double num_urls_recipricol = (double)(1.0 / num_urls);
    double *PR_prev = initArrayDoubles(num_urls, num_urls_recipricol);
    double *PR_new = initArrayDoubles(num_urls, (double)(0.0));

    // Sets up weight ranks : * Move to function
    int *out_degrees = outDegreeArray(g);
    int *in_degrees = inDegreeArray(g);
    double **W_ins = calculateW_ins(g, in_degrees);
    double **W_outs = calculateW_outs(g, out_degrees);
    free(out_degrees);
    free(in_degrees);

    int iteration = 0;
    double diff = diffPR;
    while (iteration < max_iterations && diff >= diffPR)
    {
        for (int i = 0; i < num_urls; i++)
        {
            PR_new[i] = calculatePR(g, num_urls, damp, i, PR_prev, W_ins, W_outs);
        }
        // Swap make new -> prev and prev-> new so that it can be written to
        swapDoublePointers(&PR_new, &PR_prev);

        diff = calculateDiff(PR_prev, PR_new, num_urls);
        iteration++;
    }
    // Return the last edited array and free the other
    freeDouble2dArray(W_ins, num_urls);
    freeDouble2dArray(W_outs, num_urls);
    free(PR_new);
    return PR_prev;
}

double calculatePR(Graph g,
                   int num_urls,
                   double damp,
                   Vertex i,
                   double *PR_prev,
                   double **W_ins,
                   double **W_outs)
{
    double constant = (1.0 - damp) / num_urls;

    double weight_sum = 0.0;
    for (int j = 0; j < num_urls; j++)
    {
        // Latex: \sum_{j: edge j->i} PR_prev[j] * W_in(j, i) * W_out (j, i)
        if (GraphEdgeExists(g, j, i))
        {
            weight_sum += PR_prev[j] * W_ins[j][i] * W_outs[j][i];
        }
    }
    return constant + (damp * weight_sum);
}

double **calculateW_ins(Graph g, int *in_degrees)
{
    int n = GraphNumVertices(g);
    double **w_ins = double2dArray(n, n, 0.0);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            // if no edge from j -> i; then no W_out
            if (!GraphEdgeExists(g, j, i))
                continue;
            // Add weight (i, j) to weight array
            w_ins[j][i] = calculateWeightIn(g, i, j, in_degrees);
        }
    }
    return w_ins;
}

double **calculateW_outs(Graph g, int *out_degrees)
{
    int n = GraphNumVertices(g);
    double **w_outs = double2dArray(n, n, 0.0);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            // if no edge from j -> i; then no W_out
            if (GraphEdgeExists(g, j, i))
                // Add weight (i, j) to weight array
                w_outs[j][i] = calculateWeightOut(g, i, j, out_degrees);
        }
    }
    return w_outs;
}

double calculateWeightOut(Graph g, Vertex i, Vertex j, int *out_degrees)
{
    int num_urls = GraphNumVertices(g);
    double O_i = (out_degrees[i] == 0) ? 0.5 : out_degrees[i];
    double denom_sum = 0;
    for (int k = 0; k < num_urls; k++)
    {
        // If edge from j->k, then add outsum of k. If outsum is 0, then add 0.5
        if (GraphEdgeExists(g, j, k))
            denom_sum += (out_degrees[k] == 0.0) ? 0.5 : out_degrees[k];
    }
    return (((double)O_i) / (denom_sum));
}

double calculateWeightIn(Graph g, Vertex i, Vertex j, int *in_degrees)
{
    int num_urls = GraphNumVertices(g);
    int O_i = in_degrees[i];
    double denom_sum = 0;
    for (int k = 0; k < num_urls; k++)
    {
        // If edge from j->k, then add in_deg of k. If in_deg is 0, then add 0.5
        if (GraphEdgeExists(g, j, k))
            denom_sum += (in_degrees[k] == 0.0) ? 0.5 : in_degrees[k];
    }
    return (((double)O_i / denom_sum));
}

void printDoubleArr(double *arr, int n)
{
    printf("Array: [");
    for (int i = 0; i < n; i++)
    {
        printf(" %lf, ", arr[i]);
    }
    printf("]\n");
}

void printIntArr(int *arr, int n)
{
    printf("Array: [");
    for (int i = 0; i < n; i++)
    {
        printf(" %d, ", arr[i]);
    }
    printf("]\n");
}

void swapDoublePointers(double **a, double **b)
{
    double *temp = *a;
    *a = *b;
    *b = temp;
}

double calculateDiff(double *arr1, double *arr2, int n)
{
    double diff = 0;
    for (int i = 0; i < n; i++)
        diff += fabs(arr1[i] - arr2[i]);
    return diff;
}

double *initArrayDoubles(int n, double init_value)
{
    double *arr = malloc(sizeof(*arr) * n);
    assert(arr != NULL);

    for (int i = 0; i < n; i++)
    {
        arr[i] = init_value;
    }
    return arr;
}

void fillGraph(Graph g, char **urls)
{
    for (int i = 0; urls[i] != NULL; i++)
    {
        char **outgoing = outgoingLinks(urls[i]);
        fillOutgoingLinks(g, urls, outgoing, i);
        freeTokens(outgoing);
    }
}

void fillOutgoingLinks(Graph g, char **all_urls, char **outgoing_urls, int src_index)
{
    // For all outgoing urls, find their index and insert edge
    for (int i = 0; outgoing_urls[i] != NULL; i++)
    {
        int dest_index = urlToIndex(all_urls, outgoing_urls[i]);
        // Must ignore self-loops
        if (dest_index == src_index)
            continue;
        Edge e = createEdge(src_index, dest_index, 1);
        GraphInsertEdge(g, e);
    }
}

int urlToIndex(char **urls, char *target)
{
    // Loop through urls. If same as target, return it. Returns -1 on failur
    for (int i = 0; urls[i] != NULL; i++)
    {
        int cmp = strcmp(urls[i], target);
        if (!cmp)
            return i;
    }
    return -1;
}

void sanitiseArgs(int argc, char *argv[], double *damping, double *diffPR, int *max_iterations)
{
    assert(argc >= 3);
    char *endptr;
    *damping = strtod(argv[1], &endptr);
    if (endptr == NULL || endptr == argv[1])
        exitBadInput();
    if (*damping < 0.0 || *damping > 1.0)
        exitBadInput();

    *diffPR = strtod(argv[2], &endptr);
    if (endptr == NULL || endptr == argv[2])
        exitBadInput();
    if (*diffPR < 0.0)
        exitBadInput();

    *max_iterations = (int)strtol(argv[3], &endptr, 10);
    if (endptr == NULL || endptr == argv[3])
        exitBadInput();
    if (*max_iterations < 0)
        exitBadInput();
}

void printFinalRanks(char **urls, int num_urls, int *out_degrees, double *ranks)
{
    assert(urls != NULL);
    assert(out_degrees != NULL);
    assert(ranks != NULL);

    Rank *rank_data = sanitiseRankData(num_urls, urls, out_degrees, ranks);
    sortRankData(rank_data, num_urls, compareRankUrl);
    sortRankData(rank_data, num_urls, compareRanks);
    printRankData(rank_data, num_urls);
    freeRanks(rank_data, num_urls);
}

void freeRanks(Rank *ranks, int n)
{
    for (int i = 0; i < n; i++)
        freeRank(ranks[i]);
    free(ranks);
}

void freeRank(Rank r)
{
    free(r->url);
    free(r);
}

void sortRankData(Rank *ranks, int n, int (*compare)(Rank, Rank))
{
    // return;
    bool is_sorted = false;
    while (!is_sorted)
    {
        is_sorted = true;
        for (int i = 0; i < n - 1; i++)
        {
            if ((*compare)(ranks[i], ranks[i + 1]))
            {
                swapRanks(ranks, i, i + 1);
                is_sorted = false;
            }
        }
    }
}

int compareRanks(Rank r1, Rank r2)
{
    return (r1->rank < r2->rank);
}

int compareRankUrl(Rank r1, Rank r2)
{
    int cmp = strcmp(r1->url, r2->url);
    return (cmp > 0);
}

void swapRanks(Rank *ranks, int i, int j)
{
    Rank temp = ranks[i];
    ranks[i] = ranks[j];
    ranks[j] = temp;
}

void printRankData(Rank *ranks, int n)
{
    for (int i = 0; i < n; i++)
        printf("%s, %d, %.7lf\n",
               ranks[i]->url,
               ranks[i]->out_degree,
               ranks[i]->rank);
}

Rank *sanitiseRankData(int num_urls, char **urls, int *out_degrees, double *ranks)
{
    Rank *rank_data = malloc(sizeof(*ranks) * num_urls);

    // Couple urls, out_degrees and rank to rank data struct.
    for (int i = 0; i < num_urls; i++)
    {
        rank_data[i] = newRank(urls[i], out_degrees[i], ranks[i]);
    }
    // Free data passed in. - Copied by value, not reference.
    freeTokens(urls);
    free(out_degrees);
    free(ranks);

    return rank_data;
}

Rank newRank(char *url, int out_degree, double rank)
{
    Rank new = malloc(sizeof(*new));
    char *url_name = copyString(url);
    new->url = url_name;
    new->out_degree = out_degree;
    new->rank = rank;
    return new;
}
char *copyString(char *str)
{
    int new_len = strlen(str) + 1;
    char *new = malloc(sizeof(char) * (new_len));
    assert(new != NULL);
    strncpy(new, str, new_len);

    return new;
}

double **double2dArray(int h, int w, int initial)
{
    double **arr = malloc(sizeof(*arr) * h);
    for (int i = 0; i < h; i++)
        arr[i] = initArrayDoubles(w, initial);

    return arr;
}

void freeDouble2dArray(double **arr, int h)
{
    for (int i = 0; i < h; i++)
        free(arr[i]);
    free(arr);
}

void printDoubleArr2d(double **arr, int h, int w)
{
    printf("[\n");
    for (int i = 0; i < h; i++)
    {
        printf("    %d\t", i);
        printDoubleArr(arr[i], w);
    }
    printf("]\n");
}

void exitBadInput()
{
    fprintf(stderr, "Invalid Input\n");

    exit(EXIT_FAILURE);
}