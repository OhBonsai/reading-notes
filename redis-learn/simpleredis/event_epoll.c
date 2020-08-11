// Created by 盆栽 on 2020/5/7.

#include <sys/epoll.h>
#include <stdio.h>


typedef struct {
    int epfd;
    struct epoll_event *events;
} ApiState;


static int ApiCreate(EventLoop *eventLoop) {

    ApiState *state = malloc(sizeof(ApiState));
    if (!state) return -1;

    fprintf(stderr, "Api create");

    state->events = malloc(sizeof(struct epoll_event) * eventLoop->setsize);
    if (!state->events) {
        free(state);
        return -1;
    }

    state->epfd = epoll_create(1024);
    if (state->epfd == -1) {
        free(state->events);
        free(state);
        return -1;
    }

    eventLoop->apidata = state;
    return 0;
}

static int ApiResize(EventLoop *eventLoop, int setsize) {
    ApiState *state = eventLoop->apidata;
    state->events = realloc(state->events, sizeof(struct epoll_event) * setsize);
    return 0;
}


static int ApiAddEvent(EventLoop *eventLoop, int fd, int mask) {
    ApiState *state = eventLoop->apidata;
    struct epoll_event ee;


    fprintf(stderr, "Api add event");


    int op = eventLoop->events[fd].mask == AE_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;
    ee.events = 0;
    mask |= eventLoop->events[fd].mask;

    if (mask & AE_READABLE) ee.events |= EPOLLIN;
    if (mask & AE_WRITABLE) ee.events |= EPOLLOUT;

    ee.data.u64 = 0;
    ee.data.fd = fd;

    if (epoll_ctl(state->epfd, op, fd, &ee) == -1) return -1;

    return 0;
}


static void ApiDeleteEvent(EventLoop *eventLoop, int fd, int delmask) {
    ApiState *state = eventLoop->apidata;
    struct epoll_event ee;

    int mask = eventLoop->events[fd].mask & (~delmask);

    ee.events = 0;
    if (mask & AE_READABLE) ee.events |= EPOLLIN;
    if (mask & AE_WRITABLE) ee.events |= EPOLLOUT;

    ee.data.u64 = 0;
    ee.data.fd = fd;

    if (mask != AE_NONE) {
        epoll_ctl(state->epfd, EPOLL_CTL_MOD, fd, &ee);
    } else {
        epoll_ctl(state->epfd, EPOLL_CTL_DEL, fd, &ee);
    }
}

static void ApiFree(EventLoop *eventLoop) {
    ApiState *state = eventLoop->apidata;
    close(state->epfd);
    free(state->events);
    free(state);
}


static int ApiPoll(EventLoop *eventLoop, struct timeval *tvp) {
    ApiState *state = eventLoop->apidata;

    int retval, numevents = 0;

    retval = epoll_wait(state->epfd, state->events, eventLoop->setsize,
            tvp ? (tvp->tv_sec * 1000 + tvp->tv_usec/1000) : -1);

    if (retval > 0) {
        int j;

        numevents = retval;
        for (j = 0; j < numevents; j++) {
            int mask = 0;
            struct epoll_event *e = state->events+j;

            if (e->events & EPOLLIN) mask |= AE_READABLE;
            if (e->events & EPOLLOUT) mask |= AE_WRITABLE;
            if (e->events & EPOLLERR) mask |= AE_WRITABLE;
            if (e->events & EPOLLHUP) mask |= AE_WRITABLE;

            eventLoop->fired[j].fd = e->data.fd;
            eventLoop->fired[j].mask = mask;
        }
    }

    return numevents;
}


