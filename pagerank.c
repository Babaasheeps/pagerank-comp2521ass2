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

double calculatePR(Graph g,
                   int num_urls,
                   double damp,
                   Vertex i,
                   double *PR_prev,
                   int *out_degrees,
                   int *in_degrees);

double calculateWeightSums(Graph g,
                           int num_urls,
                           Vertex i,
                           double *PR_prev,
                           int *out_degrees,
                           int *in_degrees);
double calculateWeightSum(Graph g, Vertex i, Vertex j, double *PR_prev, int *out_degrees, int *in_degrees);
double calculateWeightOut(Graph g, Vertex i, Vertex j, int *out_degrees);
double calculateWeightIn(Graph g, Vertex i, Vertex j, int *in_degrees);

double *initArrayDoubles(int n, double init_value);

int main(int argc, char *argv[])
{
    // Sanitise input from command line
    double damping;
    double diffPR;
    int max_iterations;
    sanitiseArgs(argc, argv, &damping, &diffPR, &max_iterations);

    char **urls = getLinkCollection("collection.txt");
    int num_urls = (int)countTokens(urls);
    Graph g = GraphNew(num_urls);
    printf("IN MAIN!!!!\n");
    printTokens(urls, true);

    // Fill graph
    fillGraph(g, urls);
    GraphShow(g);
    printGraph(g);

    // Run the pagerank algorithm
    double *ranks = pageRankW(g, damping, diffPR, max_iterations);
    int *out_degrees = outDegreeArray(g);
    printFinalRanks(urls, num_urls, out_degrees, ranks);

    free(out_degrees);
    GraphFree(g);
    free(ranks);
    freeTokens(urls);

    return 0;
}

double *pageRankW(Graph g, double damp, double diffPR, int max_iterations)
{
    int num_urls = GraphNumVertices(g);
    printf("There are %d nodes in the graph.\n", num_urls);
    double num_urls_recipricol = (double)(1.0 / num_urls);
    printf("num_urls = %d\n", num_urls);
    printf("num_urls_recipricol is %lf\n", num_urls_recipricol);
    printf("INitiating array prev\n");
    double *PR_prev = initArrayDoubles(num_urls, num_urls_recipricol);
    double *PR_new = initArrayDoubles(num_urls, (double)(0.0));
    printf("ON INITIALISATION\n");
    printDoubleArr(PR_prev, num_urls);
    printDoubleArr(PR_new, num_urls);
    int iteration = 0;
    double diff = diffPR;
    int *out_degrees = outDegreeArray(g);
    int *in_degrees = inDegreeArray(g);
    printf("OUT degree and INdegree are:\n");
    printIntArr(out_degrees, num_urls);
    printIntArr(in_degrees, num_urls);

    while (iteration < max_iterations && diff >= diffPR)
    {
        for (int i = 0; i < num_urls; i++)
        {
            PR_new[i] = calculatePR(g, num_urls, damp, i, PR_prev, out_degrees, in_degrees);
        }

        // Make PR_new the old array.
        diff = calculateDiff(PR_prev, PR_new, num_urls);
        // printf("Diff is: %lf\n", diff);
        iteration++;
        // printDoubleArr(PR_new, num_urls);
        // printDoubleArr(PR_prev, num_urls);
        // swapDoublePointers(&PR_new, &PR_prev);
    }
    // Return the last edited array and free the other
    // free(PR_new);
    return PR_prev;
}

double calculatePR(Graph g,
                   int num_urls,
                   double damp,
                   Vertex i,
                   double *PR_prev,
                   int *out_degrees,
                   int *in_degrees)
{
    double constant = (1.0 - damp) / num_urls;
    printf("constant is: %lf\n", constant);
    double weight_sums = calculateWeightSums(g, num_urls, i, PR_prev, out_degrees, in_degrees);
    return constant + (damp * weight_sums);
}

double calculateWeightSums(Graph g,
                           int num_urls,
                           Vertex i,
                           double *PR_prev,
                           int *out_degrees,
                           int *in_degrees)
{
    double sums = 0.0;
    // printf("FOR WEIGHT SUMS< PREV IS:\n");
    // printDoubleArr(PR_prev, num_urls);
    for (int j = 0; j < num_urls; j++)
    {
        // Sums for all p_j that are outbound to p_i
        // If pj not inbound, return 0;
        if (!GraphEdgeExists(g, j, i))
        {
            printf("No Edge from (%d, %d)\n", i, j);
            continue;
        }

        double W_in = calculateWeightIn(g, i, j, in_degrees);
        printf("Weight in for (%d, %d): is %lf\n", j, i, W_in);
        double W_out = calculateWeightOut(g, i, j, out_degrees);
        printf("For (%d,%d), (prev, in, out) is (%lf,%lf,%lf)\n",i, j, PR_prev[j], W_in, W_out);
        sums += PR_prev[j] * W_in + W_out;
    }
    return sums;
}

// ∑ p j ∈ M ( p i) P R ( p j , t) ∗ W i n ( p j , p i) ∗ W o u t ( p j , p i
double calculateWeightSum(Graph g, Vertex i, Vertex j, double *PR_prev, int *out_degrees, int *in_degrees)
{
    // Sums for all p_j that are outbound to p_i
    // If pj not inbound, return 0;
    if (!GraphEdgeExists(g, j, i))
        return 0.0;

    double W_in = calculateWeightIn(g, i, j, in_degrees);
    double W_out = calculateWeightOut(g, i, j, out_degrees);
    return PR_prev[j] * W_in * W_out;
}

double calculateWeightOut(Graph g, Vertex i, Vertex j, int *out_degrees)
{
    int num_urls = GraphNumVertices(g);
    int O_i = out_degrees[i];
    int denom_sum = 0;
    for (int k = 0; k < num_urls; k++)
    {
        // If edge from j->k, then add outsum of k. If outsum is 0, then add 0.5
        if (GraphEdgeExists(g, j, k))
            denom_sum += (out_degrees[k] == 0.0) ? 0.5 : out_degrees[k];
    }
    return (double)(O_i / denom_sum);
}

double calculateWeightIn(Graph g, Vertex i, Vertex j, int *in_degrees)
{
    int num_urls = GraphNumVertices(g);
    int O_i = in_degrees[i];
    int denom_sum = 0;
    for (int k = 0; k < num_urls; k++)
    {
        // If edge from j->k, then add outsum of k. If outsum is 0, then add 0.5
        if (GraphEdgeExists(g, j, k))
            denom_sum += (in_degrees[k] == 0.0) ? 0.5 : in_degrees[k];
    }
    return (double)(O_i / denom_sum);
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
        printf("Init value %d of %d with value %lf\n", i , n , init_value);
        arr[i] = init_value;
    }
    return arr;
}

void fillGraph(Graph g, char **urls)
{
    int num_urls = GraphNumVertices(g);
    printf("There are %d vert\n", num_urls);

    // For all of the urls in collection.txt, fill out their outgoing links
    for (int i = 0; urls[i] != NULL; i++)
    {
        printf("Looking for outoging in  %s\n", urls[i]);
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
        printf("Edge: (%d-%d)\n", src_index, dest_index);
        GraphInsertEdge(g, e);
    }
}

int urlToIndex(char **urls, char *target)
{
    printf("Looking for %s\n", target);
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
    assert (urls != NULL);
    assert (out_degrees != NULL);
    assert (ranks != NULL);

    printf("\n--------------------------------\n");
    printf("The Final Rankings Are\n");
    for (int i = 0; i < num_urls; i++)
    {
        printf("%s, %d, %lf\n", urls[i], out_degrees[i], ranks[i]);
    }
    printf("\n--------------------------------\n");
}




void exitBadInput()
{
    fprintf(stderr, "Invalid Input\n");
    exit(EXIT_FAILURE);
}