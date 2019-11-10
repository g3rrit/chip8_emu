#ifndef GFX_H
#define GFX_H

#include "util.h"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

void gfx_init();

void gfx_delete();

void gfx_set_dim(uint32_t w, uint32_t h);

// renders the current screen to and sdl window
void gfx_render_present();

// sets the specified pixel and returns the xor
uint8_t gfx_set(uint8_t x, uint8_t y);

#endif
