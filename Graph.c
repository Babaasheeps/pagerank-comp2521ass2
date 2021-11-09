// Implementation of the Undirected Weighted Graph ADT
// Uses an adjacency matrix

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
// #include "PQ.h"
#include <math.h>


struct graph {
    int nV;         // #vertices
    int nE;         // #edges
    double **edges; // adjacency matrix storing positive weights
                    // 0 if nodes not adjacent
};

static bool doHasCycle(Graph g, Vertex v, Vertex prev, bool *visited);
static int  validVertex(Graph g, Vertex v);
double *initInfArray(int n);


// Edge popValidEdgePQ(PQ pq, bool *visited);
// Edge failure();
// bool isFailure(Edge e);
// void enqueueAdjacentEdgesPQ(PQ pq, Graph g, Vertex v);
////////////////////////////////////////////////////////////////////////

Graph GraphNew(int nV) {
    assert(nV > 0);

    Graph g = malloc(sizeof(*g));
    if (g == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    g->nV = nV;
    g->nE = 0;

    g->edges = malloc(nV * sizeof(double *));
    if (g->edges == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nV; i++) {
        g->edges[i] = calloc(nV, sizeof(double));
        if (g->edges[i] == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }

    return g;
}

void GraphFree(Graph g) {
    for (int i = 0; i < g->nV; i++) {
        free(g->edges[i]);
    }
    free(g->edges);
    free(g);
}

////////////////////////////////////////////////////////////////////////

int GraphNumVertices(Graph g) {
    return g->nV;
}

bool GraphInsertEdge(Graph g, Edge e) {
    assert(validVertex(g, e.v));
    assert(validVertex(g, e.w));
    assert(e.v != e.w);
    assert(e.weight > 0.0);

    if (g->edges[e.v][e.w] == 0.0) {
        g->edges[e.v][e.w] = e.weight;
        // g->edges[e.w][e.v] = e.weight; // Uncomment if undirected
        g->nE++;
        return true;
    } else {
        return false;
    }
}

bool GraphRemoveEdge(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));

    if (g->edges[v][w] != 0.0) {   // edge e in graph
        g->edges[v][w] = 0.0;
        // g->edges[w][v] = 0.0;    // Uncomment if undirected
        g->nE--;
        return true;
    } else {
        return false;
    }
}

double GraphIsAdjacent(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));
    
    return g->edges[v][w];
}

void GraphShow(Graph g) {
    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (int v = 0; v < g->nV; v++) {
        for (int w = v + 1; w < g->nV; w++) {
            if (g->edges[v][w] != 0.0) {
                printf("Edge %d - %d: %lf\n", v, w, g->edges[v][w]);
            }
        }
    }
}

bool GraphHasCycle(Graph g) {
    bool *visited = calloc(g->nV, sizeof(bool));
    assert(visited != NULL); // lazy error checking
    
    for (int v = 0; v < g->nV; v++) {
        if (!visited[v] && doHasCycle(g, v, v, visited)) {
            free(visited);
            return true;
        }
    }

    free(visited);
    return false;
}

static bool doHasCycle(Graph g, Vertex v, Vertex prev, bool *visited) {
    visited[v] = true;
    for (int w = 0; w < g->nV; w++) {
        if (g->edges[v][w] != 0.0) {
            if (!visited[w]) {
                if (doHasCycle(g, w, v, visited)) {
                    return true;
                }
            } else if (w != prev) {
                return true;
            }
        }
    }
    return false;
}

Edge createEdge(Vertex src, Vertex dst, double weight)
{
    Edge e;
    e.v = src;
    e.w = dst;
    e.weight = weight;
    return e;
}

bool GraphEdgeExists(Graph g, Vertex start, Vertex dest)
{
    assert(validVertex(g, start));
    assert(validVertex(g, dest));
    return (g->edges[start][dest] != 0.0);
}

int outDegree(Graph g, Vertex v)
{
    int out = 0;
    for (int w = 0; w < g->nV; w++)
    {
        if (g->edges[v][w] != 0.0)
            out++;
    }
    return out;
}

int inDegree(Graph g, Vertex w)
{
    int in = 0;
    for (int v = 0; v < g->nV; v++)
    {
        if (g->edges[w][v] != 0.0)
            in++;
    }
    return in;
}

int *outDegreeArray(Graph g)
{
    int *out = malloc(sizeof(*out) * g->nV);
    for (int i = 0; i < g->nV; i++)
        out[i] = outDegree(g, i);
    return out;
}

int *inDegreeArray(Graph g)
{
    int *in = malloc(sizeof(*in) * g->nV);
    for (int i = 0; i < g->nV; i++)
        in[i] = inDegree(g, i);
    return in;
}

////////////////////////////////////////////////////////////////////////
// // Your task
// // ! Task 1
// // Returns the minimum spanning tree of the graph `g` using PRIM's
// // algorithmn w/ a priority queue.
// Graph GraphMST(Graph g)
// {
//     // Data structures to handle the PRIMS
//     Graph mst = GraphNew(g->nV);
//     PQ pq = PQNew();
//     bool *in_mst = malloc(g->nV * sizeof(*in_mst));
//     int used_v = 0;

//     enqueueAdjacentEdgesPQ(pq, g, 0);        // Add neighbours of 0th vertex to PQ
//     in_mst[0] = true;

//     while (used_v < g->nV)
//     {
//         // Find e = {s, t, w} in unused st s in usedV and t not used 
//         // and w in min weight of all such edges
//         Edge e = popValidEdgePQ(pq, in_mst);
//         // printf("Edge is: (%d-%d)\n", e.v, e.w);
//         if (isFailure(e))
//             break;

//         // Find the vertex that isnt in visited. PQ its neighbours
//         Vertex unvisited = (in_mst[e.v]) ? e.w : e.v;
//         enqueueAdjacentEdgesPQ(pq, g, unvisited);
//         in_mst[unvisited] = true;

//         // Add edge to mst, then mark the vertexes as visited
//         GraphInsertEdge(mst, e);
//         used_v++;
//     }
//     free(in_mst);
//     // printGraph(mst);
//     return mst;
// }

// // Initialises an array of `n` doubles, as infinite. Deprecated since PQ
// // being used instead
// double *initInfArray(int n)
// {
//     double *arr = malloc(sizeof(*arr) * n);
//     for (int i = 0; i < n; i++)
//         arr[i] = INFINITY;
//     return arr;
// }

// // Extracts from the priority queue till an edge is found such that
// // exactly on of its vertices has been visited.
// Edge popValidEdgePQ(PQ pq, bool *visited)
// {
//     while (!PQIsEmpty(pq))
//     {
//         // Find edge s.t. exactly one vertex is visited
//         Edge e = PQExtract(pq);
//         if (visited[e.v] != visited[e.w])
//             return e;
//     }
//     return failure();
// }

// // Returns a 'failure' Edge with both vectors as `-1` and infinite weight
// Edge failure()
// {
//     Edge failure;
//     failure.v = -1;
//     failure.w = -1;
//     failure.weight = INFINITY;
//     return failure;
// }

// // Returns if the given edge `e` is a failure edge.
// bool isFailure(Edge e)
// {
//     if (e.v == -1 && e.w == -1 && e.weight == INFINITY)
//         return true;
//     return false;
// }

// // Inserts all edges adjacent (non-zero weighted) to the given vertex `v` into
// // the priority queue
// void enqueueAdjacentEdgesPQ(PQ pq, Graph g, Vertex v)
// {
//     int n = g->nV;
//     for (int i = 0; i < n; i++)
//     {
//         if (g->edges[i][v] == 0.0)
//             continue;
//         // Queue non empty edge
//         Edge e;
//         e.v = i;
//         e.w = v;
//         e.weight = g->edges[i][v];
//         PQInsert(pq, e);
//     }
// }

////////////////////////////////////////////////////////////////////////

static int validVertex(Graph g, Vertex v) {

    bool is_valid = (v >= 0 && v < g->nV);
    if (is_valid)
        return true;
    printf("Invalid vertex: %d. Must be in range (0-%d)\n", v, g->nV);
    return false;
}

void printGraph(Graph g)
{
    int size = GraphNumVertices(g);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%lf ", g->edges[i][j]);
        }
        putchar('\n');
    }
    
}
