cmake_minimum_required(VERSION 3.14)
project(Project C)

set(CMAKE_C_STANDARD 90)

add_executable(Project main.c linkedList.c linkedList.h board.c board.h errors.c errors.h linkedListCells.c linkedListCells.h command.c command.h stack.c stack.h game.c game.h commandValidator.c commandValidator.h parser.c parser.h execute.c execute.h solver.c solver.h fileaux.c fileaux.h cell.c cell.h)