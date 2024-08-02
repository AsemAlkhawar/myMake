#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "graph.h"


GraphNode* tree;

void exitWithError() {
    freeGraph(tree);
    exit(1);
}

int main(int argc, char* argv[]) {
    // Check for correct number of command-line arguments
    //if (argc != 3) {
    //    fprintf(stderr, "Usage: %s <Makefile> <Target>\n", argv[0]);
    //    return 1;
    //}
    //char* makefile = argv[1];
    //char* target = argv[2];
    char* makefile = "Makefile";
    char* target = "all";
    // Initialization of the graph and other variables
    GraphNode* graph = (GraphNode*)malloc(sizeof(GraphNode));
    // Ensure the graph is properly initialized
    if (!graph) {
        fprintf(stderr, "Failed to allocate memory for the graph.\n");
        return 1;
    }
    graph->name = strdup("main");  // Assuming 'main' is the root node name
    graph->firstChild = NULL;
    graph->right = NULL;
    graph->parent = NULL;
    graph->commands = NULL;
    graph->isPointerNode = 0;
    graph->originalNode = NULL;
    graph->printed = 0;

    tree = graph;

    // Read input and construct the graph
    if (readInputFromFile(makefile, &graph) != 0) {
        // Cleanup if reading the file fails
        free(graph);
        return 1;
    }

    // Find the target node and print its subtree
    GraphNode* targetNode = searchNode(graph, target, NULL);
    if (targetNode) {
        printSubtree(targetNode);
    }
    else {
        fprintf(stderr, "Target '%s' not found in the graph.\n", target);
        exitWithError();
    }
    freeGraph(graph);


    return 0;
}


