cmake_minimum_required(VERSION 3.15)
project(redis_learn C)

set(CMAKE_C_STANDARD 99)


add_executable(redis_learn main.c sds.h sds.c zmalloc.h zmalloc.c adlist.c adlist.h dict.c dict.h skiplist.c skiplist.h  meta.h intset.c intset.h ziplist.c ziplist.h util.c util.h)

add_executable(test_sds test/test_sds.c sds.h sds.c zmalloc.c zmalloc.h)
add_executable(test_dict test/test_dict.c zmalloc.c zmalloc.h dict.c dict.h)
add_executable(test_skiplist zmalloc.c zmalloc.h meta.h skiplist.c skiplist.h test/test_skiplist.c meta.c)
add_executable(test_test test/test_test.c)
add_executable(test_intset test/test_intset.c intset.c intset.h zmalloc.c zmalloc.h meta.h meta.c)