#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


main() {
    int pid, status, exit_status;

    if ( (pid=fork()) < 0) {
        perror("fork failed");
        exit(1);
    }

    if (!pid) {
        sleep(4);
        exit(5);
    }


    if (wait(&status) < 0) {
        perror("wait failed");
        exit(1);
    }


    // 整形 低八位在前， 高八位在后
    if (status & 0xFF) {
        printf("Some low-roderbits not zero\n");
    } else {
        exit_status = status >> 8;
        exit_status &= 0xff;
        printf("Exist status from %d was %d\n", pid, exit_status);
    }

    exit(0);

}