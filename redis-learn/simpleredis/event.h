//
// Created by 盆栽 on 2020/5/7.

#ifndef SIMPLEREDIS_EVENT_H
#define SIMPLEREDIS_EVENT_H

#include <glib.h>
#include <stdbool.h>

#define AE_OK 0
#define AE_ERR -1

#define AE_NONE 0
#define AE_READABLE 1
#define AE_WRITABLE 2

#define AE_FILE_EVENTS 1
#define AE_TIME_EVENTS 2
#define AE_ALL_EVENTS 3
#define AE_DONT_WAIT 4

#define AE_NOMORE -1

struct eventLoop;
typedef void fileProc(struct eventLoop *eventLoop, int fd, void *clientdata, int mask);


typedef struct {
    int mask;
    fileProc *rfileProc;
    fileProc *wfileProc;
    void *clientData
} FileEvent;


typedef struct{
    int fd;
    int mask;
} FiredEvent;


typedef struct eventLoop {
    int maxfd;
    int setsize;

    FileEvent *events;
    FiredEvent *fired;

    int stop;
    void *apidata;
} EventLoop;

EventLoop *NewEventLoop(int setsize);
void FreeEventLoop(EventLoop *el);
int CreateFileEvent(EventLoop *el, int fd, int mask, fileProc *proc, void *clientData);
void FreeFileEvent(EventLoop *el, int fd, int mask);
int GetFileEvent(EventLoop *el, int fd);
int ProcessEvent(EventLoop *el, int flags);
void Main(EventLoop *el);

#endif //SIMPLEREDIS_EVENT_H
