#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"

/*
 * exit codes:
 *  5 - invalid action
 *  6 - heap is full
 *  7 - vector not created
 *  8 - getValuesInRange failed
 *  9 - planRoute failed
 *  10 - isInRange failed (same station checked)
*/

#define BUFFER_SIZE 250 //size of the buffer to read from input
#define MAX_SIZE_CARS 513 //maximum number of cars in a station
#define VECTOR_SIZE_FACTOR 0.5 //factor to increase or decrease the initial size of the vector compared to the number of stations in the tree

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
 * Description: struct to store maxRange and stationIndex (used in planRoute)
 * Values:
 *  - maxRange: sum of the maximum range of the cars in the station and the stationID
 *  - stationIndex: index of the station in the vector
 */
typedef struct pair{
    int maxRange;
    int stationIndex;
}Pair;
/*
 * Description: struct to store maxRange and stationIndex (used in planRoute)
 * Values:
 *   - maxRange: sum of the maximum range of the cars in the station and the stationID
 *   - second: index of the station in the vector
 *   - next: pointer to the next Entry
 */
typedef struct entry {
    unsigned int maxRange;
    int stationIndex;
    struct entry* next;
}Entry;
/*
 * Description: pointer to an entry
 */
typedef struct entry* pEntry;
/*
 * Description: struct to store a Queue
 * Values:
 *   - numberOfElements: number of elements in the queue
 *   - head: pointer to the first node
 *   - tail: pointer to the last node
 */
typedef struct queue {
    pEntry head;
    pEntry tail;
}Queue;
/*
 * Description: pointer to a queue
 */
typedef struct queue* pQueue;
/*
 * Description: struct to store a vector
 * Values:
 *   - size: size of the vector
 *   - numberOfElements: number of elements in the vector
 *   - array: array to store the elements of the vector
 */
typedef struct vector {
    int size;
    int numberOfElements;
    unsigned int* array;
}Vector;
/*
 * Description: pointer to a vector
 */
typedef struct vector* pVector;
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
 *   - pair1: ID of the station
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
 * Function: newQueue
 * Description: creates a new queue
 * Parameters: void
 * Returns: pointer to the new queue
 */
pQueue newQueue();
/*
 * Function: enqueue
 * Description: adds a new element to the queue
 * Parameters:
 *   - queue: pointer to the queue
 *   - maxRange: sum of the maximum range of the cars in the station and the stationID
 *   - stationIndex: index of the station in the vector
 * Returns: void
 */
void enqueue(pQueue queue, int maxRange, int stationIndex);
/*
 * Function: dequeue
 * Description: removes the first element from the queue
 * Parameters:
 *   - queue: pointer to the queue
 * Returns: pointer to the removed element
 */
pEntry dequeue(pQueue queue);
/*
 * Function: freeQueue
 * Description: frees the memory allocated for the queue
 * Parameters:
 *   - queue: pointer to the queue to free
 * Returns: void
 */
void freeQueue(pQueue queue);
/*
 * Function: newVector
 * Description: creates a new vector
 * Parameters:
 *  - size: size of the vector
 * Returns: pointer to the new vector
 */
Vector *newVector(int size);
/*
 * Function: freeVector
 * Description: frees the memory allocated for the vector
 * Parameters:
 *   - vector: pointer to the vector to free
 * Returns: void
 */
void freeVector(Vector* vector);
/*
 * Function: addVector
 * Description: adds an element to the vector
 * Returns: 1 if the element was added, 0 otherwise
 */
int addVector(Vector *vector, unsigned int value);
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
 * Description: generates a new station with the given pair1 for the red-black tree
 * Parameters:
 *   - pair1: pair1 of the new station
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
 *   - pair1: pair1 of the new station
 * Returns: pointer to the new station if it was added, NULL otherwise
 */
pStation addStation(pStation *root, unsigned  int stationID);
/*
 * Function: removeStation
 * Description: removes a station from the tree
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - pair1: of the station to remove
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
/*
 * Function: planRouteInOrder
 * Description:  The function uses an in-order traversal of a Red-Black Tree of stations, considering only those within the desired range.
 *               At each station, it checks if the station is reachable based on the current maximum range.
 *               If the station is reachable, it updates the current maximum range and maintains a priority queue of possible routes.
 *               If the current maximum range cannot reach the next station, it dequeues the next possible route until a viable route is found or the queue is empty.
 * Parameters:
 *   - currentStation: pointer to the currentStation of the tree
 *   - start: start station
 *   - end: end station
 * Returns: 1 if the route was found, 0 otherwise
 */
int planRouteInOrder(pStation root, unsigned int start, unsigned int end);
/*
 * Function: planRouteReverseOrder
 * Description: plans a route from the start station to the end station if the stations are in reverse order
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - start: start station
 *   - end: end station
 * Returns: 1 if the route was found, 0 otherwise
 */
int planRouteReverseOrder(pStation root, unsigned int start, unsigned int end);
/*
 * Function: planRoute
 * Description: plans a route from the start station to the end station
 * Parameters:
 *   - root: pointer to the root of the tree
 *   - start: start station
 *   - end: end station
 * Returns: 1 if the route was found, 0 otherwise
 */
void planRoute(pStation root, unsigned int start, unsigned int end);

/* Global variables */
unsigned int numberOfStations = 0; //number of stations in the tree

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
                readInt(&stationID); //read the station id
                readInt(&carID); //reads the second station id
                planRoute(root, stationID, carID); //plans the route
                break;
            default:
                printf("invalid action\n");
                exit(5);
        }
    }
    return 0;
}

int planRouteReverseOrder(pStation root, unsigned int start, unsigned int end) {
    if(root == NULL)
        return 0;
    printf("\nPlanning Route in Reverse from %u to %u\n", start, end);
    pStation currentStation = root;
    Pair bestCandidate;
    bestCandidate.maxRange = -1;
    bestCandidate.stationIndex = -1;
    // Create an empty vector that will be used to store all the stations in the range, allowing to retrieve each station at each step
    pVector stations = newVector((int) (numberOfStations * VECTOR_SIZE_FACTOR));
    // Create an empty vector that will be used to store the predecessors of each station. This will allow to retrieve the path at the end
    pVector predecessors = newVector((int) (numberOfStations * VECTOR_SIZE_FACTOR));
    // Create a variable to store the current maximum range, initialized to the start station
    int currentMinRange = TMP_MAX;
    // Create a variable to store the index of the station with the current maximum range, initialized to 0
    unsigned int currentMinStationIndex = 0;
    int index = 0;
    pStation stack[numberOfStations];
    int topElementStack = -1;

    while (currentStation != NULL || topElementStack != -1) {
        // Go down the right subtree if the current node's distance is greater than or equal to the end of the range
        while (currentStation != NULL && currentStation->stationID >= end) {
            stack[++topElementStack] = currentStation;
            if (currentStation->stationID > start) {
                // Stop going down the right subtree if the current node's distance is greater than the start of the range
                break;
            }
            currentStation = currentStation->right;
        }

        if (topElementStack >= 0) {
            currentStation = stack[topElementStack--];

            // If the current node is within the range, it is part of the result
            if (currentStation->stationID <= start) {
                /*
                 * For the first station the currentMinRange is initialized to the stationID - the maximum range of the cars in the initial station
                 * Its index is initialized to 0, every starting station is always 0
                 */
                if(currentStation->stationID == start) { //this takes care of the initialization
                    addVector(predecessors, 0);
                    addVector(stations, currentStation->stationID);
                    currentMinRange = (int) currentStation->stationID - (int) currentStation->cars->array[0];
                    currentMinStationIndex = 0;
                    printf("\nStart: %u - MinRange: %d\n", currentStation->stationID, currentMinRange);
                } else {
                    //We immediately add the station to the list of stations so that we can retrieve it later
                    addVector(stations, currentStation->stationID);
                    printf("\nStation: %u - fi: %d\n", currentStation->stationID, (int) currentStation->stationID - (int) currentStation->cars->array[0]);

                    /*
                     * When the current station cannot reach the next station,
                     * we dequeue the next possible route until a viable route is found or the queue is empty.
                     */
                    if((int) currentMinRange > (int) currentStation->stationID) {

                        //if there is not the best candidate we stop
                        if (bestCandidate.stationIndex == -1) {
                            printf("cannot reach station with %d\n", currentMinRange);
                            freeVector(stations);
                            freeVector(predecessors);
                            return 0;
                        }
                        //if the best is present, we check if we can reach it and if we can we update the current minimum range
                        if (currentMinRange <= (int) stations->array[bestCandidate.stationIndex]) {
                            currentMinRange = bestCandidate.maxRange;
                            currentMinStationIndex = bestCandidate.stationIndex;
                            printf("Dequeued: %u - MinRange: %d\n", stations->array[bestCandidate.stationIndex], currentMinRange);
                            bestCandidate.stationIndex = -1;
                            bestCandidate.maxRange = -1;
                        } else {
                            printf("cannot reach station with %d\n", currentMinRange);
                            freeVector(stations);
                            freeVector(predecessors);
                            return 0;
                        }
                    }

                    /*
                     * We check if the new station has a longer range than the current maximum range, in this case we save it.
                     * The queue allows us to be sure not to miss any station with a longer range than the current maximum range
                     */
                    if(currentMinRange > (int) currentStation->stationID - (int) currentStation->cars->array[0])
                        if(currentStation->stationID != end &&
                        (bestCandidate.maxRange == -1 || (int) bestCandidate.maxRange > (int) currentStation->stationID - (int) currentStation->cars->array[0])) {
                            bestCandidate.maxRange = (int) currentStation->stationID - (int) currentStation->cars->array[0];
                            if(bestCandidate.maxRange < 0)
                                bestCandidate.maxRange = 0;
                            bestCandidate.stationIndex = index;
                            printf("Enqueued: %u - fi %d\n", currentStation->stationID, bestCandidate.maxRange);
                        }

                    addVector(predecessors, currentMinStationIndex);
                }
                index ++;
            }

            // Move to the left subtree
            currentStation = currentStation->left;
        } else {
            currentStation = NULL;
        }
    }
    pVector path = newVector((int) (numberOfStations * VECTOR_SIZE_FACTOR));
    index = stations->numberOfElements - 1;

    while(index != 0){
        addVector(path, stations->array[index]);
        index = (int) predecessors->array[index];
    }
    addVector(path, stations->array[0]);
    for(index = path->numberOfElements - 1; index >= 0; index--){
        printf("%u ", path->array[index]);
    }
    printf("\n");

    freeVector(path);
    freeVector(stations);
    freeVector(predecessors);
    return 1;
}

int planRouteInOrder(pStation root, unsigned int start, unsigned int end) {
    if(root == NULL)
        return 0;
    printf("\nPlanning Route in Reverse from %u to %u\n", start, end);
    pStation currentStation = root;
    // Create an empty queue that will be used to store the stations that have longer range than the current station
    pQueue maxRanges = newQueue();
    // Create an empty vector that will be used to store all the stations in the range, allowing to retrieve each station at each step
    pVector stations = newVector((int) (numberOfStations * VECTOR_SIZE_FACTOR));
    // Create an empty vector that will be used to store the predecessors of each station. This will allow to retrieve the path at the end
    pVector predecessors = newVector((int) (numberOfStations * VECTOR_SIZE_FACTOR));
    // Create a variable to store the current maximum range, initialized to the start station
    unsigned int currentMaxRange = 0;
    // Create a variable to store the index of the station with the current maximum range, initialized to 0
    unsigned int currentMaxStationIndex = 0;
    int index = 0;
    pEntry entry;
    pStation stack[numberOfStations];
    int topElementStack = -1;

    while (currentStation != NULL || topElementStack != -1) {
        // Go down the left subtree if the current node's distance is less than or equal to the end of the range
        while (currentStation != NULL && currentStation->stationID <= end) {
            stack[++topElementStack] = currentStation;
            if (currentStation->stationID < start) {
                // Stop going down the left subtree if the current node's distance is less than the start of the range
                break;
            }
            currentStation = currentStation->left;
        }

        if (topElementStack >= 0) {
            currentStation = stack[topElementStack--];


            if (currentStation->stationID >= start) { //from this point on the stations are in the range

                /*
                 * For the first station the currentMaxRange is initialized to the stationID + the maximum range of the cars in the initial station
                 * Its index is initialized to 0, every starting station is always 0
                 */
                if(currentStation->stationID == start) { //this takes care of the initialization
                    addVector(predecessors, 0);
                    addVector(stations, currentStation->stationID);
                    currentMaxRange = currentStation->stationID + currentStation->cars->array[0];
                    currentMaxStationIndex = 0;
                    printf("\nStart: %u - MaxRange: %d\n", currentStation->stationID, currentMaxRange);
                } else {

                    //We immediately add the station to the list of stations so that we can retrieve it later
                    addVector(stations, currentStation->stationID);
                    printf("\nStation: %u - fi: %d\n", currentStation->stationID, (int) currentStation->stationID - (int) currentStation->cars->array[0]);
                    /*
                     * When the current station cannot reach the next station,
                     * we dequeue the next possible route until a viable route is found or the queue is empty.
                     */
                    while(currentMaxRange < currentStation->stationID) {   //fMax < Si+1
                        entry = dequeue(maxRanges);
                        //if the queue is empty there is no viable route
                        if(entry == NULL) {
                            printf("cannot reach station with %d\n", currentMaxRange);
                            free(entry);
                            freeVector(stations);
                            freeVector(predecessors);
                            freeQueue(maxRanges);
                            return 0;
                        }
                        //if the queue is not empty we dequeue the next possible station and if we can reach the dequeued station we update the current maximum range
                        if(currentMaxRange >= stations->array[entry->stationIndex]) {
                            currentMaxRange = entry->maxRange;
                            currentMaxStationIndex = entry->stationIndex;
                            printf("Dequeued: %u - MinRange: %d\n", stations->array[entry->stationIndex], currentMaxRange);
                        } else {
                            printf("cannot reach station with %d\n", currentMaxRange);
                            free(entry);
                            freeVector(stations);
                            freeVector(predecessors);
                            freeQueue(maxRanges);
                            return 0;
                        }
                        free(entry);
                    }

                    /*
                     * We check if the new station has a longer range than the current maximum range, in this case we save it in the queue.
                     * The queue allows us to be sure not to miss any station with a longer range than the current maximum range
                     */
                    if(currentMaxRange < currentStation->stationID + currentStation->cars->array[0]) //fMax < fSi+1
                        if(currentStation->stationID != end &&
                          (maxRanges->tail == NULL || maxRanges->tail->maxRange < currentStation->stationID + currentStation->cars->array[0])) {
                            enqueue(maxRanges, (int) currentStation->stationID + (int) currentStation->cars->array[0], index);
                            printf("Enqueued: %u - fi %d\n", currentStation->stationID, (int) currentStation->stationID - (int) currentStation->cars->array[0]);
                        }

                    addVector(predecessors, currentMaxStationIndex);
                }

                index++;
            }

            currentStation = currentStation->right;
        } else {
            currentStation = NULL;
        }
    }

    pVector path = newVector((int) (numberOfStations * VECTOR_SIZE_FACTOR));
    index = stations->numberOfElements - 1;

    while(index != 0){
        addVector(path, stations->array[index]);
        index = (int) predecessors->array[index];
    }
    addVector(path, stations->array[0]);
    for(index = path->numberOfElements - 1; index >= 0; index--){
        printf("%u ", path->array[index]);
    }
    printf("\n");

    freeVector(path);
    freeVector(stations);
    freeVector(predecessors);
    freeQueue(maxRanges);
    return 1;
}


void planRoute(pStation root, unsigned int start, unsigned int end) {
    if(start == end) {
        exit(9);
    }
    if(start > end) {
        if(planRouteReverseOrder(root, start, end) == 0) {
            printf("nessun percorso\n");
            return;
        }
    } else {
        if(planRouteInOrder(root, start, end) == 0) {
            printf("nessun percorso\n");
            return;
        }
    }
}

pStation searchStation(pStation* root, unsigned int stationID) {
    pStation node = *root;

    while (node != NULL) {// Search for the station with the given pair1
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

    while (node != NULL) {// Search for the station with the given pair1
        if (stationID == node->stationID)
            break;

        if (stationID < node->stationID)
            node = node->left;
        else
            node = node->right;
    }

    if (node == NULL)// If the station with the given pair1 is not found, return
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
    numberOfStations--;
    return 1;
}

pStation addStation(pStation* root, unsigned int stationID) {// Function to addStation a station into the Red-Black Tree
    pStation newNode = createNode(stationID);

    pStation y = NULL;
    pStation x = *root;

    while (x != NULL) {// Search for the station with the given pair1
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
    numberOfStations++;
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

void freeQueue(pQueue queue) {
    pEntry temp;
    if(queue == NULL)
        return;
    while((temp = dequeue(queue)) != NULL) {
        free(temp);
    }
    free(queue);
}

pEntry dequeue(pQueue queue) {
    if(queue->head == NULL)
        return NULL;

    Entry* temp = queue->head;

    queue->head = queue->head->next;

    if(queue->head == NULL)
        queue->tail = NULL;

    return temp;
}

void enqueue(pQueue queue, int maxRange, int stationIndex) {
    pEntry entry = (pEntry) malloc(sizeof(Entry));
    if(maxRange < 0)
        maxRange = 0;

    entry->maxRange = maxRange;
    entry->stationIndex = stationIndex;
    entry->next = NULL;

    if(queue->head == NULL) {
        queue->head = entry;
        queue->tail = entry;
        return;
    }

    queue->tail->next = entry;
    queue->tail = entry;
}

pQueue newQueue() {
    pQueue queue = (pQueue) malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

int addVector(Vector *vector, unsigned int value) {

    if(vector == NULL) {
        exit(7);
    }

    if(vector->numberOfElements == vector->size) {
        vector->size = vector->size * 2;
        unsigned int* temp = (unsigned int*) realloc(vector->array, vector->size * sizeof(unsigned int));
        if(temp == NULL) {
            exit(7);
        }
        vector->array = temp;
    }

    vector->array[vector->numberOfElements] = value;
    vector->numberOfElements++;
    return 1;
}

void freeVector(Vector* vector) {
    if(vector != NULL) {
        if(vector->array != NULL) {
            free(vector->array); // Free the memory allocated for the array
        }
        free(vector); // Free the memory allocated for the vector itself
    }
}

Vector *newVector(int size) {
    pVector vector = (pVector) malloc(sizeof(Vector));
    if(vector == NULL) {
        exit(7);
    }

    vector->numberOfElements = 0;
    vector->array = (unsigned int*) malloc(size * sizeof(unsigned int));

    if(vector->array == NULL) {
        exit(7);
    }
    vector->size = size;

    return vector;
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

