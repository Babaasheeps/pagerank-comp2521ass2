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

#endif //  PAGE_RANK_H!