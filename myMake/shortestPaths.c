/**
 * @file shortestPaths.c
 * @brief Implements Dijkstra's shortest path algorithm for a graph.
 * @author Ahmad Gaber
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct Edge {
    char* dest;
    int dist;
    struct Edge* next;
} Edge;

typedef struct Vertex {
    char* name;
    int minDist;
    int visited;
    Edge* edges;
    struct Vertex* next;
} Vertex;

typedef struct Graph {
    Vertex* vertices;
} Graph;

// Function Prototypes
Graph* initGraph();
void addVertex(Graph* g, const char* name);
Vertex* findVertex(Graph* g, const char* name);
void addEdge(Graph* g, const char* src, const char* dest, int dist);
void readGraphFromFile(Graph* g, const char* filename);
void initDijkstra(Graph* g, const char* start);
Vertex* findMinDistVertex(Graph* g);
int dijkstra(Graph* g, const char* start, const char* end);

/**
 * @brief Initializes a new graph.
 * @return A pointer to the newly initialized graph.
 */
Graph* initGraph() {
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->vertices = NULL;
    return g;
}

/**
 * @brief Adds a vertex to the graph.
 * @param g The graph.
 * @param name The name of the vertex.
 */
void addVertex(Graph* g, const char* name) {
    Vertex* cur = g->vertices;
    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0) {
            return;
        }
        cur = cur->next;
    }
    Vertex* new_vertex = (Vertex*)malloc(sizeof(Vertex));
    new_vertex->name = strdup(name);
    new_vertex->minDist = -1;
    new_vertex->visited = 0;
    new_vertex->edges = NULL;
    new_vertex->next = g->vertices;
    g->vertices = new_vertex;
}

/**
 * @brief Finds a vertex in the graph by its name.
 * @param g The graph.
 * @param name The name of the vertex.
 * @return A pointer to the vertex if found, NULL otherwise.
 */
Vertex* findVertex(Graph* g, const char* name) {
    Vertex* cur = g->vertices;
    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

/**
 * @brief Adds an edge between two vertices in the graph.
 * @param g The graph.
 * @param src The source vertex.
 * @param dest The destination vertex.
 * @param dist The distance between the vertices.
 */
void addEdge(Graph* g, const char* src, const char* dest, int dist) {
    Vertex* src_vertex = findVertex(g, src);
    Vertex* dest_vertex = findVertex(g, dest);
    if (src_vertex == NULL || dest_vertex == NULL) {
        return;
    }
    Edge* new_edge = (Edge*)malloc(sizeof(Edge));
    new_edge->dest = strdup(dest);
    new_edge->dist = dist;
    new_edge->next = src_vertex->edges;
    src_vertex->edges = new_edge;
    new_edge = (Edge*)malloc(sizeof(Edge));
    new_edge->dest = strdup(src);
    new_edge->dist = dist;
    new_edge->next = dest_vertex->edges;
    dest_vertex->edges = new_edge;
}

/**
 * @brief Reads a graph from a file.
 * @param g The graph.
 * @param filename The name of the file.
 */
void readGraphFromFile(Graph* g, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(1);
    }
    char src[65], dest[65];
    int dist;
    while (fscanf(file, "%s %s %d", src, dest, &dist) != EOF) {
        addVertex(g, src);
        addVertex(g, dest);
        addEdge(g, src, dest, dist);
    }
    fclose(file);
}

/**
 * @brief Initializes the Dijkstra algorithm for a graph.
 * @param g The graph.
 * @param start The starting vertex.
 */
void initDijkstra(Graph* g, const char* start) {
    Vertex* cur = g->vertices;
    while (cur != NULL) {
        cur->minDist = (strcmp(cur->name, start) == 0) ? 0 : INT_MAX;
        cur->visited = 0;  // Set to 0
        cur = cur->next;
    }
}
/**
 * @brief Finds the vertex with the minimum distance in the graph.
 * @param g The graph.
 * @return A pointer to the vertex with the minimum distance.
 */
Vertex* findMinDistVertex(Graph* g) {
    Vertex* min_vertex = NULL;
    int min_dist = INT_MAX;
    Vertex* cur = g->vertices;
    while (cur != NULL) {
        if (!cur->visited && cur->minDist < min_dist) {
            min_dist = cur->minDist;
            min_vertex = cur;
        }
        cur = cur->next;
    }
    if (min_vertex != NULL) {
        min_vertex->visited = 1;  // Mark as visited
    }
    return min_vertex;
}

/**
 * @brief Implements Dijkstra's algorithm to find the shortest path between two vertices.
 * @param g The graph.
 * @param start The starting vertex.
 * @param end The ending vertex.
 * @return The shortest distance between the two vertices.
 */
int dijkstra(Graph* g, const char* start, const char* end) {
    initDijkstra(g, start);
    Vertex* cur;
    while ((cur = findMinDistVertex(g)) != NULL) {
        Edge* edge = cur->edges;
        while (edge != NULL) {
            Vertex* dest_vertex = findVertex(g, edge->dest);
            if (dest_vertex->minDist != -1) {
                int new_dist = cur->minDist + edge->dist;
                if (new_dist < dest_vertex->minDist) {
                    dest_vertex->minDist = new_dist;
                }
            }
            edge = edge->next;
        }
    }
    Vertex* end_vertex = findVertex(g, end);
    if (end_vertex == NULL || end_vertex->minDist == INT_MAX) {
        return -1;
    }
    return end_vertex->minDist;
}

/**
 * @brief Frees the memory allocated for the graph.
 * @param g The graph.
 */
void freeGraph(Graph* g) {
    Vertex* cur_vertex = g->vertices;
    while (cur_vertex != NULL) {
        Edge* cur_edge = cur_vertex->edges;
        while (cur_edge != NULL) {
            Edge* temp_edge = cur_edge;
            cur_edge = cur_edge->next;
            free(temp_edge->dest);
            free(temp_edge);
        }
        Vertex* temp_vertex = cur_vertex;
        cur_vertex = cur_vertex->next;
        free(temp_vertex->name);
        free(temp_vertex);
    }
    free(g);
}

// Main Function
int main(int argc, char* argv[]) {
        //if (argc != 2) {
        //    fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
        //    return 1;
        //}

    Graph* g = initGraph();
    readGraphFromFile(g, "input.txt"); // replace with argv[0]

    char start[65], end[65];
    while (scanf("%s %s", start, end) != EOF) {
        int dist = dijkstra(g, start, end);
        if (dist == -1) {
            printf("Path not found between %s and %s.\n", start, end);
        }
        else {
            printf("%d\n", dist);
        }
    }

    freeGraph(g);

    return 0;
}
