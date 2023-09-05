#include <SDL.h>

// First objective for visualizer: visualize n element in a tree

void visualize_tree(void *data, int n) {
  // find log n
  int logn = 1, x = n;
  while (x > 0) {
    logn++;
    x >>= 1;
  }
  int width = (1 << (logn - 2)) * 3;
  printf("LEVELS:%d widht:%d\n", --logn, width);
  // now there is an heigh
  int line_height = 800 / logn;

  int lst_of_lvl = 1;
  // padding must start from logn*f down to 0, f is a factor of distance
  int fa = 1;
  //
  // A way to see this is that the width is occupied in each level by the number
  // of nodes _centered_ and _spaced_ in a regular way.
  // On the first level there is 1 node, so the width is divided by 2.
  // On level 2 there are 2 nodes, the width is divided by 4 and nodes occupy
  // pos 1 and 3.
  // On Level 3, 4 nodes, so divide width by 8 and occupy place with odd idx
  // Nodes 8 => by 16, occupy the odds.
  // ... and so on. The point is to occupy the odds
  while (lst_of_lvl <= n) {
    // for (int i = 0; i < padding * fa; i++) printf(" ");
    //   in this level there are lst_of_lvl-1 nodes
    //   printf("%d %d -- ", lst_of_lvl / 2, lst_of_lvl);
    int first = lst_of_lvl - 1;  // (2^n)-1 -> 2^(n+1)-1
    int spacing = width / (lst_of_lvl);
    for (int j = 0; j < spacing; j++) printf(" ");
    for (int i = first; i < first + lst_of_lvl && i < n; i++) {
      if (i > first)
        for (int j = 0; j < spacing * 2 - 2; j++) printf(" ");
      printf("%03d", i);
    }
    printf("\t%d\n", spacing);
    lst_of_lvl = lst_of_lvl ? lst_of_lvl <<= 1 : 1;
    spacing >>= 1;
  }
}

void draw_node_rect(SDL_Renderer *renderer, int x, int y, void *data) {
  // this draw a rectangle in the screen at the middle top position x,y
  // (and eventually render text and other provided in data, TBD howto)
  // Let say Rect width is 100
  int width = 40;
  SDL_Rect r;
  r.h = 40;
  r.w = 20;
  r.x = x - 10;
  r.y = y;
  SDL_RenderDrawRect(renderer, &r);
}

void draw_tree(SDL_Renderer *renderer, int n) {
  // this draw the structure
  int logn = 1, x = n;
  while (x > 0) {
    logn++;
    x >>= 1;
  }
  // int width = (1 << (logn - 2)) * 3;
  int width = 900;
  printf("LEVELS:%d widht:%d\n", --logn, width);
  // now there is an heigh
  int line_height = 400 / logn;

  int lst_of_lvl = 1;
  int why = 400;
  while (lst_of_lvl <= n) {
    // for (int i = 0; i < padding * fa; i++) printf(" ");
    //   in this level there are lst_of_lvl-1 nodes
    //   printf("%d %d -- ", lst_of_lvl / 2, lst_of_lvl);
    int first = lst_of_lvl - 1;  // (2^n)-1 -> 2^(n+1)-1
    // Divide the width by number of position required to show the nodes
    // for that level.
    // Only odd-positions (position with odd index) are occupied by a node.
    // i.e.:
    // first level: divide by 2, occupy 1st pos (at index 1)
    // second level: divide by 4, occupy 1st and 3rd pos
    // third level: divide by 8, occupy 1st, 3rd, 5th and 7th pos
    // etc. So if level has n node, it needs n*2 position (or n<<1).
    // Padding. By this schema, last level will have the maximum number
    // of nodes in a raw
    int spacing = width / (lst_of_lvl << 1);
    printf("SPACING::: %d\n", spacing);
    int ex = spacing;
    for (int i = first; i < first + lst_of_lvl && i < n; i++) {
      // if (i > first)
      // for (int j = 0; j < spacing * 2 - 2; j++) printf(" ");
      draw_node_rect(renderer, ex, why, NULL);
      printf("DRAW RECT %d %d\n", ex, why);
      ex += (spacing * 2);
      // printf("%03d", i);
      if (i > 0) {
        // in this case, link the drawn node to the higher level
        // link_nodes(ex, why);
        printf("it is anotherline %d\n", i);
      }
    }
    printf("\t%d\n", spacing);
    lst_of_lvl = lst_of_lvl ? lst_of_lvl <<= 1 : 1;
    spacing >>= 1;
    why += line_height;
  }
}

int main(int argc, char **argv) {
  int nodes = 33;
  if (argc > 1) {
    int n = atoi(argv[1]);
    if (n) nodes = n;
    printf("ARG: %s\n", argv[1]);
  }
  visualize_tree(NULL, nodes);
  // return 1;
  //  this use sdl to visualize something
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }
  SDL_Window *window =
      SDL_CreateWindow("Visualize HEAP", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1000, 800, 0);
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
  // draw_node_rect(renderer, 400, 400, NULL);
  draw_tree(renderer, nodes);
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