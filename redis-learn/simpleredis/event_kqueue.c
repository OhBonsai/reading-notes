// Created by 盆栽 on 2020/5/7.

#include <sys/event.h>
#include <sys/types.h>
#include <sys/_types/_timespec.h>
#include <sys/_types/_timeval.h>
#include "event.h"


typedef struct {
    int kqfd;
    struct kevent *events;
} ApiState;


static int ApiCreate(EventLoop *eventLoop) {
    ApiState *state = malloc(sizeof(ApiState));
    if (!state) return -1;

    state->events = malloc(sizeof(struct kevent) * eventLoop->setsize);
    if (!state->events) {
        free(state);
        return -1;
    }

    state->kqfd = kqueue();
    if (state->kqfd == -1) {
        free(state->events);
        free(state);
        return -1;
    }

    eventLoop->apidata = state;
    return 0;
}

static int ApiResize(EventLoop *eventLoop, int setsize) {
    ApiState *state = eventLoop->apidata;
    state->events = realloc(state->events, sizeof(struct kevent) * setsize);
    return 0;
}


static int ApiAddEvent(EventLoop *eventLoop, int fd, int mask) {
    ApiState *state = eventLoop->apidata;
    struct kevent ke;

  if (mask & AE_READABLE) {
        EV_SET(&ke, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
        if (kevent(state->kqfd, &ke, 1, NULL, 0, NULL) == -1) return -1;
    }

    if (mask & AE_WRITABLE) {
        EV_SET(&ke, fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
        if (kevent(state->kqfd, &ke, 1, NULL, 0, NULL) == -1) return -1;
    }
    return 0;
}


static void ApiDeleteEvent(EventLoop *eventLoop, int fd, int delmask) {
    ApiState *state = eventLoop->apidata;
    struct kevent ke;

    if (delmask & AE_READABLE) {
        EV_SET(&ke, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
        kevent(state->kqfd, &ke, 1, NULL, 0, NULL);
    }

    if (delmask & AE_WRITABLE) {
        EV_SET(&ke, fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
        kevent(state->kqfd, &ke, 1, NULL, 0, NULL);
    }
}

static void ApiFree(EventLoop *eventLoop) {
    ApiState *state = eventLoop->apidata;
    close(state->kqfd);
    free(state->events);
    free(state);
}


static int ApiPoll(EventLoop *eventLoop, struct timeval *tvp) {
    ApiState *state = eventLoop->apidata;

    int retval, numevents = 0;

    if (tvp != NULL) {
        struct timespec timeout;
        timeout.tv_sec = tvp->tv_sec;
        timeout.tv_nsec = tvp->tv_usec * 1000;
        retval = kevent(state->kqfd, NULL, 0, state->events, eventLoop->setsize, &timeout);
    } else {
        retval = kevent(state->kqfd, NULL, 0, state->events, eventLoop->setsize, NULL);
    }

    if (retval > 0) {
        int j;

        numevents = retval;
        for (j = 0; j < numevents; j++) {
            int mask = 0;
            struct kevent *e = state->events+j;

            if (e->filter & EVFILT_READ) mask |= AE_READABLE;
            if (e->filter & EVFILT_WRITE) mask |= AE_WRITABLE;

            eventLoop->fired[j].fd = e->ident;
            eventLoop->fired[j].mask = mask;
        }
    }

    return numevents;
}


