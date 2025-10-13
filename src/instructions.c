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
      instr_00E0(); // CLS
      break;

    case 0x00EE:
      instr_00EE(); // RET
      break;
    }
    break;
  case 0x1000:
    instr_1NNN(); // JP addr
    break;

  case 0x2000:
    instr_2NNN(); // CALL addr
    break;

  case 0x3000:
    instr_3XNN(); // SE Vx, byte
    break;

  case 0x4000:
    instr_4XNN(); // SNE Vx, byte
    break;

  case 0x5000:
    instr_5XY0(); // SE Vx, Vy
    break;

  case 0x6000:
    instr_6XNN(); // LD Vx, byte
    break;

  case 0x7000:
    instr_7XNN(); // ADD Vx, byte
    break;

  case 0x8000:
    switch (NIBBLE) {
    case 0x0000:
      instr_8XY0();
      break;

    case 0x0001:
      instr_8XY1();
      break;

    case 0x0002:
      instr_8XY2();
      break;

    case 0x0003:
      instr_8XY3();
      break;

    case 0x0004:
      instr_8XY4();
      break;

    case 0x0005:
      instr_8XY5();
      break;

    case 0x0006:
      instr_8XY6();
      break;

    case 0x0007:
      instr_8XY7();
      break;

    case 0x000E:
      instr_8XYE();
      break;
    }
    break;

  case 0x9000:
    instr_9XY0();
    break;

  case 0xA000:
    instr_ANNN();
    break;

  case 0xB000:
    instr_BNNN();
    break;

  case 0xC000:
    instr_CXNN();
    break;

  case 0xD000:
    instr_DXYN();
    break;

  case 0xE000:
    switch (BYTE) {
    case 0x009E:
      instr_EX9E();
      break;

    case 0x00A1:
      instr_EXA1();
      break;
    }
    break;

  case 0xF000:
    switch (BYTE) {
    case 0x0007:
      instr_FX07();
      break;

    case 0x000A:
      instr_FX0A();
      break;

    case 0x0015:
      instr_FX15();
      break;

    case 0x0018:
      instr_FX18();
      break;

    case 0x001E:
      instr_FX1E();
      break;

    case 0x0029:
      instr_FX29();
      break;

    case 0x0033:
      instr_FX33();
      break;

    case 0x0055:
      instr_FX55();
      break;

    case 0x0065:
      instr_FX65();
      break;
    }
    break;
  }
}

void instr_00E0() {
  memset(cpu->screenBuf, 0, sizeof(cpu->screenBuf));
  cpu->drawFlag = 1;
}

void instr_00EE() {
  cpu->regPC = cpu->stack[cpu->regSP];
  cpu->regSP -= 1;
}

void instr_1NNN() { cpu->regPC = ADDR; }

void instr_2NNN() {
  cpu->regSP += 1;
  cpu->stack[cpu->regSP] = cpu->regPC;
  cpu->regPC = ADDR;
}

void instr_3XNN() {
  if (cpu->reg[X] == BYTE) {
    cpu->regPC += 2;
  }
}

void instr_4XNN() {
  if (cpu->reg[X] != BYTE) {
    cpu->regPC += 2;
  }
}

void instr_5XY0() {
  if (cpu->reg[X] == cpu->reg[Y]) {
    cpu->regPC += 2;
  }
}

void instr_6XNN() { cpu->reg[X] = BYTE; }

void instr_7XNN() { cpu->reg[X] += BYTE; }

void instr_8XY0() { cpu->reg[X] = cpu->reg[Y]; }

void instr_8XY1() { cpu->reg[X] |= cpu->reg[Y]; }

void instr_8XY2() { cpu->reg[X] &= cpu->reg[Y]; }

void instr_8XY3() { cpu->reg[X] ^= cpu->reg[Y]; }

void instr_8XY4() {
  uint8_t sum = cpu->reg[X] + cpu->reg[Y];
  if (sum > 0xFF) {
    cpu->reg[VF] = 1;
  }
  cpu->reg[X] = sum;
}

void instr_8XY5() {
  if (cpu->reg[X] > cpu->reg[Y]) {
    cpu->reg[VF] = 1;
  } else {
    cpu->reg[VF] = 0;
  }
  cpu->reg[X] -= cpu->reg[Y];
}

void instr_8XY6() {
  if (cpu->reg[X] & 0x01) {
    cpu->reg[VF] = 1;
  } else {
    cpu->reg[VF] = 0;
  }
  cpu->reg[X] /= 2;
}

void instr_8XY7() {
  if (cpu->reg[Y] > cpu->reg[X]) {
    cpu->reg[VF] = 1;
  } else {
    cpu->reg[VF] = 0;
  }
  cpu->reg[X] = cpu->reg[Y] - cpu->reg[X];
}

void instr_8XYE() {
  if (cpu->reg[X] & 0x80) {
    cpu->reg[VF] = 1;
  } else {
    cpu->reg[VF] = 0;
  }
  cpu->reg[X] *= 2;
}

void instr_9XY0() {
  if (cpu->reg[X] != cpu->reg[Y]) {
    cpu->regPC += 2;
  }
}

void instr_ANNN() { cpu->regI = ADDR; }

void instr_BNNN() { cpu->regPC = (ADDR + cpu->reg[V0]); }

void instr_CXNN() {
  // TODO: do this one later, needs random number
}

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

void instr_EX9E() {
  // TODO: need to implement SDL keyreading first i suppose
}

void instr_EXA1() {
  // TODO: Same thing here, needs sdl input to be functioning
}

void instr_FX07() { cpu->reg[X] = cpu->regDT; }

void instr_FX0A() {
  // TODO: wait for keypress, so pause(needs SDL), then store value of key in Vx
}

void instr_FX15() { cpu->regDT = cpu->reg[X]; }

void instr_FX18() { cpu->regST = cpu->reg[X]; }

void instr_FX1E() { cpu->regI += cpu->reg[X]; }

void instr_FX29() {}
