cmake_minimum_required(VERSION 3.15)
project(simpleredis C)

set(CMAKE_C_STANDARD 99)

add_executable(simpleredis sredis.c sredis.h)

