//
// Created by 盆栽 on 2020/3/19.
//
#include "meta.h"


void decrRefCount(robj *o) {
    o->refcount--;
}

