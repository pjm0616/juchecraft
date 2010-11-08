#!/usr/bin/make

DEBUG				?= 1
ENABLE_PROFILING 	?= 0

# 0: don't use, 1: generate, 2: use
ENABLE_PGO			?= 0
PGO_DIR			= ./tmp_profile/
#########

CROSSC				=

SRCS_UI				= ui/SCGameUI.cpp ui/sdl/SCGameUI_SDL.cpp ui/ncurses/SCGameUI_ncurses.cpp
SRCS				= libs/etc/md5.c libs/jcimg/jcimg.cpp libs/luacpp/luacpp.cpp SCCoordinate.cpp SCObject.cpp SCPlayer.cpp SCObjectList.cpp SCObjectFactory.cpp SCGame.cpp main.cpp $(SRCS_UI) 
TARGET1				= mini_sc

DEFS				= -D_REENTRANT -fopenmp -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE=1 -DBOOST_NO_RTTI
LIBS				= -fopenmp ./libs/lua/src/liblua.a -lSDL -lSDL_ttf -lSDL_image -lSDL_gfx -lz -lbz2 -lncursesw
INCLUDEDIR			= -I. -Ilibs/lua/src -Ilibs
LIBDIR				=

ifeq ($(DEBUG),1)
DEFS_DBG			= -DDEBUG
CFLAGS_DBG			= $(DEFS_DBG) -g -O0 -fstack-protector-all -fstrict-aliasing
#CFLAGS_DBG			= $(DEFS_DBG) -g -O2 -fstack-protector-all -fno-omit-frame-pointer
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

ifeq ($(ENABLE_PGO),1)
C_CXX_LD_FLAGS_PGO	= --coverage -fprofile-dir=$(PGO_DIR) -fprofile-generate #-fprofile-correction
else
ifeq ($(ENABLE_PGO),2)
C_CXX_LD_FLAGS_PGO	= --coverage -fprofile-dir=$(PGO_DIR) -fprofile-use -Wcoverage-mismatch
endif
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

CFLAGS				= $(DEFS) $(CFLAGS_DBG) $(C_CXX_LD_FLAGS_PROF) $(C_CXX_LD_FLAGS_PGO) $(INCLUDEDIR) \
						-std=gnu99 -finline-functions -Wall -Wextra -Wno-unused-parameter -Wshadow
CXXFLAGS			= $(DEFS) $(CXXFLAGS_DBG) $(C_CXX_LD_FLAGS_PROF) $(C_CXX_LD_FLAGS_PGO) $(INCLUDEDIR) \
						-std=gnu++98 -finline-functions -Wall -Wextra -Wno-unused-parameter -fno-rtti -Wshadow -Wno-unused
LDFLAGS				= $(LDFLAGS_DBG) $(C_CXX_LD_FLAGS_PROF) $(C_CXX_LD_FLAGS_PGO) $(LIBDIR)
#OBJS				=$(SRCS:.cpp=.o) 
OBJS_TMP				=$(SRCS:.c=.o) 
OBJS				=$(OBJS_TMP:.cpp=.o) 




.PHONY:	all libs libclean tools toolclean resources resclean doc docclean clean distclean dep depclean

all:	dep libs $(TARGET1) tools resources

.SUFFIXES: .c .o
.c.o:
	@echo CC $<
	$(CC) -c $(CFLAGS) -o $@ $<

.SUFFIXES: .cpp .o
.cpp.o:
	@echo CXX $<
	$(CXX) -c $(CXXFLAGS) -o $@ $<

# FIXME: mini_sc is relinked every time. it doesn't happen if there's not `libs'
$(TARGET1):	libs $(OBJS)
	@echo LD $@
	$(LD) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

libs:
	$(MAKE) -C ./libs
libclean:
	$(MAKE) -C ./libs clean

tools: libs $(TARGET1)
	$(MAKE) -C tools
toolclean:
	$(MAKE) -C tools clean

resources: libs tools
	$(MAKE) -C res_raw
resclean:
	$(MAKE) -C res_raw clean

doc:
	doxygen ./Doxyfile
docclean:
	rm -rf ./docs/html ./docs/latex

clean:
	rm -f $(OBJS)
	rm -f $(TARGET1)
	
distclean: libclean resclean clean toolclean docclean
	rm -f .depend

dep:	.depend

.depend:
	$(CC) -MM $(CXXFLAGS) $(SRCS) 1>.depend

depclean:
	$(RM) .depend



ifneq ($(wildcard .depend),)
include .depend
endif

