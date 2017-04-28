//
// Created by Gabriel Mitterrutzner on 27.04.17.
//

#include <stdio.h>

#define PrintExpr(x) (printf("%s = %d\n", #x, (x)))

int main(int argc, char **argv) {
    PrintExpr(2+2);

    int test = 4;
    void* heii = &test;
    test = *((int*)heii);
    printf("%d", test);
    return 0;
}
