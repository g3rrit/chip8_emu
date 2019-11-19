#include "gfx.h"

uint8_t screen[SCREEN_WIDTH * SCREEN_HEIGHT];

void screen_clear() {
  for (uint32_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
    screen[i] = 0;
  }
}

uint32_t window_width = 600;
uint32_t window_height = 600;

SDL_Event event;
SDL_Renderer *renderer;
SDL_Window *window;

#define CHECK(r) {          \
  if (r) {                  \
    panic(SDL_GetError());  \
  }                         \
}

void gfx_init() {
  CHECK(SDL_Init(SDL_INIT_VIDEO));
  CHECK(SDL_CreateWindowAndRenderer(window_width, window_height, SDL_WINDOW_RESIZABLE, &window, &renderer));

  screen_clear();
}

void gfx_delete() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void gfx_set_dim(uint32_t w, uint32_t h) {
  window_width = w;
  window_height = h;
}

void gfx_render_present() {

  CHECK(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0));
  CHECK(SDL_RenderClear(renderer));

  uint32_t dw = window_width;
  uint32_t dh = window_height;

  printf("width: %d height: %d\n", dw, dh);

  //double sf = dw > dh ? dh / SCREEN_HEIGHT : dw / SCREEN_WIDTH;
  double sf = dw / (double) SCREEN_WIDTH;

  uint32_t offx = (dw - (SCREEN_WIDTH * sf)) / 2.0;
  uint32_t offy = (dh - (SCREEN_HEIGHT * sf)) / 2.0;

  if (sf <= 1.0) {
    offx = 0;
    offy = 0;
    sf = 1;
  }

  uint32_t cx = offx;
  uint32_t cy = offy;
  double dxf = 0;
  double dyf = 0;
  for (uint32_t y = 0; y < SCREEN_HEIGHT; y++) {

    dyf += sf;
    while (dyf >= 1) {

      for (uint32_t x = 0; x < SCREEN_WIDTH; x++) {

        printf("off: (%d, %d) sf: %f pixel: (%d, %d)\n", offx, offy, sf, cx, cy);

        if (screen[y * SCREEN_HEIGHT + x]) {
          CHECK(SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255));
        } else {
          CHECK(SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255));
        }
      
        dxf += sf;
        while (dxf >= 1) {
          CHECK(SDL_RenderDrawPoint(renderer, cx, cy));
          cx += 1;
          dxf -= 1;
        }
      }

      cx = 0;
      dxf = 0;
      cy += 1;
      dyf -= 1;
    }
  }

  SDL_RenderPresent(renderer);
}

uint8_t gfx_set(uint8_t x, uint8_t y) {
  if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return 0;
  return screen[y * SCREEN_WIDTH + x] ^= 1;
}




