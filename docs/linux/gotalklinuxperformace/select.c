
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/select.h>
#include<string.h>
#include<errno.h>

int main(int argc, char *argv[]) {
    struct timeval timeout;
    char buf[10];
    fd_set readfds;
    int nread, nfds, ready, fd;

    while (1) {
        timeout.tv_sec = 20L;
        timeout.tv_usec = 0;

        fd = 0;     //stdin
        nfds = fd + 1;
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);

        ready = select(nfds, &readfds, NULL, NULL, &timeout);
        if (ready == -1 && errno == EINTR) {
            continue;
        } else if (ready == -1) {
            fprintf(stderr, "select error:%s\n", strerror(errno));
        }
        for (fd = 0; fd < nfds; fd++) {
            if (FD_ISSET(fd, &readfds)) {
                nread = read(fd, buf, 9);
                buf[nread] = '\0';
                puts(buf);
            }
        }
    }

    return 0;
}
