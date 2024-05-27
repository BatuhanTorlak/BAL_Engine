CC=gcc

WIN_FLAGS = -lgdi32
LIN_FLAGS = -libX11

FILES = src/colormap/color.h
FILES += src/colormap/colormap.h
FILES += src/colormap/color.c
FILES += src/colormap/colormap.c

FILES += src/ui/windows/wui.h
FILES += src/ui/ui.h

FILES += src/math/point.h
FILES += src/math/vector2.h
FILES += src/math/vector3.h
FILES += src/math/point.c
FILES += src/math/vector2.c
FILES += src/math/vector3.c

clean:
ifeq ($(OS), Windows_NT)
	rm /build/Win/BALE.exe
else
	rm /build/Lin/BALE.o
endif

compile:
ifeq ($(OS), Windows_NT)
	$(FILES += src/ui/windows/wui.c)
	$(FILES += src/ui/ui.c)
	@echo $(FILES)
	$(CC) $(FILES) $(CustomFiles) -w $(WIN_FLAGS) -O3 -o /build/Win/BALE.exe
else
	FILES+=" src/ui/linux/lui.h"
	$(CC) $(FILES) $(CustomFiles) -w $(LIN_FLAGS) -O3 -o /build/Lin/BALE.o
endif

run:
ifeq ($(OS), Windows_NT)
	start /build/Win/BALE.exe
else
	/build/Lin/BALE.o
endif
