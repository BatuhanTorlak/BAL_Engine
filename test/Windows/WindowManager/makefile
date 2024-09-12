all: compile run

compile:
	gcc main.c -IBALE -LBALE -lwbale -lgdi32 -m64 -g -o main.exe

run: main.exe
	./main.exe