
#ADD_SUBDIRECTORY(zlib)
#ADD_SUBDIRECTORY(lua)


SET(LIBLUA_SRCS_CORE lua/src/lapi.c lua/src/lcode.c lua/src/ldebug.c lua/src/ldo.c lua/src/ldump.c lua lua/src/lfunc.c lua/src/lgc.c lua/src/llex.c lua/src/lmem.c lua/src/lobject.c lua/src/lopcodes.c lua/src/lparser.c lua/src/lstate.c lua/src/lstring.c lua/src/ltable.c lua/src/ltm.c lua/src/lundump.c lua/src/lvm.c lua/src/lzio.c)
SET(LIBLUA_SRCS_LIB lua/src/lauxlib.c lua/src/lbaselib.c lua/src/ldblib.c lua/src/liolib.c lua/src/lmathlib.c lua/src/loslib.c lua/src/ltablib.c lua/src/lstrlib.c lua/src/loadlib.c lua/src/linit.c)
SET(LIBLUA_SRCS ${LIBLUA_SRCS_CORE} ${LIBLUA_SRCS_LIB})
ADD_LIBRARY(liblua ${LIBLUA_SRCS})
SET_TARGET_PROPERTIES(liblua PROPERTIES COMPILE_FLAGS "")

ADD_EXECUTABLE(lua lua/src/lua.c)
TARGET_LINK_LIBRARIES(lua m liblua)
ADD_EXECUTABLE(luac lua/src/luac.c lua/src/print.c)
TARGET_LINK_LIBRARIES(luac m liblua)


#INCLUDE_DIRECTORIES(lua/src)

SET(MINI_SC_LIBS_ETC etc/md5.c)

ADD_LIBRARY(mini_sc_libs ${MINI_SC_LIBS_ETC})
TARGET_LINK_LIBRARIES(mini_sc_libs)



