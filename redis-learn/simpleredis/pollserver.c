//
// Created by 盆栽 on 2020/5/3.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/un.h>
#include <fcntl.h>
#include "sredis.h"
#include "errno.h"
#include "event.h"

const int PORT = 8888;


void ReadQueryFromClient(struct eventLoop *eventLoop, int fd, void *clientdata, int mask) {
    // 这里先每次读1000Byte
    int nread, readlen=1000;
    char buf[readlen];

    nread = read(fd, buf, sizeof(buf));

    if (nread == -1) {
        printf("read from connect fail");
    } else {
        // 处理输入
        buf[nread] = '\0';
        printf("I got stream %s", buf);
    }
}



static int ListenPort() {
    int listen_fd;
    struct sockaddr_in server_addr;

    listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_fd < 0) {
        perror("new socket error");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    int opt_param = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt_param, sizeof(opt_param)) == -1) {
        perror("set socket reuse error");
        exit(1);
    }

    if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind socket error");
        exit(1);
    }

    int flags;
    if ((flags = fcntl(listen_fd, F_GETFL)) == -1) {
        perror("get file state fail");
        exit(1);
    }

    if (fcntl(listen_fd, flags | O_NONBLOCK) == -1) {
        perror("set socket nonblock fail");
        exit(1);
    }


    if (listen(listen_fd, 511) == -1) {
        perror("listen error");
        close(listen_fd);
        exit(1);
    }

    return listen_fd;
}


static int AcceptConnect(int listen_fd) {
    int conn_fd;
    struct sockaddr_un sa;
    socklen_t salen = sizeof(sa);
    while(1) {
        conn_fd = accept(listen_fd, (struct sockaddr*)&sa, &salen);
        if (conn_fd == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("accept error");
                return -1;
            }

        }
        break;
    }
    return conn_fd;
}


void AcceptUnixHandler(EventLoop *el, int fd, void *privdata, int mask) {
    int conn_fd, max = 1000;

    while(max--) {
        conn_fd = AcceptConnect(fd);
        printf("accept connection %d\n", conn_fd);

        if (conn_fd == -1) {
            printf("connect fail");
        };

        // set non block
        int flags;
        if ((flags ==fcntl(fd, F_GETFL)) == -1) {
            printf("get file flags fail");
        }
        if (fcntl(conn_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
            printf("set file flags nonblock fail");
        }

        int t1 = 1;
        if (setsockopt(conn_fd, IPPROTO_TCP, TCP_NODELAY, &t1, sizeof(t1)) == -1) {
            printf("disable Nagle fail");
        }

        if (setsockopt(conn_fd, SOL_SOCKET, SO_KEEPALIVE, &t1, sizeof(t1)) == -1) {
            printf("set keep alive fail");
        }

        CreateFileEvent(el, conn_fd, AE_READABLE,
                        ReadQueryFromClient, NULL);
    }

}

int main(int argc, char **argv) {
    int listen_fd = ListenPort();
    EventLoop *el = NewEventLoop(100);
    if (CreateFileEvent(el, listen_fd, AE_READABLE, AcceptUnixHandler, NULL) == AE_ERR){
        exit(1);
    }
    printf("staring polling event");
    Main(el);
    FreeEventLoop(el);
    return 0;
}





