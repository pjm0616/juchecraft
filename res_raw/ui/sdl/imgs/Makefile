
SRCS = $(shell find -type f -name '*.png')

../../../../res/ui/sdl/imgs/.timestamp: $(SRCS)
	../../../../tools/grp_conv/convert_grp ../../../../tools/grp_conv/scripts/convertscript.lua
	touch $@

clean:
	find ../../../../res/ui/sdl/imgs -name '*.dat' -exec rm {} \;


