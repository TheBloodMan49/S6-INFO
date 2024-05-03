cmake_minimum_required(VERSION 3.29)
project(tp04 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -Wall -Wextra -Werror -pedantic")

add_executable(ex1 ex1.c)
add_executable(ex2 ex2.c)
add_executable(ex3 ex3.c)
