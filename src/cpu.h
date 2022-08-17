#ifndef CPU_H
#define CPU_H

#include "util.h"

void cpu_reset();

void cpu_load(char * path);

// returns 1 if graphics should be updated
int cpu_spin();

#endif
