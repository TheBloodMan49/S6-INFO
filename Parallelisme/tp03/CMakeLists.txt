cmake_minimum_required(VERSION 3.28)
project(tp03 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -Wall -Wextra -Werror -Wpedantic")

add_executable(tp03 main.c)
