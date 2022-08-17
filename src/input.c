#include "input.h"

uint8_t keymap[16] = {0};

void key_set(uint8_t k)
{
    keymap[k & 0x0f] = 1;
}

void key_reset(uint8_t k)
{
    keymap[k & 0x0f] = 0;
}

uint8_t key_get(uint8_t k)
{
    return keymap[k & 0x0f];
}
