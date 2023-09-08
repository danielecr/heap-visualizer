#include <SDL.h>
#include <SDL_ttf.h>

#include "heap.h"

// First objective for visualizer: visualize n element in a tree

typedef struct {
  SDL_Renderer *renderer;
  int x;
  int y;
  int w;
  int h;
  int box_h;
  int box_w;
  int n_lines;
  TTF_Font *font;
} VHeapContext;

VHeapContext *vheap_context_create(SDL_Renderer *renderer, int x, int y, int w,
                                   int h, int box_h, int box_w, int n_lines,
                                   TTF_Font *font) {
  // this take renderer, x, y, width, height
  VHeapContext *ctx = (VHeapContext *)malloc(sizeof(VHeapContext));
  *ctx = (VHeapContext){
      .renderer = renderer,
      .x = x,
      .y = y,
      .w = w,
      .h = h,
      .box_h = box_h,
      .box_w = box_w,
      .n_lines = n_lines,
      .font = font,
  };
  return ctx;
}

// vheap_context_arange_node(VHeapContext* ctx, )

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

void render_content(VHeapContext *ctx, char *content, int x, int y) {
  SDL_Color col = {.r = 255, .g = 255, .b = 255, .a = 255};
  SDL_Surface *sur = TTF_RenderText_Blended(ctx->font, content, col);
  SDL_Rect r = {
      .x = x,
      .y = y,
      .w = sur->w,
      .h = sur->h,
  };
  SDL_Texture *tex = SDL_CreateTextureFromSurface(ctx->renderer, sur);
  SDL_FreeSurface(sur);
  SDL_RenderCopy(ctx->renderer, tex, NULL, &r);

  SDL_DestroyTexture(tex);
}

void vheap_draw_node(VHeapContext *ctx, int line, int slots, int pos,
                     char *content) {
  // now this function draw a node according to context and calculate coords
  // based on line, slots (in a line) and pos
  int calc_x = ctx->w / slots * pos;
  SDL_Rect r1, r;
  r1.h = ctx->box_h;
  r1.w = ctx->box_w;
  r1.x = ctx->x + calc_x - ctx->box_w / 2;
  r1.y = ctx->y + (ctx->h / ctx->n_lines) * line;
  SDL_RenderDrawRect(ctx->renderer, &r1);
  render_content(ctx, content, r1.x + 2, r1.y + 1);
  // return;
}

void vheap_draw_link(VHeapContext *ctx, int to_line, int slots, int pos) {
  // draw a link edge between node to_line pos, from the parent node.
  // Parent node is easily calculated by
  //     _       _
  // 0, ,1, ,2, ,3
  // 0,1,2,3,4,5,6,7
  //   -   -   -   -
  // pos == 1 => parent_pos ==1
  // pos == 3 => parent_pos ==1
  // pos == 5 => parent_pos ==3
  // pos == 7 => parent_pos ==3

  //   -   -   -   -
  // pos == 0 => parent_pos ==0
  // pos == 2 => parent_pos ==0
  // pos == 4 => parent_pos ==2
  // pos == 6 => parent_pos ==2

  // the logical position of child is (pos-1)/2 (i.e.: 0,1,2,3)
  // parent of 0 and 1 is (logical) 0, parent of 2 and 3 is (logical) 1
  // logical 0 == 1, logical 1 == 3
  // int logi_child_p = ((pos - 1) >> 1);
  // int logi_parent_p = logi_child_p >> 1;
  // int parent_pos = (logi_parent_p << 1) + 1;
  int parent_pos = (((pos - 1) >> 2) << 1) + 1;

  int parent_slots = slots >> 1;
  int parent_x = ctx->x + ctx->w / parent_slots * parent_pos;
  int parent_y = ctx->y + (ctx->h / ctx->n_lines) * (to_line - 1) + ctx->box_h;
  // printf("%d %d %d %d\n", parent_pos, parent_slots, parent_x, parent_y);
  int calc_x = ctx->w / slots * pos;
  int target_x = ctx->x + calc_x;
  int target_y = ctx->y + (ctx->h / ctx->n_lines) * to_line;
  SDL_RenderDrawLine(ctx->renderer, parent_x, parent_y, target_x, target_y);
}

void draw_tree(SDL_Renderer *renderer, int n) {
  // this draw the structure
  int logn = 1, x = n;
  while (x > 0) {
    logn++;
    x >>= 1;
  }
  // int width = (1 << (logn - 2)) * 3;
  TTF_Font *font = TTF_OpenFont("Roboto-Bold.ttf", 13);

  VHeapContext *ctx =
      vheap_context_create(renderer, 50, 200, 1200, 600, 16, 20, logn, font);
  // SDL_RenderPresent(ctx->renderer);
  // render_content(ctx, "ciao");
  int width = 900;
  printf("LEVELS:%d widht:%d\n", --logn, width);
  // now there is an heigh
  int line_height = 400 / logn;

  int lst_of_lvl = 1;
  int line = 0;
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
    int pos = 1;
    for (int i = first; i < first + lst_of_lvl && i < n; i++) {
      // if (i > first)
      // for (int j = 0; j < spacing * 2 - 2; j++) printf(" ");
      char *content = (char *)malloc(sizeof(char) * 12);
      sprintf(content, "%d", i);
      vheap_draw_node(ctx, line, lst_of_lvl << 1, pos, content);
      // printf("DRAW RECT %d %d\n", ex, why);
      // printf("%03d", i);
      if (i > 0) {
        // in this case, link the drawn node to the higher level
        // Now it become evident the needs of a supporting object,
        // a kind of "context", I name it VHeapContext, and I add as many
        // information as it makes sense to hold in the context
        // link_nodes(ex, why);
        // printf("it is anotherline %d; line:%d lvl:%d pos:%d\n", i, line,
        //       lst_of_lvl, pos);
        vheap_draw_link(ctx, line, lst_of_lvl << 1, pos);
      }
      pos += 2;
    }
    line++;
    lst_of_lvl = lst_of_lvl ? lst_of_lvl <<= 1 : 1;
  }
  // render_content(ctx, "ciao");
}

void draw_heap(SDL_Renderer *renderer, Heap *h) {
  int n = h->size;
  // this draw the structure
  int logn = 1, x = h->size;
  while (x > 0) {
    logn++;
    x >>= 1;
  }
  // int width = (1 << (logn - 2)) * 3;
  TTF_Font *font = TTF_OpenFont("Roboto-Bold.ttf", 13);

  VHeapContext *ctx =
      vheap_context_create(renderer, 50, 200, 1200, 600, 16, 20, logn, font);
  // SDL_RenderPresent(ctx->renderer);
  // render_content(ctx, "ciao");
  int width = 900;
  printf("LEVELS:%d widht:%d\n", --logn, width);
  // now there is an heigh
  int line_height = 400 / logn;

  int lst_of_lvl = 1;
  int line = 0;
  char *content = (char *)malloc(sizeof(char) * 15);
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
    int pos = 1;
    for (int i = first; i < first + lst_of_lvl && i < n; i++) {
      // if (i > first)
      // for (int j = 0; j < spacing * 2 - 2; j++) printf(" ");
      sprintf(content, "%d", *((int *)h->data[i]));
      vheap_draw_node(ctx, line, lst_of_lvl << 1, pos, content);
      // printf("DRAW RECT %d %d\n", ex, why);
      // printf("%03d", i);
      if (i > 0) {
        // in this case, link the drawn node to the higher level
        // Now it become evident the needs of a supporting object,
        // a kind of "context", I name it VHeapContext, and I add as many
        // information as it makes sense to hold in the context
        // link_nodes(ex, why);
        // printf("it is anotherline %d; line:%d lvl:%d pos:%d\n", i, line,
        //       lst_of_lvl, pos);
        vheap_draw_link(ctx, line, lst_of_lvl << 1, pos);
      }
      pos += 2;
    }
    line++;
    lst_of_lvl = lst_of_lvl ? lst_of_lvl <<= 1 : 1;
  }
  free(content);
  // render_content(ctx, "ciao");
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
  TTF_Init();
  SDL_Window *window =
      SDL_CreateWindow("Visualize HEAP", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1300, 800, 0);
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
  int elem = nodes;
  Heap *h = heap_create(biggerthan, 100);
  int *values = (int *)malloc(elem * sizeof(int));
  for (int i = 0; i < elem; i++) {
    values[i] = rand();
  }
  for (int i = 0; i < 10 && i < elem; i++) {
    heap_insert(h, &values[i]);
  }
  heap_print(h, formatint);
  for (int i = 10; i < elem; i++) {
    heap_insert(h, &values[i]);
  }
  heap_print(h, formatint);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  draw_heap(renderer, h);
  // draw_tree(renderer, nodes);
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
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  heap_free(&h);
  free(values);
  return 0;
}