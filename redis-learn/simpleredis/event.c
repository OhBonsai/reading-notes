// Created by 盆栽 on 2020/5/7.

#include "event.h"
#include <sys/time.h>
#include <sys/errno.h>
#include <poll.h>


#ifdef __APPLE__
#include "event_kqueue.c"
#else
#include "event_epoll.c"
#endif


EventLoop *NewEventLoop(int setsize) {
    EventLoop *eventLoop;
    int i;


    if ((eventLoop = malloc(sizeof(&eventLoop))) == NULL) goto err;

    eventLoop->events = malloc(sizeof(FileEvent) * setsize);
    eventLoop->fired = malloc(sizeof(FiredEvent) * setsize);

    if (eventLoop->events == NULL || eventLoop->fired == NULL) goto err;

    eventLoop->setsize = setsize;
    eventLoop->maxfd = -1;
    eventLoop->stop = 0;

    if (ApiCreate(eventLoop) == -1) goto err;
    for (i = 0; i < setsize; i++) eventLoop->events[i].mask = AE_NONE;

    return eventLoop;

err:
    if (eventLoop) {
        free(eventLoop->events);
        free(eventLoop->fired);
        free(eventLoop);
    }
    return NULL;
}

void FreeEventLoop(EventLoop *el) {
    ApiFree(el);
    free(el->events);
    free(el->fired);
    free(el);
}

int CreateFileEvent(EventLoop *el, int fd, int mask, fileProc *proc, void *clientData) {
    if (fd >= el->setsize) {
        errno = ERANGE;
        return AE_ERR;
    }

    FileEvent *fe = &(el->events[fd]);
    if (ApiAddEvent(el, fd, mask) == -1) return AE_ERR;

    fe->mask |= mask;
    if (mask & AE_READABLE) fe->rfileProc = proc;
    if (mask & AE_WRITABLE) fe->wfileProc = proc;

    fe->clientData = clientData;

    if (fd > el->maxfd) el->maxfd = fd;
    return AE_OK;
}


void FreeFileEvent(EventLoop *el, int fd, int mask) {
    if (fd >= el->setsize) return;

    FileEvent *fe = &el->events[fd];

    if (fe->mask == AE_NONE) return;

    fe->mask = fe->mask & (~mask);

    if (fd == el->maxfd && fe->mask == AE_NONE) {
        int j;
        for (j = el->maxfd - 1; j >=0; j--) {
            if (el->events[j].mask != AE_NONE) break;
        }
        el->maxfd = j;
    }
    ApiDeleteEvent(el, fd, mask);
}


int GetFileEvent(EventLoop *el, int fd) {
    if (fd >= el->setsize) return 0;
    FileEvent *fe = &el->events[fd];
    return fe->mask;
}


int ProcessEvent(EventLoop *el, int flags) {
    int processed = 0, numevents;

    if (!(flags & AE_FILE_EVENTS)) return 0;

    if (el->maxfd != -1 || !(flags & AE_DONT_WAIT) ) {
        int j;
        numevents = ApiPoll(el, NULL);

        for (j=0; j<numevents; j++) {
            FileEvent *fe = &(el->events[el->fired[j].fd]);
            int mask = el->fired[j].mask;
            int fd = el->fired[j].fd;
            int rfired = 0;

            if (fe->mask & mask & AE_READABLE) {
                rfired = 1;
                fe->rfileProc(el, fd, fe->clientData, mask);
            }

            if (fe->mask & mask & AE_WRITABLE) {
                if (!rfired || fe->wfileProc != fe->rfileProc) {
                    fe->wfileProc(el, fd, fe->clientData, mask);
                }
            }

            processed++;
        }
    }

    return processed;
}




void Main(EventLoop *el) {
    el->stop = 0;

    while (!el->stop) {
        ProcessEvent(el, AE_ALL_EVENTS);
    }
}

