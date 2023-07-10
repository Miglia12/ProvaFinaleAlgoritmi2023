#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"

/*
* exit codes:
*  5 - invalid action
*  6 - heap is full
*/

#define BUFFER_SIZE 250 //size of the buffer to read from input
#define MAX_SIZE_CARS 513 //maximum number of cars in a station

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
 * Description: maxHeap struct to store the cars in the station
 * Values:
 *   - numOfCars: number of cars in the station
 *   - array: array to store the cars
 */
typedef struct maxHeap {
    int numOfCars;
    unsigned int array[MAX_SIZE_CARS];
} MaxHeap;
/*
 * Description: pointer to a MaxHeap
 */
typedef struct maxHeap* pMaxHeap;
/*
 * Description: enum to store the color of a node
 * Values:
 *   - RED: the node is red
 *   - BLACK: the node is black
 */
typedef enum color {
    RED,
    BLACK
}Color;
/*
 * Description: pointer to a Station
 */
typedef struct station* pStation;
/*
 * Description: struct to store a station
 * Values:
 *   - stationID: ID of the station
 *   - cars: pointer to the maxHeap of the station that stores the cars
 *   - color: color of the node
 *   - parent: pointer to the parent of the node
 *   - left: pointer to the left child of the node
 *   - right: pointer to the right child of the node
 */
typedef struct station {
    unsigned int stationID;
    pMaxHeap cars;
    Color color;
    pStation parent;
    pStation left;
    pStation right;
}Station;

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
 * Function: merge
 * Description: Merge two sorted sub arrays into one sorted array
 * Parameters:
 *   - arr: array to merge
 *   - left: left sub array
 *   - leftSize: size of the left sub array
 *   - right: right sub array
 *   - rightSize: size of the right sub array
 * Returns: void
 */
void merge(unsigned int arr[], const unsigned int left[], unsigned int leftSize, const unsigned int right[], unsigned int rightSize);
/*
 * Function: mergeSort
 * Description: Sorts an array using merge sort algorithm
 * Parameters:
 *   - arr: array to sort
 *   - size: size of the array
 * Returns: void
 */
void mergeSort(unsigned int arr[], unsigned int size);
/*
 * Function: binarySearch
 * Description: searches for a target in a sorted array using binary search algorithm
 * Parameters:
 *   - arr: array to search
 *   - size: size of the array
 *   - target: target to search
 * Returns: index of the target if found, -1 otherwise
 */
int binarySearch(const unsigned int arr[], int size, unsigned int target);
/*
 * Function: createMaxHeap
 * Description: creates a new maxHeap
 * Parameters: void
 * Returns: pointer to the new maxHeap
 */
pMaxHeap createMaxHeap();
/*
 * Function: addCar
 * Description: adds a new car to the maxHeap
 * Parameters:
 *   - maxHeap: pointer to the maxHeap
 *   - carID: ID of the new car
 * Returns: void
 */
void addCar(pMaxHeap maxHeap, unsigned int carID);
/*
 * Function: removeCar
 * Description: removes a car from the maxHeap
 * Parameters:
 *   - maxHeap: pointer to the maxHeap
 *   - element: id of the car to remove
 * Returns: 1 if the car was removed, 0 otherwise
 */
int removeCar(MaxHeap* maxHeap, unsigned int carID);
/*
 * Function: restoreHeapProperty
 * Description: restores the heap property of the maxHeap
 * Parameters:
 *   - maxHeap: pointer to the maxHeap
 *   - index: index to start restoring the heap property
 * Returns: void
 */
void restoreHeapProperty(MaxHeap* maxHeap, int idx);
/*
 * Function: createNode
 * Description: generates a new station with the given stationID for the red-black tree
 * Parameters:
 *   - stationID: stationID of the new station
 * Returns: pointer to the new station
 */
pStation createNode(unsigned int stationID);
/*
 * Function: rightRotate
 * Description: performs a right rotation on the given station
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - station: station to perform the rotation
 * Returns: void
 */
void rightRotate(pStation *root, pStation node);
/*
 * Function: leftRotate
 * Description: performs a left rotation on the given station
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - station: station to perform the rotation
 * Returns: void
 */
void leftRotate(pStation *root, pStation node);
/*
 * Function: insertFix
 * Description: checks if any rotation is needed after inserting a new station
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - station: station to check if any rotation is needed
 * Returns: void
 */
void insertFix(pStation *root, pStation node);
/*
 * Function: addStation
 * Description: inserts a new station in the tree
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - stationID: stationID of the new station
 * Returns: pointer to the new station if it was added, NULL otherwise
 */
pStation addStation(pStation *root, unsigned  int stationID);
/*
 * Function: removeStation
 * Description: removes a station from the tree
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - stationID: of the station to remove
 * Returns: 1 if the station was removed, 0 otherwise
 */
int removeStation(pStation* root, unsigned int stationID);
/*
 * Function: fixDelete
 * Description: checks if any rotation is needed after removing a station
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - station: station to check if any rotation is needed
 *   - parent: parent of the station
 * Returns: void
 */
void fixDelete(pStation* root, pStation node, pStation parent);
/*
 * Function: searchStation
 * Description: searches for a station in the tree
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - station: station to search
 * Returns: pointer to the station if found, NULL otherwise
 */
pStation searchStation(pStation* root, unsigned int stationID);

/* Global variables */

int main() {
    Action action; //action to perform
    unsigned int carID; //number read from input
    unsigned int stationID; //number read from input
    Station *station; //pointer to a station
    pStation root = NULL; //root of the red-black tree

    while ((action = readAction()) != ENDINPUT) {
        switch (action) {
            case ADDSTATION: //the input said to add a station
                readInt(&stationID); //read the station id
                station = addStation(&root, stationID); //insert the station in the tree
                if(station == NULL){ //if the station was already in the tree
                    while (readInt(&carID) != 0 && carID != 0); //WARNING: this is a workaround I'm not sure if I should add the cars or not
                    printf("non aggiunta\n");
                }
                else{ //if the station was not in the tree
                    while (readInt(&carID) != 0 && carID != 0) { //read the cars in the station until the last one is read
                        addCar(station->cars, carID); //insert the car in the station
                    }
                    if(carID != 0)
                        addCar(station->cars, carID);//insert the last car in the station
                    printf("aggiunta\n");
                }
                break;

            case RMVSTATION:
                readInt(&stationID);    //read the station id
                if(removeStation(&root, stationID) == 0){ //if the station is not in the tree
                    printf("non demolita\n");
                }
                else{ //if the station was removed
                    printf("demolita\n");
                }
                break;
            case ADDCAR:
                readInt(&stationID); //read the station id
                station = searchStation(&root, stationID); //search for the station in the tree
                if(station == NULL) {
                    readInt(&carID);
                    printf("non aggiunta\n");
                } else {
                    readInt(&carID); //read the car id
                    addCar(station->cars, carID);//insert the car in the station
                    printf("aggiunta\n");
                }
                break;
            case RMVCAR:
                readInt(&stationID); //read the station id
                station = searchStation(&root, stationID);  //search for the station in the tree
                if(station == NULL) {
                    readInt(&carID);
                    printf("non rottamata\n");
                } else {
                    readInt(&carID); //read the car id
                    if(removeCar(station->cars, carID)) { //the car was not in the station
                        printf("non rottamata\n");
                    } else { //the car was in the station
                        printf("rottamata\n");
                    }
                }
                break;
            case PLANROUTE:
                printf("nessun percorso\n");
                readInt(&stationID); //read the station id
                readInt(&carID); //reads the second station id
                break;
            default:
                printf("invalid action\n");
                exit(5);
        }
    }
    return 0;
}

pStation searchStation(pStation* root, unsigned int stationID) {
    pStation node = *root;

    while (node != NULL) {// Search for the station with the given stationID
        if (stationID == node->stationID)
            break;

        if (stationID < node->stationID)
            node = node->left;
        else
            node = node->right;
    }

    return node;
}

void fixDelete(pStation* root, pStation node, pStation parent) {
    pStation sibling;

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

int removeStation(pStation* root, unsigned int stationID) {
    pStation node = *root;
    pStation temp;
    pStation successor;

    while (node != NULL) {// Search for the station with the given stationID
        if (stationID == node->stationID)
            break;

        if (stationID < node->stationID)
            node = node->left;
        else
            node = node->right;
    }

    if (node == NULL)// If the station with the given stationID is not found, return
        return 0;

    if (node->left == NULL || node->right == NULL)// If the station has only one child
        successor = node;
    else {// If the station has two children
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
        node->stationID = successor->stationID;

    if (successor->color == BLACK)// If the successor is black, fix the tree
        fixDelete(root, temp, successor->parent);

    free(successor);

    return 1;
}

pStation addStation(pStation* root, unsigned int stationID) {// Function to addStation a station into the Red-Black Tree
    pStation newNode = createNode(stationID);

    pStation y = NULL;
    pStation x = *root;

    while (x != NULL) {// Search for the station with the given stationID
        y = x;
        if(stationID == x->stationID) {//WARNING: This is not specified in the assignment you may need to add the cars to the station
            return NULL;
        } else if (stationID < x->stationID) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    newNode->parent = y;

    if (y == NULL) {// If the tree is empty
        *root = newNode;
    } else if (stationID < y->stationID) {// If the station is a left child
        y->left = newNode;
    } else {// If the station is a right child
        y->right = newNode;
    }

    insertFix(root, newNode);// Fix the tree
    return newNode;
}

void insertFix(pStation* root, pStation node) {// Function to fix the Red-Black Tree after insertion
    while (node != *root && node->parent->color == RED) {// While the parent of the station is red
        if (node->parent == node->parent->parent->left) {// If the parent of the station is a left child
            pStation uncle = node->parent->parent->right;

            if (uncle != NULL && uncle->color == RED) {// If the uncle of the station is red
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {// If the uncle of the station is black
                if (node == node->parent->right) {
                    node = node->parent;
                    leftRotate(root, node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rightRotate(root, node->parent->parent); // Right rotate the tree
            }
        } else {// If the parent of the station is a right child
            pStation uncle = node->parent->parent->left;

            if (uncle != NULL && uncle->color == RED) {// If the uncle of the station is red
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {// If the uncle of the station is black
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

void leftRotate(pStation* root, pStation node) {// Function to left rotate the Red-Black Tree

    pStation rightChild = node->right;
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

void rightRotate(pStation* root, pStation node) {// Function to right rotate the Red-Black Tree

    pStation leftChild = node->left;
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

pStation createNode(unsigned int stationID) {

    pStation newNode = (pStation) malloc(sizeof(Station));
    newNode->stationID = stationID;
    newNode->cars = createMaxHeap();
    newNode->parent = newNode->left = newNode->right = NULL;
    newNode->color = RED;

    return  newNode;
}

int removeCar(MaxHeap* maxHeap, unsigned int carID) {
    // Check if the heap is empty
    if (maxHeap->numOfCars == 0) {
        return 0;
    }

    // Find the element in the heap
    int i;
    for (i = 0; i < maxHeap->numOfCars; i++) {
        if (maxHeap->array[i] == carID) {
            break;
        }
    }

    // If the element is not found, return 0
    if (i == maxHeap->numOfCars) {
        return 0;
    }

    // Swap the found element with the last element in the heap
    maxHeap->array[i] = maxHeap->array[maxHeap->numOfCars - 1];

    // Decrease the size of the heap
    maxHeap->numOfCars--;

    // Heapify the root element
    restoreHeapProperty(maxHeap, i);

    // Return 1 indicating success
    return 1;
}

void restoreHeapProperty(MaxHeap* maxHeap, int idx) {
    int largest;

    do {
        int left = (idx << 1) + 1; // left = 2*idx + 1
        int right = (idx + 1) << 1; // right = 2*idx + 2

        largest = idx;

        // If left child is larger than root
        if (left < maxHeap->numOfCars && maxHeap->array[left] > maxHeap->array[largest])
            largest = left;

        // If right child is larger than largest so far
        if (right < maxHeap->numOfCars && maxHeap->array[right] > maxHeap->array[largest])
            largest = right;

        // If largest is not root
        if (largest != idx) {
            // Swap
            unsigned int temp = maxHeap->array[largest];
            maxHeap->array[largest] = maxHeap->array[idx];
            maxHeap->array[idx] = temp;

            // Move to the next node
            idx = largest;
        }
    } while (largest != idx);
}

void addCar(pMaxHeap maxHeap, unsigned int carID) {
    if(maxHeap->numOfCars == MAX_SIZE_CARS) {
        exit(6);
    }
    // First insert the new number at the end
    int i = maxHeap->numOfCars;
    maxHeap->array[i] = carID;
    maxHeap->numOfCars++;

    // Fix the max heap property if it is violated
    while (i != 0 && maxHeap->array[(i - 1) / 2] < maxHeap->array[i]) {
        unsigned int temp = maxHeap->array[i];
        maxHeap->array[i] = maxHeap->array[(i - 1) / 2];
        maxHeap->array[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

pMaxHeap createMaxHeap() {
    pMaxHeap heap = (pMaxHeap) malloc(sizeof(MaxHeap));
    heap->numOfCars = 0;
    return heap;
}

int binarySearch(const unsigned int arr[], int size, unsigned int target) {
    int low = 0;
    int high = size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;  // Calculate the middle index

        if (arr[mid] == target) {
            return mid;  // Found the target, return the index
        } else if (arr[mid] < target) {
            low = mid + 1;  // Target is in the upper half
        } else {
            high = mid - 1;  // Target is in the lower half
        }
    }

    return -1;  // Target not found
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
void mergeSort(unsigned int arr[], unsigned int size) {
    if (size < 2) {
        return; // Base case: the array is already sorted or empty
    }

    unsigned int mid = size / 2;
    unsigned int left[mid];
    unsigned int right[size - mid];

    // Divide the array into two sub-arrays
    for (unsigned int i = 0; i < mid; i++) {
        left[i] = arr[i];
    }
    for (unsigned int i = mid; i < size; i++) {
        right[i - mid] = arr[i];
    }

    // Recursively sort the two sub-arrays
    mergeSort(left, mid);
    mergeSort(right, size - mid);

    // Merge the sorted sub-arrays
    merge(arr, left, mid, right, size - mid);
}
#pragma clang diagnostic pop

void merge(unsigned int arr[], const unsigned int left[], unsigned int leftSize, const unsigned int right[], unsigned int rightSize) {
    unsigned int i = 0; // Index for left subarray
    unsigned int j = 0; // Index for right subarray
    unsigned int k = 0; // Index for merged array

    while (i < leftSize && j < rightSize) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of left subarray, if any
    while (i < leftSize) {
        arr[k] = left[i];
        i++;
        k++;
    }

    // Copy the remaining elements of right subarray, if any
    while (j < rightSize) {
        arr[k] = right[j];
        j++;
        k++;
    }
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