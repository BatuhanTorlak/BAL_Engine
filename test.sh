gcc src/ui/events.h src/ui/events.c src/colormap/colormap.h src/colormap/colormap.c src/ui/windows/wui.h src/ui/windows/wui.c src/ui/ui.h src/main.c -w -lgdi32 -O3 -o main.exe
if [ -e main.exe ]
then
    ./main.exe
fi