//
// Created by 盆栽 on 2020/5/7.
//
#include <glib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#define MAXLINE 9999

int listen_fd;


void sigchld_handler(int sig) {
    printf("Exit");
    fflush(stdout);

    if (listen_fd > 0) close(listen_fd);
    exit(0);
}

void print_client_info(const struct sockaddr_in* outsockaddr) {
    char ipstr[16];
    memset(ipstr, 0, sizeof(ipstr));

    inet_ntop(AF_INET, &outsockaddr->sin_addr.s_addr, ipstr, sizeof(ipstr));
    printf("来了老弟: %s(%d)\n", ipstr, ntohs(outsockaddr->sin_port));
}


gboolean g_str_equal(gconstpointer v1, gconstpointer v2) {
    return strcmp((const gchar *)v1, (const gchar *)v2) == 0;
}


guint g_str_hash(gconstpointer v) {
    const signed char *p = v;
    guint32 h = *p;
    if (h) {
        for (p+=1; *p!='\0'; p++) {
            h = (h<<5) - h + *p;
        }
    }

    return h;
}


int main(int argc, char **argv) {


    signal(SIGINT, sigchld_handler);

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

    GHashTable *gHashTable = g_hash_table_new(g_str_hash, g_str_equal);

    memset(&buf, 0, sizeof(buf));

    for (;;) {
        conn_fd = accept(listen_fd, (struct sockadrr*)&client_addr, &client_addr_len);

        if (conn_fd < 0) {
            perror("accept error");
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
                    g_hash_table_insert(gHashTable, strdup(params[1]), strdup(params[2]));
                }

            } else if (strcmp(params[0], "GET") == 0 ) {
                gconstpointer value = g_hash_table_lookup(gHashTable, (gconstpointer)params[1]);
                if (value) printf("%s\n", (char *)value);
            } else {
                printf("老弟你瞎几把说啥: (%s, %s, %s)\n", params[0], params[1], params[2]);
            }

            fflush(stdout);

            memset(&buf, 0, sizeof(buf));

        }

        perror("something fuck");

        printf("bye bye %d\n", conn_fd);
        close(conn_fd);
    }


}



