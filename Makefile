.PHONY: all
# CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99 -g
CFLAGS=-std=c99 -g
SDLFLAGS=$(sdl2-config --cflags --libs)

all: testheap visualizer

testheap: heap.o testheap.c
	$(CC) $(CFLAGS) -o testheap heap.o testheap.c

visualizer.o: visualizer.c
	$(CC) -c visualizer.c `sdl2-config --cflags --libs`

visualizer: visualizer.o
	$(CC) -o visualizer visualizer.o `sdl2-config --cflags --libs`
