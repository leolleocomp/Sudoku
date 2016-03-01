# Define required macros here
SHELL = /bin/sh

OBJS =  main.c
CFLAG = -Wall
CC = gcc
INCLUDE =
LIBS = -lm -lgraph -lGL -lGLU -lglut 

main:${OBJS}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}

#clean:
#   -rm -f *.o core *.core

#.cpp.o:
#   ${CC} ${CFLAGS} ${INCLUDES} -c $<
