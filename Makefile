#!/usr/bin/make

DEBUG				?= 1

CROSSC				=

SRCS_UI				= ui/SCUserInterface.cpp ui/SCUserInterface_ncurses.cpp ui/SCUserInterface_SDL.cpp
SRCS				= $(SRCS_UI) SCCoordinate.cpp SCObject.cpp SCPlayer.cpp SCObjectList.cpp SCGame.cpp main.cpp
TARGET1				= mini_sc

DEFS				= -D_FILE_OFFSET_BITS=64
LIBS				= -lncursesw -lSDL -lSDL_ttf -lSDL_image -lSDL_gfx
INCLUDEDIR			= -I.
LIBDIR				=

ifeq ($(DEBUG),1)
DEFS_DBG			= -DDEBUG 
CFLAGS_DBG			= $(DEFS_DBG) -g -O0 
CXXFLAGS_DBG		= $(CFLAGS_DBG) 
else
DEFS_DBG			= -DNDEBUG
CFLAGS_DBG			= $(DEFS_DBG) -O3
CXXFLAGS_DBG		= $(CFLAGS_DBG) 
endif




CC					=$(CROSSC)gcc
CXX					=$(CROSSC)g++
LD					=$(CROSSC)g++
AR					=$(CROSSC)ar
AS					=$(CROSSC)as
STRIP				=$(CROSSC)strip
OBJDUMP				=$(CROSSC)objdump

RM					=rm
LN_S				=ln -s
CP					=cp
MV					=mv
TAR					=tar
ZIP					=zip
UNZIP				=unzip

CFLAGS				= $(DEFS) $(CFLAGS_DBG) $(INCLUDEDIR) \
						-std=gnu99 -Wall -Wextra -Wno-unused -Wno-unused-function #-Wextra -Wshadow
CXXFLAGS			= $(CXXFLAGS_DBG) $(INCLUDEDIR) \
						-std=gnu++98 -Wall -Wextra -Wno-unused -Wno-unused-function #-Wextra -Wshadow
LDFLAGS				= $(LDFLAGS_DBG) $(LIBDIR) $(LIBS) 
OBJS				=$(SRCS:.cpp=.o) 


all:    $(TARGET1)

.SUFFIXES: .c .o
.c.o:
	@echo CC $<
	@$(CC) -c $(CFLAGS) -o $@ $<

.SUFFIXES: .cpp .o
.cpp.o:
	@echo CXX $<
	@$(CXX) -c $(CXXFLAGS) -o $@ $<

$(TARGET1):     $(OBJS)
	@echo LD $@
	@$(LD) -o $@ $(LDFLAGS) $(OBJS) 

doc:
	doxygen ./Doxyfile

docclean:
	rm -rf ./docs/html ./docs/latex

clean:
	rm -f $(OBJS)
	rm -f $(TARGET1)
	
distclean: clean docclean
	rm -f .depend

dep:    depend

depend:
	$(CC) -MM $(CXXFLAGS) $(SRCS) 1>.depend

depclean:
	$(RM) .depend



ifneq ($(wildcard .depend),)
include .depend
endif

