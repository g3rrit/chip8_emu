#include "cpu.h"

#include "gfx.h"
#include "input.h"

#define MEM_SIZE 4096
#define STACK_SIZE 64

struct cpu_t {
  // MEMORY
  uint8_t mem[MEM_SIZE];

  // REGISTERS
  uint8_t v[16];

  // INDEX REGISTER
  uint16_t i;

  // STACK
  uint16_t stack[STACK_SIZE];
  uint16_t sp;

  // PROGRAM COUNTER
  uint16_t pc;

  // DELAY TIMER
  uint8_t dt;

  // SOUND TIMER
  uint8_t st;
} cpu;

#define FONTSET_SIZE 80
uint8_t fontset[FONTSET_SIZE] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void cpu_reset() {

  for (uint32_t i = 0; i < MEM_SIZE; i++) {
    cpu.mem[i] = 0;
  }
  for (uint32_t i = 0; i < 16; i++) {
    cpu.v[i] = 0;
    cpu.stack[i] = 0;
  }
  cpu.i = 0; 
  cpu.pc = 0x200;
  cpu.sp = 0;
  cpu.dt = 0;
  cpu.st = 0;

  // load fontset into memory
  for (uint32_t i = 0; i < FONTSET_SIZE; i++) {
    cpu.mem[i] = fontset[i];
  }
}

void cpu_load(char *path) {

  FILE *file = fopen(path, "rb");
  if (!file) {
    panic("unable to open file %s\n", path);
  }

  for (int i = 0; i < MEM_SIZE - 0x200; i++) {
    fread(cpu.mem + 0x200 + i, 1, MEM_SIZE - 0x200, file);
  }

  fclose(file);
}

static inline uint16_t push(uint16_t add) {
  printf("push\n");
  if (cpu.sp >= STACK_SIZE - 1) {
    panic("stack size exceeded\n");
  }
  cpu.stack[cpu.sp++] = cpu.pc;
  return add;
}

static inline uint16_t pop() {
  printf("pop\n");
  if (cpu.sp <= 0) {
    panic("unable to pop stack\n");
  }
  return cpu.stack[--cpu.sp];
}

static inline void draw_sprite(uint8_t x, uint8_t y, uint8_t n) {
  for (uint8_t i = 0; i < n; i++) {
    uint8_t l = cpu.mem[cpu.i + i];
    for (uint8_t b = 7; b >= 0; b--) {
      if ((l >> b) & 1) {
        if (!gfx_set(x + b, y + i)) cpu.v[0xf] = 1;
      }
    }
  }
}

int cpu_spin() {

#define GETO(n) ((op >> (4 * n)) & 0xf)

  int draw_flag = 0;

  // FETCH
  uint16_t op = cpu.mem[cpu.pc] << 8 | cpu.mem[cpu.pc + 1];
  printf("pc: %x op: %x\n", cpu.pc, op);
  cpu.pc += 2;

  
  // DECODE 
  // EXECUTE
  switch (GETO(3)) {
    case 0:
      if (op == 0x00e0) {
        // clear screen
        screen_clear();
      } else if (op == 0x00ee) {
        // return
        cpu.pc = pop();
      } else {
        // call 
        cpu.pc = push(op & 0x0fff);        
      }
      break;
    case 1: 
      // jump
      cpu.pc = op & 0x0fff;
      break;
    case 2: 
      // call
      cpu.pc = push(op & 0x0fff);
      break;
    case 3: 
      if (cpu.v[GETO(2)] == (op & 0x00ff)) {
        cpu.pc += 2;
      }
      break;
    case 4: 
      if (cpu.v[GETO(2)] != (op & 0x00ff)) {
        cpu.pc += 2;
      }
      break;
    case 5:
      if (cpu.v[GETO(2)] == cpu.v[GETO(1)]) {
        cpu.pc += 2;
      }
      break;
    case 6:
      cpu.v[GETO(2)] = op & 0x00ff;
      break;
    case 7:
      cpu.v[GETO(2)] += op & 0x00ff;
      break;
    case 8:
      switch (GETO(0)) {
        case 0:
          cpu.v[GETO(2)] = cpu.v[GETO(1)];
          break;
        case 1:
          cpu.v[GETO(2)] |= cpu.v[GETO(1)];
          break;
        case 2:
          cpu.v[GETO(2)] &= cpu.v[GETO(1)];
          break;
        case 3:
          cpu.v[GETO(2)] ^= cpu.v[GETO(1)];
          break;
        case 4:
          {
            uint16_t r = cpu.v[GETO(2)] + cpu.v[GETO(1)];
            cpu.v[0xf] = (r >= 255);
            cpu.v[GETO(2)] = (uint8_t) r;
            break;
          }
        case 5:
          {
            uint16_t r = cpu.v[GETO(2)] - cpu.v[GETO(1)];
            cpu.v[0xf] = (r >= 255);
            cpu.v[GETO(2)] = (uint8_t) r;
            break;
          }
        case 6:
          cpu.v[0xf] = cpu.v[GETO(2)] & 1;
          cpu.v[GETO(2)] >>= 1;
          break;
        case 7:
          {
            uint16_t r = cpu.v[GETO(1)] - cpu.v[GETO(2)];
            cpu.v[0xf] = (r >= 255);
            cpu.v[GETO(2)] = (uint8_t) r;
            break;
          }
        case 0xe:
          cpu.v[0xf] = cpu.v[GETO(2)] & 128;
          cpu.v[GETO(2)] <<= 1;
          break;
      }
    case 9:
      if (cpu.v[GETO(2)] != cpu.v[GETO(1)]) {
        cpu.pc += 2;
      }
      break;
    case 0xa:
      cpu.i = op & 0x0fff;
      break;
    case 0xb:
      cpu.pc = op & 0x0fff;
      cpu.pc += cpu.v[0];
      break; 
    case 0xc:
      cpu.v[GETO(2)] = ((uint8_t) rand() % 255) & (op & 0x00ff);
      break;
    case 0xd:
      draw_sprite(cpu.v[GETO(2)], cpu.v[GETO(1)], GETO(0));
      draw_flag = 1;
      break;
    case 0xe:
      if ((op & 0x00ff) == 0x9e) {
        if (key_get(cpu.v[GETO(2)])) {
          cpu.pc += 2;
        }
      } else if ((op & 0x00ff) == 0xa1) {
        if (!key_get(cpu.v[GETO(2)])) {
          cpu.pc += 2;
        }
      }
      break;
    case 0xf:
      switch (op & 0x00ff) {
        case 0x07:
          cpu.v[GETO(2)] = cpu.dt;
          break;
        case 0x0a:
          {
            int kp = 0;
            uint8_t k = 0;
            for (uint8_t i = 0; i < 16; i++) {
              k = key_get(i);
              if (k) kp = 1;
            }
            if (!k) {
              cpu.pc -= 2;
              return draw_flag;
            }
            cpu.v[GETO(2)] = k;
            break;
          }
        case 0x15:
          cpu.dt = cpu.v[GETO(2)];
          break;
        case 0x18:
          cpu.st = cpu.v[GETO(2)];
          break;
        case 0x1e:
          cpu.v[0xf] = ((uint32_t) cpu.i + (uint32_t) cpu.v[GETO(2)]) >> 16;
          cpu.i += cpu.v[GETO(2)];
          break;
        case 0x29:
          cpu.i = cpu.v[GETO(2)] * 5; 
          break;
        case 0x33:
          if (cpu.i + 2 >= MEM_SIZE) panic("invalid memory access\n");
          cpu.mem[cpu.i] = cpu.v[GETO(2)] / 100;
          cpu.mem[cpu.i + 1] = (cpu.v[GETO(2)] / 10) % 10;
          cpu.mem[cpu.i + 2] = cpu.v[GETO(2)] % 10;
          break;
        case 0x55:
          if (cpu.i + 15 >= MEM_SIZE) panic("invalid memory access\n");
          for (int i = 0; i < 16; i++) {
            cpu.mem[cpu.i + i] = cpu.v[i];
          }
          break;
        case 0x65:
          if (cpu.i + 15 >= MEM_SIZE) panic("invalid memory access\n");
          for (int i = 0; i < 16; i++) {
            cpu.v[i] = cpu.mem[cpu.i + i];
          }
          break;
      }
      break;
  }

  cpu.dt--;
  cpu.st--;

  return draw_flag;
}


