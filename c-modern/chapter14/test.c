//
// Created by 盆栽 on 2020/1/30.
//
#include <stdio.h>

#define println(s) printf("%s\n", s)
#define println2(s) printf(#s " = %s\n", s)

#define GENERIC_MAX(type)       \
type type##_max(type x, type y) \
{                               \
    return x > y ? x : y;       \
}

#define DEBUG


GENERIC_MAX(float)

int main(int argc, char **argv) {
    println("abc");
    println2("abc");
    char *str = "bcd";

    println2(str);


    printf("%.2f\n", float_max(2.0, 3.0));

#if defined(DEBUG)
    println2("abc");
#endif

#ifndef __APPLE__
#error only support apple platform
#endif

#ifdef WIN32
    println("windows\n");
#elif defined(__APPLE__)
    println("mac\n");
#elif defined(LINUX)
    println("linux\n");
#endif

    return 0;
}


