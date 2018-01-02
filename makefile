all: sand

sand: sand.c
	gcc sand.c -std=c99 -o sand `pkg-config --libs --cflags sdl2`
