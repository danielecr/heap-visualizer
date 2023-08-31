.PHONY: all
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99 -g
SDLFLAGS=$(sdl2-config --cflags --libs)

all: heap visualizer

heap: heap.o
	$(CC) $(CFLAGS) -o heap heap.o

visualizer.o: visualizer.c
	$(CC) -c visualizer.c `sdl2-config --cflags --libs`

visualizer: visualizer.o
	$(CC) -o visualizer visualizer.o `sdl2-config --cflags --libs`
