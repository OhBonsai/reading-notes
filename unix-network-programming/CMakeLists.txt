cmake_minimum_required(VERSION 3.15)
project(unixnetwork C)

set(CMAKE_C_STANDARD 99)



add_executable(c2_1 chapter2/fork.c)
add_executable(c2_2 chapter2/exce.c)
add_executable(c2_3 chapter2/proc_fiel.c  chapter2/util.h)
add_executable(c2_4 chapter2/wait.c)
add_executable(smallsh chapter2/smallsh.c chapter2/smallsh.h)

add_executable(server chapter2/server.c)
add_executable(client chapter2/client.c)
