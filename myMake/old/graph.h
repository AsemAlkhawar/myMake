// graph.h
#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct GraphNode {
    char* name;
    struct GraphNode* firstChild;
    struct GraphNode* right;
    struct GraphNode* parent;
    struct CommandNode* commands;
    int isPointerNode;
    struct GraphNode* originalNode;
    int printed;

    struct stat fileInfo;  // New field to store file information
    int fileExists;        // New field to indicate if file exists (1 for exists, 0 for not exists)
} GraphNode;

typedef struct CommandNode {
    char* command;
    struct CommandNode* next;
} CommandNode;

//int isUpToDate = 1;

// Function prototypes
void remove_spaces(char* s);
void addChild(GraphNode* parent, char* childName, GraphNode* graph);
GraphNode* createGraph(char* input);
void addCommand(GraphNode* node, char* command);
int readInputFromFile(char* filename, GraphNode** graph);
GraphNode* findOrCreateNode(GraphNode** graph, char* nodeName);
GraphNode* searchNode(GraphNode* current, char* nodeName, GraphNode* callingParent);
void printSubtreehelper(GraphNode* current, GraphNode* callingParent, GraphNode* rootNode, int startPrinting);
void printCommands(GraphNode* node);
void printSubtree(GraphNode* rootNode);
int characterExists(char* str, char c);
GraphNode* findParentNode(GraphNode* current);
GraphNode* findLeftSibling(GraphNode* node);
void disconnectAndAddToNewParent(GraphNode* mainNode, GraphNode* node, GraphNode* newParent);
void freeGraph(GraphNode* node);
void executeShellCommand(const char* command);
int hasNewerChild(GraphNode* targetNode);
void initializeGraphNode(GraphNode* node);
void exitWithError();

#endif // GRAPH_H
