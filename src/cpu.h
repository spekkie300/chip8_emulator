#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#define MEM_SZ (1 << 12)
#define MEM_START 0x200
#define SCR_W 64
#define SCR_H 32
#define PX_SZ 2048
#define SCALE 10
#define REG_SZ 16
#define STCK_SZ 16
#define FONT_SZ 80

// Register Enum
enum {
  R_V0,
  R_V1,
  R_V2,
  R_V3,
  R_V4,
  R_V5,
  R_V6,
  R_V7,
  R_V8,
  R_V9,
  R_VA,
  R_VB,
  R_VC,
  R_VD,
  R_VE,
  R_VF,
};

typedef struct {
  uint8_t memory[MEM_SZ];
  uint8_t reg[R_VF];
  uint8_t regST;
  uint8_t regDT;
  uint8_t regSP;
  uint8_t regI;
  uint16_t regPC;
  uint16_t stack[STCK_SZ];

} chip_cpu;
extern chip_cpu *cpu;

int read_image_file(const char *rom);
void load_fontset();
void cpu_init();

#endif
