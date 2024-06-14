CC=gcc

WIN_FLAGS = -lgdi32
LIN_FLAGS = -libX11


ifeq ($(OS), Windows_NT)
	FILES +=src/ui/windows/wui.c
else
	FILES +=" src/ui/linux/lui.h"
endif

FILES+=src/ui/events.c
FILES+=src/ui/ui.c
FILES+=src/colormap/colormap.c

clean:
ifeq ($(OS), Windows_NT)
	rm ./build/Win/BALE.exe
else
	rm ./build/Lin/BALE.o
endif

compile:
ifeq ($(OS), Windows_NT)
	$(CC) $(FILES) $(CustomFiles) -w $(WIN_FLAGS) -O3 -o ./build/Win/BALE.exe
else
	$(CC) $(FILES) $(CustomFiles) -w $(LIN_FLAGS) -O3 -o /build/Lin/BALE.o
endif

run:
ifeq ($(OS), Windows_NT)
	start "./build/Win/BALE.exe"
else
	./build/Lin/BALE.o
endif


all: compile run
	