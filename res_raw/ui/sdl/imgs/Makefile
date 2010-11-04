
SRCS = $(shell find -type f -name '*.png')

all: $(SRCS)
	../../../../tools/grp_conv/convert_grp ../../../../tools/grp_conv/scripts/convertscript.lua

clean:
	find ../../../../res/ui/sdl/imgs -name '*.dat' -exec rm {} \;


