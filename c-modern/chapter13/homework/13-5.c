//
// Created by 盆栽 on 2020/1/30.
//
#include <string.h>
#include <stdlib.h>

int main5(char argc, char **argv) {
    int sum = 0;

    for (int i=0; i<argc; i++) {
        sum += atoi(*(argv + i));
    }
    return sum;
}

