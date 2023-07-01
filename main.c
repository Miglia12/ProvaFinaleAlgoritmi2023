#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"

/*
* exit codes:
*  5 - invalid action
*  6 - hash table too big
*  7 - hash table too small
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

typedef enum size{
    INCREASE,
    DECREASE
}Size;
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
    unsigned int probingDistance;
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
 * Function: treeInsert
 * Description: inserts a new node in the tree
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - key: key of the new node
 * Returns: void
 */
void treeInsert(pNode *root, unsigned  int key);
/*
 * Function: removeNode
 * Description: removes a node from the tree
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - key: of the node to remove
 * Returns: 0 if the node was not found, 1 otherwise
 */
int removeNode(pNode* root, unsigned int key);
/*
 * Function: fixDelete
 * Description: checks if any rotation is needed after removing a node
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - node: node to check if any rotation is needed
 *   - parent: parent of the node
 * Returns: void
 */
void freeTree(pNode root);
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
 *   - hashMapSize: size of the hash table, it will be automatically converted to a prime number
 * Returns: the hash value of the key
 */
unsigned int hash(unsigned int key, unsigned int hashMapSize);
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
 *   - map: pointer to the hash
 *   - size: choose if you want to increase or decrease the size of the hash table
 * Returns: void
 */
void resize(pHashMap map, Size size);
/*
 * Function: insertStation
 * Description: inserts a new station in the hash table. If the hash table is too big, it resizes it. It is using robin hood hashing.
 * Parameters:
 *   - map: pointer to the hash table
 *   - stationID: id of the station to insert
 * Returns: pointer to the new station. NULL if the station already exists
 */
Station* insertStation(pHashMap map, unsigned int stationID);
/*
 * Function: searchStation
 * Description: searches for a station in the hash table
 * Parameters:
 *   - map: pointer to the hash table
 *   - stationID: id of the station to search
 * Returns: pointer to the station if found, NULL otherwise
 */
Station* searchStation(pHashMap map, unsigned int stationID);
/*
 * Function: removeStation
 * Description: removes a station from the hash table. it uses backward shifting to avoid clustering.
 * Parameters:
 *   - map: pointer to the hash table
 *   - stationID: id of the station to remove
 * Returns: void
 */
void removeStation(pHashMap map, unsigned int stationID);

/* Global variables */
//array of primes to use in the hash table and the hash function
unsigned int primes[] ={13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191,
               16381, 32749, 65521, 131071, 262139, 524287, 1048573,
               2097143, 4194301, 8388593, 16777213, 33554393, 67108859,
               134217689, 268435399, 536870909, 1073741827, 2147483647};

int main() {
    Action action; //action to perform
    unsigned int carID; //number read from input
    unsigned int stationID; //number read from input
    pHashMap map = createHashMap(); //create the hash table
    Station *station; //pointer to a station

    while ((action = readAction()) != ENDINPUT) {
        switch (action) {
            case ADDSTATION: //the input said to add a station
                readInt(&stationID); //read the station id
                station = insertStation(map, stationID); //insert the station in the hash table
                if(station == NULL){ //if the station was already in the hash table
                    printf("non aggiunta\n");
                }
                else{ //if the station was not in the hash table
                    while (readInt(&carID) != 0 && carID != 0) { //read the cars in the station until the last one is read
                        treeInsert(&(station->cars), carID); //insert the car in the station
                    }
                    if(carID != 0)
                        treeInsert(&(station->cars), carID); //insert the last car in the station
                    printf("aggiunta\n");
                }
                break;

            case RMVSTATION:
                readInt(&stationID); //read the station id
                station = searchStation(map, stationID); //search for the station in the hash table
                if(station == NULL){ //if the station is not in the hash table
                    printf("non demolita\n");
                }
                else{ //if the station is in the hash table
                    removeStation(map, stationID); //remove the station from the hash table
                    printf("demolita\n");
                }
                break;
            case ADDCAR:
                readInt(&stationID); //read the station id
                station = searchStation(map, stationID); //search for the station in the hash table
                if(station == NULL) {
                    printf("non aggiunta\n");
                } else {
                    readInt(&carID); //read the car id
                    treeInsert(&(station->cars), carID); //insert the car in the station
                    printf("aggiunta\n");
                }
                break;
            case RMVCAR:
                readInt(&stationID); //read the station id
                station = searchStation(map, stationID); //search for the station in the hash table
                if(station == NULL) {
                    readInt(&carID);
                    printf("non rottamata\n");
                } else {
                    readInt(&carID); //read the car id
                    if(removeNode(&(station->cars), carID) == 0) { //the car was not in the station
                        printf("non rottomata\n");
                    } else { //the car was in the station
                        printf("rottomata\n");
                    }
                }
                break;
            case PLANROUTE:
                printf("piano\n");
                readInt(&stationID); //read the station id
                readInt(&stationID);
                break;
            default:
                printf("invalid action\n");
                exit(5);
        }
    }
    return 0;
}

void removeStation(pHashMap map, unsigned int stationID) {
    int backShiftingPerformed = 0; // flag to indicate if back shifting was performed
    unsigned int hashValue = hash(stationID, map->hashMapSize); // calculate the hash value
    unsigned int nextHashValue = (hashValue + 1) % sizeToPrime(map->hashMapSize); // hash value of the next station
    while(map->stations[hashValue].stationID != 0) {
        if(map->stations[hashValue].stationID == stationID) {
            // Found the station with matching stationID, remove it
            map->stations[hashValue].stationID = 0;
            map->stations[hashValue].probingDistance = 0;
            freeTree(map->stations[hashValue].cars);
            map->stations[hashValue].cars = NULL;
            //TODO: handle adjacency list
            map->numStations--;
            //shift the stations in the hash table to avoid clustering
            while(map->stations[nextHashValue].stationID != 0 && map->stations[nextHashValue].probingDistance > 0) {
                map->stations[hashValue].stationID = map->stations[nextHashValue].stationID;
                map->stations[nextHashValue].probingDistance --;
                hashValue = nextHashValue;
                nextHashValue = (hashValue + 1) % sizeToPrime(map->hashMapSize);
                backShiftingPerformed = 1;
            }
            if(backShiftingPerformed) { // if back shifting was performed, set the last station to 0
                map->stations[hashValue].stationID = 0;
                map->stations[hashValue].probingDistance = 0;
                freeTree(map->stations[hashValue].cars);
                map->stations[hashValue].cars = NULL;
            }
            if(map->numStations < sizeToPrime(map->hashMapSize) / 4 && sizeToPrime(map->hashMapSize)/4 > primes[0]) { // if the hash table is too big, resize it
                resize(map, DECREASE);
            }
            return;
        }
        hashValue = (hashValue + 1) % sizeToPrime(map->hashMapSize);
    }
}

Station* searchStation(pHashMap map, unsigned int stationID) {
    unsigned int hashValue = hash(stationID, map->hashMapSize); // calculate the hash value
    unsigned int probe = 0; // probe counter

    while (map->stations[hashValue].stationID != 0 && map->stations[hashValue].probingDistance >= probe) {
        if (map->stations[hashValue].stationID == stationID) {
            // Found the station with matching stationID, return the pointer to the station
            return &(map->stations[hashValue]);
        }

        // Linear probing: move to the next position in the hash table
        hashValue = (hashValue + 1) % sizeToPrime(map->hashMapSize);
        probe++;
    }

    // Station not found in the hash map
    return NULL;
}

Station* insertStation(pHashMap map, unsigned int stationID) {
    unsigned int probe = 0; //probe counter

    if(map->numStations >= sizeToPrime(map->hashMapSize)* 0.75)//check if the hash table needs to be resized
        resize(map, INCREASE);
    unsigned int hashValue = hash(stationID, map->hashMapSize);//get the hash value

    while(map->stations[hashValue].stationID != 0 && map->stations[hashValue].probingDistance <= probe){ //check if the position is empty or if the probing distance is less than the current probe
        if(stationID == map->stations[hashValue].stationID) {//check if the station is already in the hash table
            return NULL;
        }

        if(map->stations[hashValue].probingDistance < probe){//check if the probing distance is less than the current probe and swap the stations
            // Swapping stations since current station has smaller probing distance
            unsigned int tempID = map->stations[hashValue].stationID;
            unsigned int tempProbe = map->stations[hashValue].probingDistance;
            map->stations[hashValue].stationID = stationID;
            map->stations[hashValue].probingDistance = probe;
            stationID = tempID;
            probe = tempProbe;
        }
        // Linear probing: move to the next position in the hash table
        hashValue = (hashValue + 1) % sizeToPrime(map->hashMapSize);
        probe++;
    }
    //found an empty position insert the station
    map->stations[hashValue].stationID = stationID;
    map->stations[hashValue].probingDistance = probe;
    map->stations[hashValue].cars = NULL;
    map->numStations++;

    return &(map->stations[hashValue]);
}

void resize(pHashMap map, Size size) {
    int i; //counter
    unsigned int probingDistance = 0; //probing distance

    if(size == INCREASE && map->hashMapSize >= MAX_SIZE_PRIMES)//check if the hash table is already at max size
        exit(6);
    if(size == DECREASE && map->hashMapSize == 0)//check if the hash table is already at min size
        exit(7);
    unsigned int oldSize = sizeToPrime(map->hashMapSize);//get the old size

    if(size == INCREASE) //increase the size
    map->hashMapSize++;
    else if(size == DECREASE) //decrease the size
    map->hashMapSize--;

    Station *oldStations = map->stations;   //save the old stations
    map->stations = calloc(sizeToPrime(map->hashMapSize), sizeof(Station));//create the new hashmap

    for(i = 0; i<oldSize; i++){//rehash the old stations
        if(oldStations[i].stationID != 0){
            probingDistance = 0; //reset the probing distance

            unsigned int hashValue = hash(oldStations[i].stationID, map->hashMapSize);//get the new hash value
            //robin hood hashing
            while(map->stations[hashValue].stationID != 0 && map->stations[hashValue].probingDistance <= probingDistance) {
                if(map->stations[hashValue].probingDistance < probingDistance){//check if the probing distance is less than the current probe and swap the stations
                    // Swapping stations since current station has smaller probing distance
                    unsigned int tempID = map->stations[hashValue].stationID;
                    unsigned int tempProbe = map->stations[hashValue].probingDistance;

                    map->stations[hashValue].stationID = oldStations[i].stationID; //copy the station
                    map->stations[hashValue].cars = oldStations[i].cars; //copy the cars
                    map->stations[hashValue].probingDistance = probingDistance; //copy the probing distance
                    oldStations[i].stationID = tempID;
                    probingDistance = tempProbe;
                }
                // Linear probing: move to the next position in the hash table
                hashValue = (hashValue + 1) % sizeToPrime(map->hashMapSize);
                probingDistance++;
            }

            //found an empty position insert the station
            map->stations[hashValue].stationID = oldStations[i].stationID;
            map->stations[hashValue].probingDistance = probingDistance;
            map->stations[hashValue].cars = oldStations[i].cars;
            map->numStations++; //increment the number of stations
            //TODO: handle the adjacency list

        }
    }

    free(oldStations); //free the old stations
}

pHashMap createHashMap() {
    pHashMap map = (pHashMap) malloc(sizeof(HashMap));
    map->stations = calloc(sizeToPrime(0), sizeof(Station));
    map->hashMapSize = 0;
    map->numStations = 0;
    return map;
}

unsigned int hash(unsigned int key, unsigned int hashMapSize) {
    const float A = (float) 0.6180339887; // (sqrt(5) - 1) / 2

    float product = (float) key * A;
    float fractionalPart = product - (unsigned int)product;

    unsigned int hashValue = (unsigned int)(sizeToPrime(hashMapSize) * fractionalPart) % sizeToPrime(hashMapSize);

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

void freeTree(pNode root) {
    if (root == NULL) {
        return;
    }

    // Free the left and right subtrees
    freeTree(root->left);
    freeTree(root->right);

    // Free the current node
    free(root);
}

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

int removeNode(pNode* root, unsigned int key) {
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
        return 0;

    if(node->numCars > 1){// If the node has more than one car
        node->numCars--;
        return 1;
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

    return 1;
}

void treeInsert(pNode *root, unsigned int key) {// Function to treeInsert a node into the Red-Black Tree
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
    else if(buffer[0] == 'd') { //check if the action is remove station or remove car
            return RMVSTATION;
    } else if(buffer[0] == 'r') {
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

