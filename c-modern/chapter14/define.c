//
// Created by 盆栽 on 2020/1/31.
//
#include <stdio.h>

#define tri(x) ((x)*(x)*(x))

#define maxthan100(x, y) ( ((x) * (y)) > 100 ) ? 1 : 0


int main(int argc, char **argv) {
    printf("%d\n", tri(3));
    int result = maxthan100(2,3);
    printf("%d\n", result);

    return 0;
}
