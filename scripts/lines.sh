#!/bin/sh

find . ui tools libs/libmpqgrp/ libs/luacpp/ res_raw/ -maxdepth 1 -name '*.cpp' -o -name '*.c' -o -name '*.h'|xargs wc -l

