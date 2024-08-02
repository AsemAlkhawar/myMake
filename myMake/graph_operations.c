// graph_operations.c
#include "graph.h"


GraphNode* searchNode(GraphNode* current, char* nodeName, GraphNode* callingParent) {
    if (!current) {
        return NULL;  // Base case
    }

    // Check the current node, but skip if it's a pointer node
    if (strcmp(current->name, nodeName) == 0 && !current->isPointerNode) {
        return current;  // Node found
    }

    // Check for child, go to child if exists and not backtracking to calling parent
    if (current->firstChild && current != callingParent) {
        return searchNode(current->firstChild, nodeName, current);
    }

    // If no child, check for right sibling
    if (!current->firstChild && current->right) {
        return searchNode(current->right, nodeName, callingParent);
    }

    // If no child and no right sibling, go to parent
    if (!current->firstChild && !current->right) {
        return searchNode(current->parent, nodeName, current->parent);
    }

    // Special case: if current node is the calling parent and has a right sibling
    if (current == callingParent && current->right) {
        return searchNode(current->right, nodeName, current);
    }

    // Special case: if current node is the calling parent and has no right sibling
    if (current == callingParent && !current->right) {
        return searchNode(current->parent, nodeName, current->parent);
    }

    return NULL;
}

GraphNode* findParentNode(GraphNode* current) {
    if (!current) {
        return NULL;  // Base case for null input
    }

    if (current->parent != NULL) {
        return current->parent;  // Return the parent if it exists
    }
    else {
        // If the current node has no parent, move right until a node with a parent is found
        return findParentNode(current->right);
    }
}

GraphNode* findLeftSibling(GraphNode* node) {
    if (node == NULL || findParentNode(node) == NULL) {
        return NULL;  // No left sibling if node is NULL or if node has no parent
    }

    // Find the parent of the node
    GraphNode* parent = findParentNode(node);

    // Traverse the children of the parent to find the left sibling of 'node'
    GraphNode* currentNode = parent->firstChild;
    while (currentNode != NULL && currentNode->right != node) {
        currentNode = currentNode->right;
    }

    return currentNode;  // currentNode is the left sibling of 'node', NULL if 'node' is the first child
}

void addChild(GraphNode* parent, char* childName, GraphNode* graph) {
    // Search for an existing node with the same name
    GraphNode* existingNode = searchNode(graph, childName, NULL);

    GraphNode* newChild;
    newChild = (GraphNode*)malloc(sizeof(GraphNode));
    newChild->name = strdup(childName);  // Duplicate the name for the pointer node
    newChild->firstChild = NULL;
    newChild->right = NULL;
    newChild->parent = parent;
    newChild->commands = NULL;  // Assuming pointer nodes don't have their own commands
    //initializeGraphNode(newChild);
    GraphNode* findParentNode(GraphNode * current);
    GraphNode* findLeftSibling(GraphNode * node);

    if (existingNode) {
        // If an existing node is found, create a pointer node
        newChild->isPointerNode = 1;  // Mark this as a pointer node
        newChild->originalNode = existingNode;  // Point to the existing node
    }
    else {
        // If no existing node is found, create a regular node
        newChild->isPointerNode = 0;
        newChild->originalNode = NULL;
    }

    // Add the new child to the parent node
    if (parent->firstChild == NULL) {
        parent->firstChild = newChild;
    }
    else {
        GraphNode* rightmostChild = parent->firstChild;
        while (rightmostChild->right != NULL) {
            rightmostChild = rightmostChild->right;
        }
        rightmostChild->right = newChild;
        rightmostChild->parent = NULL;
    }
}

void disconnectAndAddToNewParent(GraphNode* mainNode, GraphNode* node, GraphNode* newParent) {
    if (node == NULL || newParent == NULL) {
        return;  // Do nothing if either node is NULL
    }

    // Find the left sibling of the node to be disconnected
    GraphNode* leftSibling = findLeftSibling(node);

    // Disconnect the node from its current position
    if (leftSibling != NULL && node->right) {
        leftSibling->right = node->right;  // Bypass the node being moved
    }
    else if (leftSibling != NULL && !node->right) {
        leftSibling->right = node->parent;  // Bypass the node being moved
    }
    else if (mainNode->firstChild == node) {
        mainNode->firstChild = node->right;  // If node is the first child of mainNode
    }

    // Add the node as the rightmost child of the new parent
    if (newParent->firstChild == NULL) {
        newParent->firstChild = node;  // If newParent has no children
    }
    else {
        GraphNode* rightmostChild = newParent->firstChild;
        while (rightmostChild->right != NULL) {
            rightmostChild = rightmostChild->right;
        }
        rightmostChild->right = node;  // Add node to the right of the rightmost child
        rightmostChild->parent = NULL;
    }
    node->parent = newParent;
    node->right = NULL;  // The moved node now has no right sibling
}

GraphNode* findOrCreateNode(GraphNode** graph, char* nodeName) {
    GraphNode* found = searchNode(*graph, nodeName, NULL);

    if (found) {
        return found;
    }

    // Node not found, create a new node and add it as a child to the graph root node
    addChild(*graph, nodeName, *graph); // Add new node to the graph
    return searchNode(*graph, nodeName, NULL);  // Return the newly added node
}

void printSubtreehelper(GraphNode* current, GraphNode* callingParent, GraphNode* rootNode, int startPrinting) {
    if (!current) {
        return;  // Base case
    }

    // Check for child, go to child if exists and not backtracking to calling parent
    if (current->firstChild && current != callingParent) {
        return printSubtreehelper(current->firstChild, current, rootNode, startPrinting);
    }
    else {
        startPrinting = 1;  // Start printing from this node onwards
    }
    if (startPrinting) {
        printCommands(current);
    }
    // If no child, check for right sibling
    if (current != rootNode && current->right) {
        return printSubtreehelper(current->right, callingParent, rootNode, startPrinting);
    }
    else if (current != rootNode && current->parent) {
        // Go to the parent's right sibling if not at the root node
        return printSubtreehelper(current->parent, current->parent, rootNode, startPrinting);
    }
}

void printCommands(GraphNode* node) {
    if (!node->isPointerNode && node->printed != 1) {
        node->printed = 1;

        initializeGraphNode(node);
        if (node->fileExists == 0 || hasNewerChild(node) == 1) {
            CommandNode* commands = node->commands;
            if (!commands) {
                fprintf(stderr, "File not found and not a target: %s\n", node->name);
                exitWithError();
            }
            while (commands) {
                executeShellCommand(commands->command);
                commands = commands->next;
            }
            initializeGraphNode(node);
        }
    }
    else {
        // For a pointer node, print the subtree of the node it points to
        printSubtreehelper(node->originalNode, NULL, node->originalNode, 1);
    }
}

void printSubtree(GraphNode* rootNode) {
    printSubtreehelper(rootNode, NULL, rootNode, 0);
}

int characterExists(char* str, char c) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == c) {
            return 1; // Character found
        }
    }
    return 0; // Character not found
}

void executeShellCommand(const char* command) {
    // Execute the command using system()
    printf("%s\n", command);
    int status = system(command);

    // Check the return value.
    if (status != 0) {
        fprintf(stderr, "Command failed to execute\n");
        exitWithError();
    }
    return;
}

int hasNewerChild(GraphNode* targetNode) {
    if (targetNode->firstChild == NULL) {
        return 0;
    }
    GraphNode* current = targetNode->firstChild;
    while (current != NULL) {
        initializeGraphNode(current);
        if (current->fileExists) {
            if (current->fileInfo.st_mtime > targetNode->fileInfo.st_mtime) {
                return 1; // Found a node with a newer date
            }
        }
        else {
            fprintf(stderr, "Impossible situation.\n");
            exitWithError();
        }
        current = current->right; // Move to the next right sibling
    }

    // No sibling node has a newer date
    return 0;
}