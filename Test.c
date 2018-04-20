//
// Created by Gabriel Mitterrutzner on 27.04.17.
//

#include <stdio.h>

#define PrintExpr(x) (printf("%s = %d\n", #x, (x)))

int addition(int numberOne, int numberTwo);
int suptraktion(int numberOne, int numberTwo);
int multplikation(int numberOne, int numberTwo);
int divisoin(int numberOne, int numberTwo);

int main(int argc, char **argv) {
    PrintExpr(2+2);

    int test = 4;
    void* heii = &test;
    test = *((int*)heii);
    printf("%d\n", test);

    char testArray[] = "Esel";

    printf("\n%c\n", 1[testArray]);

    int (*array[4])(int, int);

    array[0] = &addition;
    array[1] = &suptraktion;
    array[2] = &multplikation;
    array[3] = &divisoin;

    printf("\nFunktion Pointer\n");

    for (int i = 0; i < 4; ++i) {
        printf("%d\n", array[i](5, 3));
    }

    printf("\n%c\n", ('a' + 'b' - 'd' + 2));

    return 0;
}

int addition(int numberOne, int numberTwo) {
    return (numberOne + numberTwo);
}

int suptraktion(int numberOne, int numberTwo){
    return (numberOne - numberTwo);
}

int multplikation(int numberOne, int numberTwo){
    return (numberOne * numberTwo);
}

int divisoin(int numberOne, int numberTwo){
    return (numberOne / numberTwo);
}