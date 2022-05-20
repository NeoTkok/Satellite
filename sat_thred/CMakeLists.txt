cmake_minimum_required(VERSION 3.20)
project(sat_thred)

set(CMAKE_CXX_STANDARD 20)

add_executable(sat_thred main.cpp satellite.cpp satellite.h integrator.h
               integrator.cpp includes.h vec_operations.h vec_operations.cpp)

if(WIN32)
    target_link_libraries(sat_thred wsock32 ws2_32)
endif()