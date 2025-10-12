#include "instructions.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define MEM_SZ (1 << 12) // max memory size 4096 adresses
#define MEM_START 0x200
#define SCR_W 64
#define SCR_H 32

int read_image_file(const char *rom);

uint8_t memory[MEM_SZ];

// Registers
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
  R_D_TMR, // TODO: Implement timers decreasing by 60hz when value != 0
  R_S_TMR,
};
uint8_t reg[R_S_TMR];
uint8_t regSP = 0;
uint16_t regI = 0;
uint16_t regPC = 0; // TODO: Initialize regPC to MEM_START when initializing

// Stack
uint16_t stack[16];

// Fontset
uint8_t fontSet[80] = {
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

// Keymap used later to compare input events
// TODO: Implement SDL input polling and compoare it based on scancode
SDL_Keycode key_map[16] = {SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_Q, SDLK_W,
                           SDLK_E, SDLK_R, SDLK_A, SDLK_S, SDLK_D, SDLK_F,
                           SDLK_Z, SDLK_X, SDLK_C, SDLK_V};

// function to load fontset into memory
void load_fontset() {
  for (int i = 0; i < 80; i++) {
    memory[i + 0x50] = fontSet[i];
  }
}

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    printf("Usage ./chip8 [image-file]... \n");
    return -1;
  };

  read_image_file(argv[1]);
  return 0;
}

int read_image_file(const char *rom) {
  size_t romSize = 0;
  unsigned char *rom_buf = NULL;
  FILE *file = NULL;

  file = fopen(rom, "rb");

  if (file == NULL) {
    printf("Error: Unable to open file(file == NULL) \n");
    return -1;
  }
  if (fseek(file, 0, SEEK_END) != 0) {
    printf("Error: Unable to find end of file \n");
    fclose(file);
    return -1;
  }

  romSize = ftell(file);
  uint16_t maxRomSize = (MEM_SZ - MEM_START);

  if (romSize <= 0) {
    printf("Error: Romsize is zero or negative \n");
    fclose(file);
    return -1;
  }

  if (romSize >= maxRomSize) {
    printf("Error: Rom is too big for memory \n");
    fclose(file);
    return -1;
  }

  if (fseek(file, 0, SEEK_SET) != 0) {
    printf("Error: Could not set file pointer to begin \n");
    fclose(file);
    return -1;
  }

  rom_buf = calloc(romSize, sizeof(*rom_buf));
  if (rom_buf == NULL) {
    printf(
        "Error: unable to allocate memory for rom buffer (rom_buf == NULL) \n");
    fclose(file);
    return -1;
  }

  if (fread(rom_buf, sizeof(*rom_buf), romSize, file) != romSize) {
    size_t fread_result = fread(rom_buf, sizeof(*rom_buf), romSize, file);
    printf("Size of Rom_buf: %zu \n", sizeof(*rom_buf));
    printf("Romsize: %zu \n", romSize);
    printf("fread_result: %zu \n", fread_result);
    printf("Error: Reading rom went wrong(fread is not equal to romSize \n");
    fclose(file);
    free(rom_buf);
    return -1;
  }

  for (size_t data = 0; data < romSize; data++) {
    memory[MEM_START + data] = rom_buf[data];
  }

  free(rom_buf);
  fclose(file);
  printf("Rom succesfully loaded into memory! \n");
  return 0;
}

uint16_t fetch_instruction(uint16_t programCounter) {
  uint16_t instruction =
      memory[programCounter] << 8 | memory[programCounter + 1];
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
