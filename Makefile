DBGEXE:=RCT-debug
RELEXE:=RCT-release
DBGOBJDIR:=obj/debug
RELOBJDIR:=obj/release
CONFIG:=debug

LDFLAGS:=-L raylib -lm -lraylib -lX11 -ldl -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio -pthread

CFLAGS:= -g -pedantic -Wall -Wextra -Wno-missing-field-initializers
CFLAGS+= -std=c++17 -I ./include -I raylib -I /usr/include/opencv4

SRC:=$(wildcard src/*.cpp)
INC:=$(wildcard include/*.hpp)
DBGOBJ:=$(SRC:src/%.cpp=obj/debug/%.o)
#DGBOBJ:=$(addprefix obj/debug/, $(DBGOBJ))
RELOBJ:=$(SRC:src/%.cpp=obj/release/%.o)

CC=g++

.PHONY: all debug release clean style

all: debug release

debug:CONFIG:=debug
debug:CFLAGS+= -g -DDEBUG

release:CONFIG:=release
release: CFLAGS+= -O3

debug: $(DBGOBJ)
	$(CC) $(DBGOBJ) -o $(DBGEXE) $(LDFLAGS)

release: $(RELOBJ)
	$(CC) $(RELOBJ) -o $(RELEXE) $(LDFLAGS)

$(DBGOBJDIR)/%.o : src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(RELOBJDIR)/%.o : src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

debug: $(DGBEXE)
release: $(RELEXE)

clean:
	rm obj/debug/* obj/release/* $(DBGEXE) $(RELEXE) -f

style: $(SRC) $(INC)
	astyle -A10 -s4 -S -p -xg -j -z2 -n src/* include/*
