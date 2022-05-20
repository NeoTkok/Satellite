cmake_minimum_required(VERSION 3.20)
project(try_winsock_serv)

set(CMAKE_CXX_STANDARD 20)


add_executable(try_winsock_serv main.cpp handle_connections.h handle_connections.cpp includes.h)

if(WIN32)
    target_link_libraries(try_winsock_serv wsock32 ws2_32)
endif()