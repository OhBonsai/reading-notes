//
// Created by 盆栽 on 2020/5/2.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

main(int argc, char ** argv) {

    pid_t pid = fork();
    switch (pid) {
        case -1:
            perror("fork fal");
            _exit(1);
        case 0:
            execlp("ls", "-la", NULL);
            perror("execl failed");
            _exit(0);
        default:
            wait(NULL);
            printf("ls completed\n");
            exit(0);
    }


}
