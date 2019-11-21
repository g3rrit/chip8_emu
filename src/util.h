#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <time.h>
#include <SDL2/SDL.h>

#define DEBUG_F 1

#define DEBUG(f) (DEBUG_F ? f : (void)0 )


static inline void panic(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  printf("ERROR: ");
  vprintf(fmt, args);
  va_end(args);

  exit(-1);
}


#endif
