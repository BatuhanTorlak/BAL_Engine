CC=gcc

CFLAGS=-w -O3

GLOBAL_FILES=src/colormap/color.c src/colormap/colormap.c src/math/point.c src/math/vector2.c src/math/vector3.c src/ui/events.c src/ui/component.c
WIN_FILES=src/ui/windows/wui.c src/thread/windows/W_thread.c
LIN_FILES=src/ui/linux/lui.c src/thread/linux/L_thread.c

compilewin:
	$(CC) -c $(GLOBAL_FILES) $(WIN_FILES) $(CFLAGS)
	ar rcs build/Win/libwbale.a *.o
	rm *.o

compilelin:
