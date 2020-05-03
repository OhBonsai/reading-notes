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




void print_client_info(const struct sockaddr_in* outsockaddr) {
    char ipstr[16];
    memset(ipstr, 0, sizeof(ipstr));

    inet_ntop(AF_INET, &outsockaddr->sin_addr.s_addr, ipstr, sizeof(ipstr));
    printf("老弟来了 %s(%d)\n", ipstr, ntohs(outsockaddr->sin_port));
}


int main(int argc, char **argv) {
    int listen_fd, conn_fd;
    socklen_t len;
    struct sockaddr_in server_addr, client_addr;
    char buf[MAXLINE];

    listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listen_fd < 0) {
        perror("socket error");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8888);


    if(bind(listen_fd, (struct sockadrr*)&server_addr, sizeof(server_addr)) < 0 ) {
        perror("bind error");
        exit(1);
    }

    if(listen(listen_fd, 100000) < 0) {
        perror("listen error");
        exit(1);
    }

    socklen_t client_addr_len = sizeof(client_addr);
    for (;;) {
        conn_fd = accept(listen_fd, (struct sockadrr*)&client_addr, &client_addr_len);

        if (conn_fd < 0) {
            perror("accept error");
            continue;
        }

        print_client_info(&client_addr);

        while (read(conn_fd, buf, sizeof(buf)) > 0) {
            printf("老弟，说了啥: %s\n", buf);
        }


        printf("走了老弟: %d\n", conn_fd);
        close(conn_fd);
    }


}

