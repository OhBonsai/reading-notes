//
// Created by 盆栽 on 2020/5/7.
//

#ifndef SIMPLEREDIS_EVENT_H
#define SIMPLEREDIS_EVENT_H

#include <glib.h>

#define AE_NONE 0
#define AE_READABLE 1
#define AE_WRITABLE 2

#define AE_FILE_EVENTS 1
#define AE_TIME_EVENTS 2
#define AE_ALL_EVENTS (AE_FILE_EVENT|AE_TIME_EVENTS)
#define AE_DONT_WAIT 4
#define AE_NOMORE -1


typedef void fileHandler(struct EL *el, inf fd, void *data, int mask);
typedef int timeHandler(struct EL *el, long id, void *data);
typedef void eventFree(struct EL *el, void *data);


typedef struct EL {
    int max_fd;
    int set_size;

    long time_next_id;
    GSList *time_event_list;
    GSList *register_file_event_list;
    GSList *ready_file_event_list;

    void *data;
    int stop;
} EventLoop;

typedef struct TE{
    long id;
    long when;

    timeHandler *handle_func;
    eventFree *free_func;
} TimeEvent;

typedef struct FE {
    int mask;
    fileHandler *read_handle_func;
    fileHandler *write_handle_func;

    void *data;
};

EventLoop *new_event_loop();
long create_time_event(
        EventLoop *el,
        long ms,
        timeHandler *handler,
        void *data,
        eventFree *free_func
);
int delete_time_event(EventLoop *el, long id);
int stop_event_lop(EventLoop *el);


#endif //SIMPLEREDIS_EVENT_H
