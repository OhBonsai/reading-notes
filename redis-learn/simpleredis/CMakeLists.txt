cmake_minimum_required(VERSION 3.15)
project(simpleredis C)

set(CMAKE_C_STANDARD 99)

set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/local/lib/pkgconfig")
find_package(PkgConfig REQUIRED)
pkg_check_modules(glib REQUIRED IMPORTED_TARGET glib-2.0)



#add_executable(server server.c sredis.c sredis.h)
add_executable(server glibserver.c event.c event.h)
target_link_libraries(server PRIVATE PkgConfig::glib)


add_executable(client client.c)
add_executable(test sredis.c sredis.h test.c)

