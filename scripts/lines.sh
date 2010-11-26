#!/bin/sh

srcs="`find ./ libs/jcimg/ libs/luacpp/ tools/libs/libmpqgrp/ tools/grp_conv/ -maxdepth 1 -name '*.cpp' -o -name '*.c' -o -name '*.h' -o -name '*.lua'`"
srcs="$srcs `find src/ res_raw/ -name '*.cpp' -o -name '*.c' -o -name '*.h' -o -name '*.lua'`"

wc $srcs


