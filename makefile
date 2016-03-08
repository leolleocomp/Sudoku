# Define required macros here
SHELL = /bin/sh

OBJS =  main.c sudoku_solver/Sudoku.cpp sudoku_solver/SudokuOpr.cpp
CFLAG = -Wall
CC = g++
INCLUDE =
LIBS = -lm -lGL -lGLU -lglut 

main:${OBJS}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}

#clean:
#   -rm -f *.o core *.core

#.cpp.o:
#   ${CC} ${CFLAGS} ${INCLUDES} -c $<
