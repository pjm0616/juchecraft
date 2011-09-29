
SRCS=$(shell find -type f -name '*.lua')

../../../res/game/object_data/objects.dat: $(SRCS)
#	./compile_object_data.sh ../../../res/game/object_data/objects.dat
	cp -a *.lua objects objects.dat ../../../res/game/object_data/


clean:
	rm -rf ../../../res/game/object_data/*



