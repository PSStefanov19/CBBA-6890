CC=g++
CCFLAGS=-Wall -O2
SYS:=$(shell g++ -dumpmachine)
BIN=bin
	
ifneq (, $(findstring linux, $(SYS)) )
	CCFLAGS+=-lncurses
	EXT=.out
else ifneq(, $(findstring mingw, $(SYS)) )
	CCFLAGS+=-llibncursesw -I.\include\ncursesw -L.\lib -I.\include
	EXT=.exe
endif

default:
	$(CC) ./src/main.cpp -o $(BIN)/main$(EXT) $(CCFLAGS)