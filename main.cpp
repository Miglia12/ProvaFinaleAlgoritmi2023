#include <iostream>

#define MAX_CHAR_SIZE 123
#define BUFFER_SIZE 50

//reads int from input stream
int readInt();

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

int readInt() {
    char buffer[BUFFER_SIZE];
    int number = 0;
    int ch;
    int i = 0;
    int j;
    int p = 1;

    while((ch = getchar()) !='\n' && ch != EOF) {
        buffer[i] = (char) ch;
        i++;
    }
    buffer[i+1] = '\0';

    for(j = (i - 1); j >= 0; j--) {

        number = number + (buffer[j] - '0') * p;
        p = p * 10;
    }

    return number;
}
