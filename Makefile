DBGEXE:=RCT-debug
RELEXE:=RCT-release
CONFIG:=debug
OS?=LIN

ifeq ($(OS),LIN)
    LDFLAGS:= -L raylib/linux/lib -lm -lraylib -lraygui -lX11 -ldl -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio -pthread
endif
ifeq ($(OS),WIN)
	LDFLAGS+= -L opencv/windows/lib -L raylib/windows/lib -lm -lraylib -lraygui -lopencv_core455 -lopencv_highgui455 -lopencv_imgcodecs455 -lopencv_imgproc455 -lopencv_videoio455 -lopengl32 -pthread -lgdi32 -lwinmm
endif

CFLAGS:= -pedantic -Wall -Wextra -Wno-missing-field-initializers -std=c++17 -I include -I raylib/include
ifeq ($(OS),LIN)
    CFLAGS+= -I /usr/include/opencv4 -DOS_LIN
	DBGBINDIR:=bin/linux/debug
    RELBINDIR:=bin/linux/release
	DBGOBJDIR:=obj/linux/debug
    RELOBJDIR:=obj/linux/release
	CC=g++
endif
ifeq ($(OS),WIN)
    CFLAGS+= -I opencv/windows/include -DOS_WIN
	DBGBINDIR:=bin/windows/debug
    RELBINDIR:=bin/windows/release
	DBGOBJDIR:=obj/windows/debug
    RELOBJDIR:=obj/windows/release
	CC=x86_64-w64-mingw32-g++
endif

ifeq ($(CAMERA),OFF)
	CFLAGS+= -DNO_CAMERA
endif

SRC:=$(wildcard src/*.cpp)
INC:=$(wildcard include/*.hpp)
DBGOBJ:=$(SRC:src/%.cpp=$(DBGOBJDIR)/%.o)
RELOBJ:=$(SRC:src/%.cpp=$(RELOBJDIR)/%.o)

.PHONY: all debug release clean style

all: debug release

debug:CONFIG:=debug
debug:CFLAGS+= -g -DDEBUG

release:CONFIG:=release
release: CFLAGS+= -O3

debug: $(DBGOBJ)
	$(CC) $(DBGOBJ) -o $(DBGBINDIR)/$(DBGEXE) $(LDFLAGS)

release: $(RELOBJ)
	$(CC) $(RELOBJ) -o  $(RELBINDIR)/$(RELEXE) $(LDFLAGS)

$(DBGOBJDIR)/%.o : src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(RELOBJDIR)/%.o : src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm obj/linux/debug/* obj/linux/release/* obj/windows/debug/* obj/windows/release/* bin/linux/debug/$(DBGEXE) bin/linux/release/$(RELEXE) bin/windows/debug/$(DBGEXE) bin/windows/release/$(RELEXE) -f