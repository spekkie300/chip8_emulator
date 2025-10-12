#include "instructions.h"
#include "cpu.h"
#include <stdint.h>
#include <stdio.h>

uint16_t fetch_instruction(uint16_t programCounter) {
  uint16_t instruction =
      cpu->memory[programCounter] << 8 | cpu->memory[programCounter + 1];
  programCounter += 2;
  return instruction;
}

void decode_instructions(uint16_t instruction) {
  uint16_t addr = (instruction & 0x0FFF);
  uint16_t nibble = (instruction & 0x000F);
  uint16_t x = (instruction & 0x0F00);
  uint16_t y = (instruction & 0x00F0);
  uint16_t byte = (instruction & 0x00FF);

  switch (instruction & 0xF000) {
  case 0x0000:
    switch (byte) {
    case 0x00E0:
      // TODO: Need to implement SDL and get the screen functioning before
      // continuing instructions
      instr_00E0();
      break;
    }
  }
}

void instr_00E0() { printf("Not implemented yet!"); }
