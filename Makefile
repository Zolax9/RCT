
APPNAME:=$(shell basename `pwd`)

LDFLAGS:=-L ../raylib/src -lm -lraylib -lX11 -ldl -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio -pthread

CFLAGS:= -g -pedantic -Wall -Wextra -Wno-missing-field-initializers
CFLAGS+= -std=c++17 -I ./include -I ../raylib/src -I /usr/include/opencv4

SRC:=$(wildcard src/*.cpp)
OBJ:=$(SRC:src/%.cpp=obj/%.o)
INC:=$(wildcard include/*.hpp)

CC=g++

$(APPNAME): $(OBJ)
	$(CC) $(OBJ) -o $(APPNAME) $(LDFLAGS)

$(OBJ): obj/%.o : src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: debug release
debug: CFLAGS+= -g -DDEBUG
release: CFLAGS+= -O3

debug release: clean $(APPNAME)

.PHONY:	clean
clean:
	rm obj/* -f
	rm $(APPNAME) -f

style: $(SRC) $(INC)
	astyle -A10 -s4 -S -p -xg -j -z2 -n src/* include/*
