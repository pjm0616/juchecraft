#!/usr/bin/make

DEBUG				?= 1

CROSSC				=

SRCS_LIBMPQ			= libs/libmpq/SComp/SComp.cpp libs/libmpq/SComp/SErr.cpp libs/libmpq/SComp/SMem.cpp libs/libmpq/SComp/crc32.c libs/libmpq/SComp/explode.c libs/libmpq/SComp/huffman.cpp libs/libmpq/SComp/implode.c libs/libmpq/SComp/wave.cpp libs/libmpq/SFmpqapi/MpqBlockTable.cpp libs/libmpq/SFmpqapi/MpqCrypt.cpp libs/libmpq/SFmpqapi/MpqHashTable.cpp libs/libmpq/SFmpqapi/SFUtil.cpp libs/libmpq/SFmpqapi/SFmpqapi.cpp libs/libmpq/SFmpqapi/windows.cpp
SRCS_UI				= ui/SCGameUI.cpp ui/sdl/SCGameUI_SDL.cpp ui/ncurses/SCGameUI_ncurses.cpp
SRCS				= libs/libmpqgrp/grp.cpp libs/luacpp/luacpp.cpp $(SRCS_UI) $(SRCS_LIBMPQ) SCCoordinate.cpp SCObject.cpp SCPlayer.cpp SCObjectList.cpp SCObjectPrototypes.cpp SCGame.cpp main.cpp
TARGET1				= mini_sc

DEFS				= -D_REENTRANT -fopenmp -D_FILE_OFFSET_BITS=64
LIBS				= -fopenmp ./libs/lua/src/liblua.a -lSDL -lSDL_ttf -lSDL_image -lSDL_gfx -lz -lbz2 -lncursesw
INCLUDEDIR			= -I. -Ilibs/lua/src -Ilibs -Ilibs/libmpq/SFmpqapi
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
						-std=gnu99 -finline-functions -Wall -Wextra -Wno-unused -Wno-unused-function #-Wextra -Wshadow
CXXFLAGS			= $(DEFS) $(CXXFLAGS_DBG) $(INCLUDEDIR) \
						-std=gnu++98 -finline-functions -Wall -Wextra -Wno-unused -Wno-unused-function #-Wextra -Wshadow
LDFLAGS				= $(LDFLAGS_DBG) $(LIBDIR)
#OBJS				=$(SRCS:.cpp=.o) 
OBJS_TMP				=$(SRCS:.c=.o) 
OBJS				=$(OBJS_TMP:.cpp=.o) 


all:	lua resources $(TARGET1)

.SUFFIXES: .c .o
.c.o:
	@echo CC $<
	@@$(CC) -c $(CFLAGS) -o $@ $<

.SUFFIXES: .cpp .o
.cpp.o:
	@echo CXX $<
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(TARGET1):     $(OBJS)
	@echo LD $@
	@$(LD) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

lua:
	make -C ./libs/lua/src linux
luaclean:
	make -C ./libs/lua/src clean

resources:
	make -C res_raw
resclean:
	make -C res_raw clean

doc:
	doxygen ./Doxyfile
docclean:
	rm -rf ./docs/html ./docs/latex

clean:
	rm -f $(OBJS)
	rm -f $(TARGET1)
	
distclean: luaclean resclean clean docclean
	rm -f .depend

dep:    depend

depend:
	$(CC) -MM $(CXXFLAGS) $(SRCS) 1>.depend

depclean:
	$(RM) .depend



ifneq ($(wildcard .depend),)
include .depend
endif

