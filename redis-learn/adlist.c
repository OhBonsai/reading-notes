//
// Created by 盆栽 on 2020/1/16.
//
#include <stdlib.h>

#include "adlist.h"
#include "zmalloc.h"

list *listCreate(void) {
    struct list *n_list;
    n_list = zmalloc(sizeof(*n_list));
    if (n_list == NULL) {
        return NULL;
    }

    n_list->head = NULL;
    n_list->tail = NULL;
    n_list->len = 0;

    n_list->dup = NULL;
    n_list->free = NULL;
    n_list->match = NULL;

    return n_list;
}



void listRelease(list *list) {
    unsigned long len;
    listNode *current, *next;

    current = list->head;
    len = list->len;
    while (len--) {
        next = current->next;
        if (list->free) list->free(current->value);

        zfree(current);
        current = next;
    }
    zfree(list);
}


list *listAddNodeHead(list *list, void *value){
    listNode *node;

    if ((node = zmalloc(sizeof(*node))) == NULL)
        return NULL;

    node->value = value;

    if (list->len == 0) {
        list->head = list->tail = node;
        node->prev = node->next = NULL;
    } else {
        node->prev = NULL;
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }

    list->len++;
    return list;
};


list *listAddNodeTail(list *list, void *value)
{
    listNode *node;

    // 为新节点分配内存
    if ((node = zmalloc(sizeof(*node))) == NULL)
        return NULL;

    // 保存值指针
    node->value = value;

    // 目标链表为空
    if (list->len == 0) {
        list->head = list->tail = node;
        node->prev = node->next = NULL;
        // 目标链表非空
    } else {
        node->prev = list->tail;
        node->next = NULL;
        list->tail->next = node;
        list->tail = node;
    }

    // 更新链表节点数
    list->len++;

    return list;
}


list *listInsertNode(list *list, listNode *old_node, void *value, int after) {
    listNode *node;

    if ((node = zmalloc(sizeof(*node))) == NULL)
        return NULL;

    node->value = value;

    if (after) {
        node->prev = old_node;
        node->next = old_node->next;

        if (list->tail == old_node) {
            list->tail = node;
        }
    } else {
        node->next = old_node;
        node->prev = old_node->prev;
        if (list->head == old_node) {
            list->head = node;
        }
    }

    // 更新新节点的前置指针
    if (node->prev != NULL) {
        node->prev->next = node;
    }
    // 更新新节点的后置指针
    if (node->next != NULL) {
        node->next->prev = node;
    }

    // 更新链表节点数
    list->len++;

    return list;
}

void listDelNode(list *list, listNode *node){
    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;


    if (list->free) list->free(node->value);

    zfree(node);
    list->len--;
}


listIter *listGetIterator(list *list, int direction){
    listIter *iter;
    if ((iter = zmalloc(sizeof(*iter))) == NULL) return NULL;

    if (direction == AL_START_HEAD)
        iter->next = list->head;
    else
        iter->next = list->tail;

    iter->direction = direction;
    return iter;
};

void listReleaseIterator(listIter *iter) {
    zfree(iter);
}

void listRewind(list *list, listIter *li) {
    li->next = list->head;
    li->direction = AL_START_HEAD;
}

void listRewindTail(list *list, listIter *li) {
    li->next = list->tail;
    li->direction = AL_START_TAIL;
}


listNode *listNext(listIter *iter){
    listNode *current = iter->next;

    if (current != NULL) {
        if (iter->direction == AL_START_HEAD)
            iter->next = current->next;
        else
            iter->next = current->prev;
    }
    return current;
}


list *listDup(list *orig) {
    list *copy;
    listIter *iter;
    listNode *node;

    if ((copy=listCreate()) == NULL)
        return NULL;

    copy->dup = orig->dup;
    copy->free = orig->free;
    copy->match = orig->match;

    iter = listGetIterator(orig, AL_START_HEAD);
    while((node = listNext(iter)) != NULL) {
        void *value;

        if (copy->dup) {
            value = copy->dup(node->value);
            if (value == NULL) {
                listRelease(copy);
                listReleaseIterator(iter);
                return NULL;
            }
        } else
            value = node->value;


        if (listAddNodeTail(copy, value) == NULL) {
            listRelease(copy);
            listReleaseIterator(iter);
            return NULL;;
        }
    }

    listReleaseIterator(iter);
    return copy;
}


listNode *listSearchKey(list *list, void *key) {
    listIter *iter;
    listNode *node;

    iter = listGetIterator(list, AL_START_HEAD);
    while ((node = listNext(iter)) != NULL) {
        if (list->match) {
            if (list->match(node->value, key)) {
                listReleaseIterator(iter);
                return node;
            }
        } else {
            if (key == node->value) {
                listReleaseIterator(iter);
                return node;
            }
        }
    }

    listReleaseIterator(iter);
    return NULL;
}


listNode *listIndex(list *list, long index) {
    listNode *n;

    if (index < 0) {
        index = (-index)-1;
        n = list->tail;
        while(index-- && n) n = n->prev;
    } else {
        n = list->head;
        while(index-- && n) n = n->next;
    }

    return n;
}


void listRotate(list *list) {
    listNode *tail = list->tail;

    if (listLength(list) <= 1) return;

    list->tail = tail->prev;
    list->tail->next =NULL;

    list->head->prev = tail;
    tail->prev = NULL;
    tail->next = list->head;
    list->head = tail;
};