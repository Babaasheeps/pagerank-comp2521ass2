// UNSW COMP2521 21T3 Starter Code

// Interface to the Undirected Weighted Graph ADT
// - Vertices are identified by integers between 0 and nV - 1,
//   where nV is the number of vertices in the graph
// - Weights are doubles and must be positive
// - Self-loops are not allowed


#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

typedef struct graph *Graph;

typedef int Vertex;

// edges are pairs of vertices (end-points)
typedef struct Edge {
    Vertex v;
    Vertex w;
    double weight;
} Edge;

/**
 * Creates a new instance of a graph
 */
Graph  GraphNew(int nV);

/**
 * Frees all memory associated with the given graph
 */
void   GraphFree(Graph g);

/**
 * Returns the number of vertices in the graph
 */
int    GraphNumVertices(Graph g);

/**
 * Inserts  an  edge into a graph. Does nothing if there is already an
 * edge between `e.v` and `e.w`. Returns true if successful, and false
 * if there was already an edge.
  */
bool   GraphInsertEdge(Graph g, Edge e);

/**
 * Removes an edge from a graph. Returns true if successful, and false
 * if the edge did not exist.
 */
bool   GraphRemoveEdge(Graph g, Vertex v, Vertex w);

/**
 * Returns the weight of the edge between `v` and `w` if it exists, or
 * 0.0 otherwise
 */
double GraphIsAdjacent(Graph g, Vertex v, Vertex w);

/**
 * Returns true if the graph contains a cycle, and false otherwise
 */
bool   GraphHasCycle(Graph g);

/**
 * Returns a minimum spanning tree of the given graph. The given graph
 * should not be modified. Returns NULL if the graph has no minimum 
 * spanning tree.
 */
Graph  GraphMST(Graph g);

/**
 * Displays information about the graph
 */
void   GraphShow(Graph g);

/**
 * Retuns an Edge Struct, filled with the provided information.
 */
Edge createEdge(Vertex src, Vertex dst, double weight);

// FUNCTIONS ABOVE THIS POINT ARE STARTER CODE FROM 2521 LABS
// ##################################################################### //
// ########################## SPLITTING POINT ########################## //
// ##################################################################### //
// FUNCTION BELOW THESE POINT ARE CUSTOM


/** 
 * Prints the adjacency matrix of a graph
 */
void printGraph(Graph g);

/**
 * Returns true if an edge between the start and dest node exists.
 */
bool GraphEdgeExists(Graph g, Vertex start, Vertex dest);

/**
 * Calculates outdegree of a given vertex
 */
int outDegree(Graph g, Vertex v);

/**
 * Calculates indegree for a given vertex
 */
int inDegree(Graph g, Vertex w);

/**
 * Create an array containing indegrees for all vertices. Indexed by vertex
 */
int *inDegreeArray(Graph g);

/**
 * Creates an array containing outdegrees for all vertexes. Indexed by vertex
 */
int *outDegreeArray(Graph g);

#endif
