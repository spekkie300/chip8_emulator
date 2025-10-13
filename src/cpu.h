#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include <stdint.h>
#define ADDR (cpu->instruction & 0x0FFF)
#define NIBBLE (cpu->instruction & 0x000F)
#define X ((cpu->instruction & 0x0F00) >> 8)
#define Y ((cpu->instruction & 0x00F0) >> 4)
#define BYTE (cpu->instruction & 0x00FF)

// Definitions for specs
#define MEM_SZ (1 << 12)
#define MEM_START 0x200
#define SCR_W 64
#define SCR_H 32
#define PX_SZ 2048
#define SCALE 20
#define REG_SZ 16
#define STCK_SZ 16
#define KEY_SZ 16
#define FONT_SZ 80

// Register Enum
enum {
  V0,
  V1,
  V2,
  V3,
  V4,
  V5,
  V6,
  V7,
  V8,
  V9,
  VA,
  VB,
  VC,
  VD,
  VE,
  VF,
};

typedef struct {
  uint8_t memory[MEM_SZ];
  uint8_t reg[REG_SZ];
  uint8_t regST;
  uint8_t regDT;
  uint8_t regSP;
  uint16_t regI;
  uint16_t regPC;
  uint16_t stack[STCK_SZ];
  uint16_t screenBuf[PX_SZ];
  uint8_t keyStates[KEY_SZ];
  uint8_t drawFlag;
  uint16_t instruction;
  uint8_t randNum;
  uint8_t isPaused;
  bool quirky;
} chip_cpu;
extern chip_cpu *cpu;

int read_image_file(const char *rom);
void load_fontset();
void cpu_init();
void cpu_cycle();
void cpu_cleanup();

#endif
