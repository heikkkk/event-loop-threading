cmake_minimum_required(VERSION 3.26)
project(event_loop_threading)

set(CMAKE_CXX_STANDARD 17)

add_executable(event_loop_threading Workers.cpp
        Workers.h)
