all : simple

CPPFLAGS=-g

simple : simple.o learn.o
	g++ -g  -o simple simple.o learn.o

learn.o : learn.cpp learn.h

simple.o : simple.cpp learn.h


