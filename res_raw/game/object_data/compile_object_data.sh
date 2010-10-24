#!/bin/sh

outfile="$1"

../../../libs/lua/src/lua init.lua | ../../../libs/lua/src/luac -o $outfile -

exit $?

