//
// Created by 盆栽 on 2020/5/2.
//
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "util.h"

void main() {
    int pid;
    char buf[10];

    int fd = open("/Users/penzai/w/c/reading-notes/unix-network-programming/chapter2/data", O_RDONLY);

    if (fd <  0)  failure("open failed");


    read(fd, buf, 10);

    printpos("before fork", fd);

    if ((pid=fork()) < 0)
        failure("fork failed");
    else if (!pid) {
        printpos("child before read", fd);
        read(fd, buf, 10);
        printpos("child after read", fd);
    } else {
        wait(NULL);
        printpos("parent after wait", fd);
    }
}
