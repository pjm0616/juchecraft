#!/usr/bin/make

DEBUG				?= 1
ENABLE_PROFILING 	?= 0

CROSSC				=

SRCS_UI				= ui/SCGameUI.cpp ui/sdl/SCGameUI_SDL.cpp ui/ncurses/SCGameUI_ncurses.cpp
SRCS				= libs/jcimg/jcimg.cpp libs/luacpp/luacpp.cpp SCCoordinate.cpp SCObject.cpp SCPlayer.cpp SCObjectList.cpp SCObjectPrototypes.cpp SCGame.cpp main.cpp $(SRCS_UI) 
TARGET1				= mini_sc

DEFS				= -D_REENTRANT -fopenmp -D_FILE_OFFSET_BITS=64 -DBOOST_NO_RTTI
LIBS				= -fopenmp ./libs/lua/src/liblua.a -lSDL -lSDL_ttf -lSDL_image -lSDL_gfx -lz -lbz2 -lncursesw
INCLUDEDIR			= -I. -Ilibs/lua/src -Ilibs
LIBDIR				=

ifeq ($(DEBUG),1)
DEFS_DBG			= -DDEBUG
#CFLAGS_DBG			= $(DEFS_DBG) -g -O0
CFLAGS_DBG			= $(DEFS_DBG) -g -O2 -fno-omit-frame-pointer
CXXFLAGS_DBG		= $(CFLAGS_DBG)
LDFLAGS_DBG			=
ifeq ($(ENABLE_PROFILING),1)
C_CXX_LD_FLAGS_PROF	= -pg
endif

else
DEFS_DBG			= -DNDEBUG
CFLAGS_DBG			= $(DEFS_DBG) -O3
CXXFLAGS_DBG		= $(CFLAGS_DBG) 
LDFLAGS_DBG			=
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

CFLAGS				= $(DEFS) $(CFLAGS_DBG) $(C_CXX_LD_FLAGS_PROF) $(INCLUDEDIR) \
						-std=gnu99 -finline-functions -Wall -Wextra -Wno-unused -Wno-unused-function #-Wextra -Wshadow
CXXFLAGS			= $(DEFS) $(CXXFLAGS_DBG) $(C_CXX_LD_FLAGS_PROF) $(INCLUDEDIR) \
						-std=gnu++98 -finline-functions -Wall -Wextra -Wno-unused -Wno-unused-function -fno-rtti #-Wextra -Wshadow
LDFLAGS				= $(LDFLAGS_DBG) $(C_CXX_LD_FLAGS_PROF) $(LIBDIR)
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

