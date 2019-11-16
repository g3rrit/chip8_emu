#include "util.h"
#include "gfx.h"
#include "input.h"
#include "cpu.h"

#define HZ 60

int main(int argc, char **argv) {

  cpu_reset();

  if (argc < 2) {
    printf("usage: chip8 rom.ch8\n");
    //return EXIT_SUCCESS;
  }

  //cpu_load(argv[1]);

  gfx_init();
  gfx_render_present();

  clock_t t0 = clock();
  clock_t t1;
  double dt = 0;
  SDL_Event event;
  for (;;) {

    // POLL EVENTS
    SDL_PollEvent(&event);
    switch (event.type) {
      case SDL_QUIT: {
        goto cleanup;
      }
      case SDL_WINDOWEVENT: {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
          gfx_set_dim(event.window.data1, event.window.data2);
          gfx_render_present();
        }
        break;
      }
      case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
          case SDLK_1: key_set(0x1); break;
          case SDLK_2: key_set(0x2); break;
          case SDLK_3: key_set(0x3); break;
          case SDLK_4: key_set(0xc); break;
          case SDLK_q: key_set(0x4); break;
          case SDLK_w: key_set(0x5); break;
          case SDLK_e: key_set(0x6); break;
          case SDLK_r: key_set(0xd); break;
          case SDLK_a: key_set(0x7); break;
          case SDLK_s: key_set(0x8); break;
          case SDLK_d: key_set(0x9); break;
          case SDLK_f: key_set(0xe); break;
          case SDLK_z: key_set(0xa); break;
          case SDLK_x: key_set(0x0); break;
          case SDLK_c: key_set(0xb); break;
          case SDLK_v: key_set(0xf); break;
        }
        break;
      }
      case SDL_KEYUP: {
        switch (event.key.keysym.sym) {
          case SDLK_1: key_reset(0x1); break;
          case SDLK_2: key_reset(0x2); break;
          case SDLK_3: key_reset(0x3); break;
          case SDLK_4: key_reset(0xc); break;
          case SDLK_q: key_reset(0x4); break;
          case SDLK_w: key_reset(0x5); break;
          case SDLK_e: key_reset(0x6); break;
          case SDLK_r: key_reset(0xd); break;
          case SDLK_a: key_reset(0x7); break;
          case SDLK_s: key_reset(0x8); break;
          case SDLK_d: key_reset(0x9); break;
          case SDLK_f: key_reset(0xe); break;
          case SDLK_z: key_reset(0xa); break;
          case SDLK_x: key_reset(0x0); break;
          case SDLK_c: key_reset(0xb); break;
          case SDLK_v: key_reset(0xf); break;
        }
        break;
      }
    }

    // SPIN CPU
    
    if(cpu_spin()) {
      gfx_render_present();
    }

    // ADJUST HZ

    t1 = clock();
    dt = (t1 - t0) / (double) CLOCKS_PER_SEC;

    if (dt < 1.0 / (double) HZ) {
      SDL_Delay((1.0 / (double) HZ) - dt);
    }

    t0 = t1;
  }

cleanup:

  gfx_delete();
  SDL_Quit();
  return EXIT_SUCCESS;
}
