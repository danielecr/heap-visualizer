.PHONY: all
# CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99 -g
CFLAGS=-std=c99 -g
SDLFLAGS=$(sdl2-config --cflags --libs)

all: testheap visualizer serve/data/visualizer-wasm.html

testheap: heap.o testheap.c
	$(CC) $(CFLAGS) -o testheap heap.o testheap.c

visualizer.o: visualizer.c
	$(CC) -c visualizer.c `sdl2-config --cflags --libs`

visualizer: visualizer.o heap.o
	$(CC) -o visualizer visualizer.o heap.o `sdl2-config --cflags --libs` -lSDL2_ttf

heap-wasm.o: heap.c
	emcc -c heap.c -o heap-wasm.o

visualizer-wasm.o: visualizer-wasm.c
	emcc -c visualizer-wasm.c -o visualizer-wasm.o -s USE_SDL=2 -s USE_SDL_TTF=2

serve/data/visualizer-wasm.html: visualizer-wasm.o heap-wasm.o
	emcc visualizer-wasm.o heap-wasm.o -o serve/data/visualizer-wasm.html -sUSE_SDL=2 -sUSE_SDL_TTF=2 --preload-file Resources --use-preload-plugins