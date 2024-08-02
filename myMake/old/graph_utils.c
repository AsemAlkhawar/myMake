// graph_utils.c
#include "graph.h"


void remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}

void addCommand(GraphNode* node, char* command) {

    CommandNode* newCommand = (CommandNode*)malloc(sizeof(CommandNode));
    newCommand->command = strdup(command);
    newCommand->next = NULL;  // New command node should point to NULL, as it is at the end

    if (node->commands == NULL) {
        // If there are no commands yet, this is the first command
        node->commands = newCommand;
    }
    else {
        // If there are already commands, traverse to the end of the list and add the new command
        CommandNode* currentCommand = node->commands;
        while (currentCommand->next != NULL) {
            currentCommand = currentCommand->next;
        }
        currentCommand->next = newCommand;  // Append the new command at the end
    }
}

int readInputFromFile(char* filename, GraphNode** graph) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open file %s for reading.\n", filename);
        exitWithError();
    }

    char line[1024];
    GraphNode* currentParent = NULL;

    while (fgets(line, sizeof(line), file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        // Ignore empty lines
        if (strlen(line) == 0) continue;

        // If line starts with a tab, it's a command
        if (line[0] == '\t') {
            if (currentParent == NULL) {
                printf("command without a target: %s", line + 1);
                return 1;
            }
            addCommand(currentParent, line + 1); // Skip the tab character
            continue;
        }

        // Split the line to get parent and child nodes
        if (!characterExists(line, ':')) {
            printf("No ':' on definition line: %s", line);

            return 1;
        }
        char* token = strtok(line, ":");
        remove_spaces(token);

        if (token == "clean" || token == "clear") {
            fgets(line, sizeof(line), file);
            line[strcspn(line, "\n")] = 0;
            executeShellCommand(line + 1);
            continue;
        }
        char* parentName = token;
        GraphNode* test = searchNode(*graph, token, NULL);
        if (searchNode(*graph, token, NULL) && searchNode(*graph, token, NULL)->firstChild) {
            printf("Target, %s, declared more than once", token);
            return 1;
        }
        currentParent = findOrCreateNode(graph, parentName);

        token = strtok(NULL, " ");
        while (token != NULL) {
            GraphNode* existingNode = searchNode(*graph, token, NULL);  // Search for the node in the graph

            if (existingNode && findParentNode(existingNode) == *graph) {
                // If the node exists and its parent is the main node, disconnect and add to the right parent
                disconnectAndAddToNewParent(*graph, existingNode, currentParent);
            }
            else {

                addChild(currentParent, token, *graph);
            }
            token = strtok(NULL, " ");
        }

    }

    fclose(file);
    return 0;
}

void freeCommands(CommandNode* command) {
    while (command != NULL) {
        CommandNode* temp = command;
        command = command->next;
        free(temp->command);
        free(temp);
    }
}

void freeGraph(GraphNode* node) {
    if (node == NULL) return;

    freeGraph(node->firstChild);
    freeGraph(node->right);

    if (node->name != NULL) {
        free(node->name);
    }

    if (node->commands != NULL) {
        freeCommands(node->commands);
    }

    free(node);
}

void initializeGraphNode(GraphNode* node) {
    if (node == NULL) {
        fprintf(stderr, "Error: Node is NULL\n");
        return;
    }

    GraphNode* targetNode = node;

    // Check if it's a pointer node
    if (node->isPointerNode && node->originalNode != NULL) {
        // Initialize the original node
        initializeGraphNode(node->originalNode);

        // Copy fileInfo and fileExists from originalNode to the pointer node
        node->fileInfo = node->originalNode->fileInfo;
        node->fileExists = node->originalNode->fileExists;
    }
    else {
        // For non-pointer nodes or pointer nodes with no original node
        if (stat(node->name, &node->fileInfo) == 0) {
            // File exists, populate fileInfo and set fileExists to 1
            node->fileExists = 1;
        }
        else {
            // File does not exist, set fileExists to 0
            node->fileExists = 0;
        }
    }
}