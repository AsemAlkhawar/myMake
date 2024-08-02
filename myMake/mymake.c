#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "graph.h"

char clean[1024];
GraphNode* tree;

void exitWithError() {
    freeGraph(tree);
    exit(1);
}

int main(int argc, char* argv[]) {

    char* makefile = "myMakefile"; 
    char* target = NULL;           
    int f_flag = 0;                 

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            if (f_flag) {  
                fprintf(stderr, "Error: -f appears more than once\n");
                exitWithError();
            }
            if (i + 1 < argc) {  
                makefile = argv[i + 1];  
                f_flag = 1;             
                i++;                     
            }
            else {
                fprintf(stderr, "Error: No makefile name specified after -f\n");
                exitWithError();
            }
        }
        else {
            if (target) { 
                fprintf(stderr, "Error: More than one target specified\n");
                exitWithError();
            }
            target = argv[i];  
        }
    }

    if (target != NULL && (strcmp(target, "clean") == 0 || strcmp(target, "clear") == 0)) {

        executeShellCommand(clean);
        return 0;
    }

    GraphNode* graph = (GraphNode*)malloc(sizeof(GraphNode));

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

    if (!target) {  
        
        target = readInputFromFile(makefile, &graph, 0);
    }
    else {
        readInputFromFile(makefile, &graph, 1);
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


