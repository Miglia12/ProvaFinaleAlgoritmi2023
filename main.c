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

typedef enum color {
    RED,
    BLACK
}Color;

typedef struct node* pNode;

typedef struct node {
    int key;
    Color color;
    pNode parent;
    pNode left;
    pNode right;
}Node;

//reads int from input stream, returns 0 if the input stream is empty, 1 otherwise and stores the int in number (when it returns 0 the last int is still stored in number)
int readInt(int *number);
//reads string from input stream and returns the type of action to perform
Action readAction();

int main() {
    Action action;
    int number;

    while ((action = readAction()) != ENDINPUT) {
        switch (action) {
            case ADDSTATION:
                printf("add station\n");
                break;
            case RMVSTATION:
                printf("remove station\n");
                break;
            case ADDCAR:
                printf("add car\n");
                break;
            case RMVCAR:
                printf("remove car\n");
                break;
            case PLANROUTE:
                printf("plan route\n");
                break;
            default:
                printf("invalid action\n");
                exit(5);
        }
        while (readInt(&number) != 0) {
            printf("number: %d\n", number);
        }
        printf("number: %d\n", number);
    }
    return 0;
}

pNode generateNewNode(int key) {

    pNode newNode = (pNode) malloc(sizeof(Node));
    newNode->key = key;
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

