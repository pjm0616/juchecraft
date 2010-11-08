#!/bin/sh

find . ui ui/* libs/jcimg libs/luacpp/ res_raw/ tools/libs/libmpqgrp/ tools/grp_conv/ \
	 -maxdepth 1 -name '*.cpp' -o -name '*.c' -o -name '*.h'|xargs wc -l

