WIN_FLAGS = -lgdi32
LIN_FLAGS = -libX11

FILES = src/colormap/color.h
FILES += src/colormap/colormap.h
FILES += src/colormap/color.c
FILES += src/colormap/colormap.c

FILES += src/math/math.h
FILES += src/math/point.h
FILES += src/math/vector2.h
FILES += src/math/vector3.h
FILES += src/math/math.c
FILES += src/math/point.c
FILES += src/math/vector2.c
FILES += src/math/vector3.c

clean:

compile:
	ifeq ($(OS), Windows_NT)
		FILES += src/ui/windows/wui.h
		FILES += src/ui/ui.h
		FILES += src/ui/windows/wui.c
		FILES += src/ui/ui.c
		gcc $(FILES) $(WIN_FLAGS) -o /build/BALE.o
	else
		FILES += src/ui/linux/lui.h
		FILES += src/ui/ui.h
		FILES += src/ui/ui.c
		gcc $(FILES) $(LIN_FLAGS) -o /build/BALE.o