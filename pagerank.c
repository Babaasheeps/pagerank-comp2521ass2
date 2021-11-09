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


int main(int argc, char *argv[])
{
    // Sanitise input from command line
    double damping;
    double diffPR;
    int max_iterations;
    sanitiseArgs(argc, argv, &damping, &diffPR, &max_iterations);
    // printf("Got: %lf, %lf, %d\n", damping, diffPR, max_iterations);

    char **urls = getLinkCollection("collection.txt");
    int num_urls = (int)countTokens(urls);
    Graph g = GraphNew(num_urls);

    // Fill graph
    fillGraph(g, urls);
    printGraph(g);
    GraphFree(g);
    freeTokens(urls);

    return 0;
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
        Edge e = createEdge(src_index, dest_index, 1);
        GraphInsertEdge(g, e);
    }
}

int urlToIndex(char **urls, char *target)
{
    // Loop through urls. If same as target, return it. Returns -1 on failur
    for (int i = 0; urls[i] != NULL; i++)
    {
        int cmp =  strcmp(urls[i], target);
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

void exitBadInput()
{
    fprintf(stderr, "Invalid Input\n");
    exit(EXIT_FAILURE);
}