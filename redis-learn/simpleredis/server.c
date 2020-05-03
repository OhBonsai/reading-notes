//
// Created by 盆栽 on 2020/5/3.
//
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include "sredis.h"
#define MAXLINE 9999


int listen_fd;

void signal_handler(int signo) {
    printf("service close\n");
    close(listen_fd);
    exit(0);
}


void print_client_info(const struct sockaddr_in* outsockaddr) {
    char ipstr[16];
    memset(ipstr, 0, sizeof(ipstr));

    inet_ntop(AF_INET, &outsockaddr->sin_addr.s_addr, ipstr, sizeof(ipstr));
    printf("来了老弟: %s(%d)\n", ipstr, ntohs(outsockaddr->sin_port));
}

int main(int argc, char **argv) {

    int conn_fd;
    socklen_t len;
    struct sockaddr_in server_addr, client_addr;
    char buf[MAXLINE];
    time_t ticks;

    listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listen_fd < 0) {
        perror("socket error");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8888);
    printf("我在%s等你\n", ":8888");



    if(bind(listen_fd, (struct sockadrr*)&server_addr, sizeof(server_addr)) < 0 ) {
        perror("bind error");
        exit(1);
    }

    if(listen(listen_fd, 100000) < 0) {
        perror("listen error");
        exit(1);
    }

    socklen_t client_addr_len = sizeof(client_addr);

    Dict *d = dict_new(&vt);


    for (;;) {
        conn_fd = accept(listen_fd, (struct sockadrr*)&client_addr, &client_addr_len);

        if (conn_fd < 0) {
            perror("accept error");
            continue;
        }

        print_client_info(&client_addr);

        while (read(conn_fd, buf, sizeof(buf)) > 0) {

            int i = 0;
            char  *params[3] = {"             ", "            ", "              "};

            char * param = strtok(buf, " ");
            while( param != NULL && i<=3) {
                params[i++] = param;
                param = strtok(NULL, " ");
            }

            if (strcmp(params[0], "SET") == 0) {
                if (strcmp(params[1], "") != 0 && strcmp(params[2], "") != 0) {
                    Object *k = init_string_obj(params[1], strlen(params[1]));
                    Object *v = init_string_obj(params[2], strlen(params[2]));
                    dict_add(d, k, v);
                }
            } else if (strcmp(params[0], "GET") == 0 ) {
                Object *k = init_string_obj(params[1], strlen(params[1]));
                DictEntry *de = dict_find(d, k);
                if (de) {
                    String value = (String)((Object *)de->val)->ptr;
                    printf("%s\n", value);
                }
            } else {
                printf("老弟你瞎几把说啥: (%s, %s, %s)\n", params[0], params[1], params[2]);
            }

            fflush(stdout);


        }

        perror("something fuck");

        printf("bye bye %d\n", conn_fd);
        close(conn_fd);
    }


}

