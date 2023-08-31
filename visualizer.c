#include <SDL.h>

int main() {
  // this use sdl to visualize something
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }
  SDL_Window *window =
      SDL_CreateWindow("Visualize HEAP", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 700, 800, 0);
  if (window == NULL) {
    printf("error %s\n", SDL_GetError());
    return 1;
  }
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    printf("Renderer error %s\n", SDL_GetError());
    return 1;
  }
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
  SDL_RenderDrawLine(renderer, 0, 0, 300, 300);
  SDL_RenderDrawLine(renderer, 0, 400, 700, 400);
  SDL_Rect x;
  // SDL_Rect *r = SDL_Rect
  // SDL_RenderDrawRect();
  SDL_RenderPresent(renderer);
  // SDL_RenderPresent(renderer);

  // SDL_Delay(2000);
  while (1) {
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        break;
      }
      if (e.type == SDL_MOUSEBUTTONDOWN) {
        SDL_MouseButtonEvent b = e.button;
        printf("%d %d\n", b.x, b.y);
        if (b.y > 400) break;
      }
    }
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}