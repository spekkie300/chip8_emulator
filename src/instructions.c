#include "instructions.h"
#include "cpu.h"
#include <stdint.h>
#include <string.h>

void fetch_instruction() {
  uint16_t instruction =
      cpu->memory[cpu->regPC] << 8 | cpu->memory[cpu->regPC + 1];
  cpu->regPC += 2;
  cpu->instruction = instruction;
}

void exec_instructions() {

  switch (cpu->instruction & 0xF000) {
  case 0x0000:
    switch (BYTE) {
    case 0x00E0:
      instr_00E0();
      break;
    }
    break;
  case 0x1000:
    instr_1NNN();
    break;

  case 0x6000:
    instr_6XNN();
    break;

  case 0x7000:
    instr_7XNN();
    break;

  case 0xA000:
    instr_ANNN();
    break;

  case 0xD000:
    instr_DXYN();
    break;
  }
}

void instr_00E0() {
  memset(cpu->screenBuf, 0, sizeof(cpu->screenBuf));
  cpu->drawFlag = 1;
}

void instr_1NNN() { cpu->regPC = ADDR; }

void instr_6XNN() { cpu->reg[X] = BYTE; }

void instr_7XNN() { cpu->reg[X] += BYTE; }

void instr_ANNN() { cpu->regI = ADDR; }

void instr_DXYN() {
  uint8_t xCoordinate = (cpu->reg[X] & 63);
  uint8_t yCoordinate = (cpu->reg[Y] & 31);
  cpu->reg[VF] = 0;

  for (int i = 0; i < NIBBLE; i++) {
    uint8_t spriteByte = cpu->memory[cpu->regI + i];
    if ((yCoordinate + NIBBLE) >= SCR_H) {
      break;
    }
    for (int xBits = 0; xBits < 8; xBits++) {
      if ((xCoordinate + xBits) >= SCR_W) {
        break;
      }
      if (spriteByte & (0x80 >> xBits)) {
        uint16_t pixelIndex =
            ((yCoordinate + i) * SCR_W) + (xCoordinate + xBits);
        if (cpu->screenBuf[pixelIndex]) {
          cpu->reg[VF] = 1;
        }
        cpu->screenBuf[pixelIndex] ^= 1;
      }
    }
  }
  cpu->drawFlag = 1;
}
