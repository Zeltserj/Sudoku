cmake_minimum_required(VERSION 3.14)
project(Project C)

set(CMAKE_C_STANDARD 90)

add_executable(Project main.c linkedList.c linkedList.h board.c board.h errors.c errors.h linkedListCells.c linkedListCells.h command.c command.h)