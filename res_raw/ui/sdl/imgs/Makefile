
SRCS = $(shell find -type f -name '*.png') ../../../../tools/grp_conv/scripts/convertscript.lua

../../../../res/ui/sdl/imgs/.timestamp: $(SRCS)
	../../../../tools/grp_conv/convert_grp ../../../../tools/grp_conv/scripts/convertscript.lua
	touch $@

clean:
	rm -f ../../../../res/ui/sdl/imgs/.timestamp
	find ../../../../res/ui/sdl/imgs -name '*.dat' -exec rm {} \;


