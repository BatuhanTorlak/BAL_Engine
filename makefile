CC=gcc

CFLAGS=-w -m64 -O3

GLOBAL_FILES=src/colormap/color.c src/math/point.c src/math/vector.c src/ui/events.c src/ui/component.c
WIN_FILES=src/ui/windows/wui.c src/thread/windows/W_thread.c src/colormap/windows/colormap.c
LIN_FILES=src/ui/linux/lui.c src/thread/linux/L_thread.c src/colormap/linux/colormap.c

compilewin:
	$(CC) -c -IBALE $(GLOBAL_FILES) $(WIN_FILES) $(CFLAGS)
	ar rcs BALE/libwbale.a *.o
	rm *.o

compilelin:
