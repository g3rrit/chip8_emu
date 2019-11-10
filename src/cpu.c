#include "cpu.h"

#define MEM_SIZE 4096

struct cpu_t {
  // MEMORY
  uint8_t mem[MEM_SIZE];

  // REGISTERS
  uint8_t v[16];

  // INDEX REGISTER
  uint16_t i;

  // STACK
  uint16_t stack[16];
  uint16_t sp;

  // PROGRAM COUNTER
  uint16_t pc;
} cpu;

void cpu_reset() {

  for (uint32_t i = 0; i < MEM_SIZE; i++) {
    cpu.mem[i] = 0;
  }
  for (uint32_t i = 0; i < 16; i++) {
    cpu.v[i] = 0;
    cpu.stack[i] = 0;
  }
  cpu.i = 0; 
  cpu.pc = 0;
  cpu.sp = 0;
}

void cpu_load(char *path) {

  FILE *file = fopen(path, "rb");
  if (!file) {
    error("unable to open file %s\n", path);
  }

  fread(cpu.mem + 0x200, MEM_SIZE - 0x200, 1, file);

  fclose(file);
}

void cpu_spin() {
  
  // FETCH
  
  // DECODE 
  
  // EXECUTE

}


