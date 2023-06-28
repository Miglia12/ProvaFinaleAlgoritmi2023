#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"

/*
* exit codes:
*  5 - invalid action
*  6 - hash table too big
*/

#define BUFFER_SIZE 250 //size of the buffer to read from input
#define MAX_SIZE_PRIMES 28 //number of primes in the array

typedef enum action{
    ADDSTATION,
    RMVSTATION,
    ADDCAR,
    RMVCAR,
    PLANROUTE,
    ENDINPUT
}Action;

/* Data Structures */

/*
 * Red-Black Tree
 * Description: implementation of red-black tree
 * ENUMS:
 *   - Color: defines the color of the node.
 *   - Node: defines the node of the tree. It has a key, color, parent, left child and right child.
 */
typedef enum color {
    RED,
    BLACK
}Color;

typedef struct node* pNode;

typedef struct node {
    unsigned int key;
    unsigned int numCars;
    Color color;
    pNode parent;
    pNode left;
    pNode right;
}Node;
/*
 * Hash Table of stations
 * Description: implementation of hash table
 * variables:
 *   - stationID: id of the station
 *   - cars: pointer to the red-black tree of cars
 *   - adjacencyList: pointer to the adjacency list of stations
 */
typedef struct station {
    unsigned int stationID;
    pNode cars;
    int probingDistance;
    //TODO: add adjacency list of stations
}Station;

typedef struct hashMap {
    Station *stations;
    int hashMapSize;
    int numStations;
}HashMap;

typedef struct hashMap* pHashMap;

/* Function Declarations */

/*
 * Function: readInt
 * Description: reads int from input stream
 * Parameters:
 *   - number: pointer to int to store the read number
 * Returns: 0 if EOF, 1 otherwise (remember that when it returns 0 number is updated)
 */
int readInt(unsigned int *number);
/*
 * Function: readAction
 * Description: reads from input stream and returns the action
 * Returns: the action read
 */
Action readAction();
/*
 * Function: createNode
 * Description: generates a new node with the given key for the red-black tree
 * Parameters:
 *   - key: key of the new node
 * Returns: pointer to the new node
 */
pNode createNode(unsigned int key);
/*
 * Function: rightRotate
 * Description: performs a right rotation on the given node
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - node: node to perform the rotation
 * Returns: void
 */
void rightRotate(pNode *root, pNode node);
/*
 * Function: leftRotate
 * Description: performs a left rotation on the given node
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - node: node to perform the rotation
 * Returns: void
 */
void leftRotate(pNode *root, pNode node);
/*
 * Function: insertFix
 * Description: checks if any rotation is needed after inserting a new node
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - node: node to check if any rotation is needed
 * Returns: void
 */
void insertFix(pNode *root, pNode node);
/*
 * Function: insert
 * Description: inserts a new node in the tree
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - key: key of the new node
 * Returns: void
 */
void insert(pNode *root, unsigned  int key);
/*
 * Function: removeNode
 * Description: removes a node from the tree
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - key: of the node to remove
 * Returns: void
 */
void removeNode(pNode* root, unsigned int key);
/*
 * Function: fixDelete
 * Description: checks if any rotation is needed after removing a node
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - node: node to check if any rotation is needed
 *   - parent: parent of the node
 * Returns: void
 */
void fixDelete(pNode* root, pNode node, pNode parent);
/*
 * Function: findMaxTree
 * Description: finds the maximum key in the tree
 * Parameters:
 *   - root: pointer to the root of the tree
 * Returns: maximum key in the tree
 */
unsigned int findMaxTree(pNode root);
/*
 * Function: sizeToPrime
 * Description: returns the corresponding size of the hash table given the size number
 * Parameters:
 *   - size: size of the hash table
 * Returns: the prime number corresponding to the size
 */
unsigned int sizeToPrime(unsigned int size);
/*
 * Function: hash
 * Description: multiplicative hashing function based on the size of the hash table
 * Parameters:
 *   - key: key to hash
 *   - hashMap: pointer to the hash table
 * Returns: the hash value of the key
 */
unsigned int hash(unsigned int key, pHashMap hashMap);
/*
 * Function: createHashMap
 * Description: creates a new hash table
 * Returns: pointer to the new hash table
 */
pHashMap createHashMap();
/*
 * Function: resize
 * Description: resizes the hash table to the next prime number
 * Parameters:
 *   - map: pointer to the hash table
 * Returns: void
 */
void resize(pHashMap map);

/* Global variables */
//array of primes to use in the hash table and the hash function
unsigned int primes[] ={13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191,
               16381, 32749, 65521, 131071, 262139, 524287, 1048573,
               2097143, 4194301, 8388593, 16777213, 33554393, 67108859,
               134217689, 268435399, 536870909, 1073741827, 2147483647};

int main() {
    Action action; //action to perform
    unsigned int number; //number read from input
    pNode root = NULL;
    pHashMap map = createHashMap();

    while ((action = readAction()) != ENDINPUT) {
        switch (action) {
            case ADDSTATION:
                printf("add station\n");
                readInt(&number);
                while (readInt(&number) != 0) {
                    printf("number: %d\n", number);
                }
                printf("number: %d\n", number);
                break;

            case RMVSTATION:
                printf("remove station\n");
                while (readInt(&number) != 0) {
                    printf("number: %d\n", number);
                }
                printf("number: %d\n", number);
                break;
            case ADDCAR:
                printf("add car\n");
                while (readInt(&number) != 0) {
                    printf("number: %d\n", number);
                }
                printf("number: %d\n", number);
                break;
            case RMVCAR:
                printf("remove car\n");
                while (readInt(&number) != 0) {
                    printf("number: %d\n", number);
                }
                printf("number: %d\n", number);
                break;
            case PLANROUTE:
                printf("plan route\n");
                while (readInt(&number) != 0) {
                    printf("number: %d\n", number);
                }
                printf("number: %d\n", number);
                break;
            default:
                printf("invalid action\n");
                exit(5);
        }
    }
    return 0;
}

void resize(pHashMap map) {
    if(map->hashMapSize > MAX_SIZE_PRIMES){
        exit(6);
    }
    unsigned int oldSize = sizeToPrime(map->hashMapSize);
    map->hashMapSize = map->hashMapSize++;
    Station *oldStations = map->stations;
    map->stations = calloc(sizeToPrime(map->hashMapSize), sizeof(Station));
    //TODO: rehash the stations
}

pHashMap createHashMap() {
    pHashMap map = (pHashMap) malloc(sizeof(HashMap));
    map->stations = calloc(sizeToPrime(0), sizeof(Station));
    map->hashMapSize = 0;
    map->numStations = 0;
    return map;
}

unsigned int hash(unsigned int key, pHashMap hashMap) {
    // Get the prime number
    unsigned int prime = sizeToPrime(hashMap->hashMapSize);

    // Initialize the hash value
    unsigned int hashValue = 0;

    // Perform the hash calculation
    hashValue = prime * hashValue + key;

    return hashValue;
}

unsigned int sizeToPrime(unsigned int size) {//returns the prime number that is in primes[]
    return primes[size];
}

unsigned int findMaxTree(pNode root) {
    pNode current = root;

    if (root == NULL) {
        return 0;
    }
    while (current->right != NULL) {
        current = current->right;
    }
    return current->key;
}
// Function to fix the Red-Black Tree after deletion
void fixDelete(pNode* root, pNode node, pNode parent) {
    pNode sibling;

    while ((node == NULL || node->color == BLACK) && node != *root) {
        if (node == parent->left) {
            sibling = parent->right;

            if (sibling->color == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                leftRotate(root, parent);
                sibling = parent->right;
            }

            if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                (sibling->right == NULL || sibling->right->color == BLACK)) {
                sibling->color = RED;
                node = parent;
                parent = node->parent;
            } else {
                if (sibling->right == NULL || sibling->right->color == BLACK) {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rightRotate(root, sibling);
                    sibling = parent->right;
                }

                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->right->color = BLACK;
                leftRotate(root, parent);
                node = *root;
                break;
            }
        } else {
            sibling = parent->left;

            if (sibling->color == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rightRotate(root, parent);
                sibling = parent->left;
            }

            if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                (sibling->right == NULL || sibling->right->color == BLACK)) {
                sibling->color = RED;
                node = parent;
                parent = node->parent;
            } else {
                if (sibling->left == NULL || sibling->left->color == BLACK) {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    leftRotate(root, sibling);
                    sibling = parent->left;
                }

                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->left->color = BLACK;
                rightRotate(root, parent);
                node = *root;
                break;
            }
        }
    }

    if (node != NULL)
        node->color = BLACK;
}

// Function to remove a node from the Red-Black Tree
void removeNode(pNode* root, unsigned int key) {
    pNode node = *root;
    pNode parent = NULL;
    pNode temp;
    pNode successor;

    while (node != NULL) {// Search for the node with the given key
        if (key == node->key)
            break;

        parent = node;

        if (key < node->key)
            node = node->left;
        else
            node = node->right;
    }

    if (node == NULL)// If the node with the given key is not found, return
        return;

    if(node->numCars > 1){// If the node has more than one car
        node->numCars--;
        return;
    }

    if (node->left == NULL || node->right == NULL)// If the node has only one child
        successor = node;
    else {// If the node has two children
        successor = node->right;

        while (successor->left != NULL)
            successor = successor->left;
    }

    if (successor->left != NULL)// If the successor has a left child
        temp = successor->left;
    else // If the successor has no left child
        temp = successor->right;

    if (temp != NULL)// Set the parent of the successor's child
        temp->parent = successor->parent;

    if (successor->parent == NULL)// If the successor is the root
        *root = temp;
    else if (successor == successor->parent->left)// If the successor is a left child
        successor->parent->left = temp;
    else// If the successor is a right child
        successor->parent->right = temp;

    if (successor != node)
        node->key = successor->key;

    if (successor->color == BLACK)// If the successor is black, fix the tree
        fixDelete(root, temp, successor->parent);

    free(successor);
}

void insert(pNode *root, unsigned int key) {// Function to insert a node into the Red-Black Tree
    pNode newNode = createNode(key);

    pNode y = NULL;
    pNode x = *root;

    while (x != NULL) {// Search for the node with the given key
        y = x;
        if(key == x->key) {// If the node already exists, increment the number of cars
            x->numCars++;
            return;
        } else if (key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    newNode->parent = y;

    if (y == NULL) {// If the tree is empty
        *root = newNode;
    } else if (key < y->key) {// If the node is a left child
        y->left = newNode;
    } else {// If the node is a right child
        y->right = newNode;
    }

    insertFix(root, newNode);// Fix the tree
}

void insertFix(pNode *root, pNode node) {// Function to fix the Red-Black Tree after insertion
    while (node != *root && node->parent->color == RED) {// While the parent of the node is red
        if (node->parent == node->parent->parent->left) {// If the parent of the node is a left child
            pNode uncle = node->parent->parent->right;

            if (uncle != NULL && uncle->color == RED) {// If the uncle of the node is red
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {// If the uncle of the node is black
                if (node == node->parent->right) {
                    node = node->parent;
                    leftRotate(root, node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rightRotate(root, node->parent->parent); // Right rotate the tree
            }
        } else {// If the parent of the node is a right child
            pNode uncle = node->parent->parent->left;

            if (uncle != NULL && uncle->color == RED) {// If the uncle of the node is red
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {// If the uncle of the node is black
                if (node == node->parent->left) {
                    node = node->parent;
                    rightRotate(root, node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                leftRotate(root, node->parent->parent);// Left rotate the tree
            }
        }
    }

    (*root)->color = BLACK;
}

void leftRotate(pNode *root, pNode node) {// Function to left rotate the Red-Black Tree

    pNode rightChild = node->right;
    node->right = rightChild->left;

    if (rightChild->left != NULL) {
        rightChild->left->parent = node;
    }

    rightChild->parent = node->parent;

    if (node->parent == NULL) {
        *root = rightChild;
    } else if (node == node->parent->left) {
        node->parent->left = rightChild;
    } else {
        node->parent->right = rightChild;
    }

    rightChild->left = node;
    node->parent = rightChild;
}

void rightRotate(pNode* root, pNode node) {// Function to right rotate the Red-Black Tree

    pNode leftChild = node->left;
    node->left = leftChild->right;

    if (leftChild->right != NULL) {
        leftChild->right->parent = node;
    }

    leftChild->parent = node->parent;

    if (node->parent == NULL) {
        *root = leftChild;
    } else if (node == node->parent->right) {
        node->parent->right = leftChild;
    } else {
        node->parent->left = leftChild;
    }

    leftChild->right = node;
    node->parent = leftChild;
}

pNode createNode(unsigned int key) {

    pNode newNode = (pNode) malloc(sizeof(Node));
    newNode->key = key;
    newNode->numCars = 1;
    newNode->parent = newNode->left = newNode->right = NULL;
    newNode->color = RED;

    return  newNode;
}

Action readAction() {
    char buffer[BUFFER_SIZE];
    int ch;
    int i = 0;

    //read string from input stream
    while(isspace(ch = getchar()) == 0 && ch !='\n' && ch != EOF) {
        buffer[i] = (char) ch;
        i++;
    }

    if(ch == '\n' || ch == EOF) //if ch == '\n' or ch == EOF, then the input stream is empty
        return ENDINPUT;

    //check which action to perform
    if(buffer[0] == 'p') //check if the action is plan route
        return PLANROUTE;

    else if(buffer[0] == 'a') { //check if the action is add or remove

        if(buffer[9] == 's') //check if the action is add station or add station
            return ADDSTATION;
        else
            return ADDCAR;
    }
    else if(buffer[0] == 'd') {

        if(buffer[10] == 's') //check if the action is remove station or remove car
            return RMVSTATION;
        else
            return RMVCAR;
    }
    else{ //if the action is not valid, exit the program
        exit(5);
    }
}

int readInt(unsigned int *number) {
    char buffer[BUFFER_SIZE];
    unsigned int value = 0;
    int ch;
    int i = 0;
    int j;
    int p = 1;

    while(isspace(ch = getchar()) == 0 && ch !='\n' && ch != EOF) {
        buffer[i] = (char) ch;
        i++;
    }

    //add null terminator to the end of the string
    buffer[i] = '\0';

    for(j = (i - 1); j >= 0; j--) { //convert string to int
        value = value + (buffer[j] - '0') * p;
        p = p * 10;
    }

    *number = value;

    if(ch == '\n' || ch == EOF) //if ch == '\n' or ch == EOF, then the input stream is empty
        return 0;
    else //there are some other number to check
        return 1;
}

