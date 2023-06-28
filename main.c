#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
/*
* exit codes:
*  5 - invalid action
*/

#define BUFFER_SIZE 250

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
 * red-black tree
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
    int key;
    int numCars;
    Color color;
    pNode parent;
    pNode left;
    pNode right;
}Node;

/* Function Declarations */

/*
 * Function: readInt
 * Description: reads int from input stream
 * Parameters:
 *   - number: pointer to int to store the read number
 * Returns: 0 if EOF, 1 otherwise (remember that when it returns 0 number is updated)
 */
int readInt(int *number);
/*
 * Function: readAction
 * Description: reads from input stream and returns the action
 * Returns: the action read
 */
Action readAction();
/*
 * Function: generateNewNode
 * Description: generates a new node with the given key for the red-black tree
 * Parameters:
 *   - key: key of the new node
 * Returns: pointer to the new node
 */
pNode generateNewNode(int key);
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
void insert(pNode *root, int key);

int main() {
    Action action; //action to perform
    int number; //number read from input
    pNode root = NULL;

    while ((action = readAction()) != ENDINPUT) {
        switch (action) {
            case ADDSTATION:
                printf("add station\n");
                readInt(&number);
                while (readInt(&number) != 0) {
                    printf("number: %d\n", number);
                    insert(&root,number);
                }
                printf("number: %d\n", number);
                insert(&root,number);
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

void insert(pNode *root, int key) {
    pNode newNode = generateNewNode(key);

    pNode y = NULL;
    pNode x = *root;

    while (x != NULL) {//find the right place to insert the new node
        y = x;
        if (key < x->key) { //go left
            x = x->left;
        } else if(key == x->key) {//car already exists
            x->numCars++;
            return;
        } else {//go right
            x = x->right;
        }
    }

    newNode->parent = y;

    if (y == NULL) {//tree is empty
        *root = newNode;
    } else if (key < y->key) {
        y->left = newNode;
    }
    else {
        y->right = newNode;
    }

    if (newNode->parent == NULL) {
        newNode->color = BLACK;
        return;
    }

    if (newNode->parent->parent == NULL) {
        return;
    }

    insertFix(root,newNode);
}

void insertFix(pNode *root, pNode node) {
    pNode x;

    while (node->parent->color == RED) {

        if (node->parent == node->parent->parent->right) {
            x = node->parent->parent->left;

            if (x != NULL && x->color == RED) {
                x->color = BLACK;
                x->parent->right->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rightRotate(root,node);
                }

                if(node->parent->color == BLACK)
                    break;

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                leftRotate(root,node->parent->parent);
            }
        } else { //If node is the left child of grandParent of newNode
            x = node->parent->parent->right; //right child of granParent

            if (x != NULL && x->color == RED) {
                x->color = BLACK;
                x->parent->left->color = BLACK; //left child of granParent
                node->parent->parent->color = RED; //granParent of node
                node = node->parent->parent; //node = granParent
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    leftRotate(root,node);
                }

                if(node->parent->color == BLACK)
                    break;

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rightRotate(root,node->parent->parent);

            }
        }
        if (node == *root) {
            break;
        }
    }
    (*root)->color = BLACK;
}

void leftRotate(pNode *root, pNode node) {
    pNode x = node->right;
    node->right = x->left;

    if (x->left != NULL) {
        x->left->parent = node;
    }
    x->parent = node->parent;
    if (node->parent == NULL) {
        *root = x;
    } else if (node == node->parent->left) {
        node->parent->left = x;
    } else {
        node->parent->right = x;
    }
    x->left = node;
    node->parent = x;
}

void rightRotate(pNode *root, pNode node) {
    pNode y = node->left;
    node->left = y->right;

    if (y->right != NULL) {
        y->right->parent = node;
    }
    y->parent = node->parent;
    if (node->parent == NULL) {
        *root = y;
    } else if (node == node->parent->right) {
        node->parent->right = y;
    } else {
        node->parent->left = y;
    }
    y->right = node;
    node->parent = y;
}

pNode generateNewNode(int key) {

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

        if(buffer[9] == 's') //check if the action is add station or add car
            return ADDSTATION;
        else
            return ADDCAR;
    }
    else if(buffer[0] == 'd') {

        if(buffer[9] == 's') //check if the action is remove station or remove car
            return RMVSTATION;
        else
            return RMVCAR;
    }
    else{ //if the action is not valid, exit the program
        exit(5);
    }
}

int readInt(int *number) {
    char buffer[BUFFER_SIZE];
    int value = 0;
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

