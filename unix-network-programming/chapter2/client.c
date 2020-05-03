//
// Created by 盆栽 on 2020/5/3.
//
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#define MAXLINE 9999


int main(int argc, char **argv) {

    int socket_fd ;

    if ((socket_fd= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)  {
        perror("new socket error");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8888);


    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect fail");
        exit(1);
    }

    for(;;) {
        char buf[1024];
        scanf("%s", buf);
        write(socket_fd, buf, strlen(buf));
    }
    return 0;
}
