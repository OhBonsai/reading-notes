cmake_minimum_required(VERSION 3.15)
project(simpleredis C)

set(CMAKE_C_STANDARD 99)

add_executable(server server.c sredis.c sredis.h)
add_executable(client client.c)
add_executable(test sredis.c sredis.h test.c)

