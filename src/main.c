#include <SDL3/SDL.h>
#include <stdio.h>

#define MEM_SZ (1 << 12) // max memory size 4096 adresses
#define SCR_W 64
#define SCR_H 32

uint8_t memory[MEM_SZ];
uint16_t memStart = 0x200;

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
  R_D_TMR,
  R_S_TMR,
};
uint8_t reg[R_S_TMR];
uint8_t regSP = 0;
uint16_t regI = 0;
uint16_t regPC = 0;

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
    return 1;
  };

  // TODO: Load ROM: open file, read, load into memory, check for size, throw
  // errors if things go wrong
}
