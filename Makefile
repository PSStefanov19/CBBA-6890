CC=g++
CCFLAGS=-Wall -O2
SYS:=$(shell g++ -dumpmachine)
BIN=bin
	
ifeq (, $(findstring mingw, $(SYS)) )
	CCFLAGS+=-llibncursesw -I.\include\ncursesw -L.\lib -I.\include
	EXT=.exe
else
	CCFLAGS+=-lncurses
	EXT=.out
endif

default:
	$(CC) ./src/main.cpp -o $(BIN)/main$(EXT) $(CCFLAGS)