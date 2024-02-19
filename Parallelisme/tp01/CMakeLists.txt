cmake_minimum_required(VERSION 3.28)
project(tp01 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic-errors")

add_executable(tp01
        ex2.c)
