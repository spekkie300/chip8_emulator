#include "cpu.h"
#include "instructions.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Fontset
uint8_t fontSet[FONT_SZ] = {
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

chip_cpu *cpu = NULL;

void cpu_init() {
  cpu = calloc(1, sizeof(*cpu));
  if (cpu == NULL) {
    return;
  }

  load_fontset();
  cpu->regPC = MEM_START;
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
    printf("Error: Reading rom went wrong(fread is not equal to romSize \n");
    fclose(file);
    free(rom_buf);
    return -1;
  }

  for (size_t data = 0; data < romSize; data++) {
    cpu->memory[MEM_START + data] = rom_buf[data];
  }

  free(rom_buf);
  fclose(file);
  printf("Rom succesfully loaded into memory! \n");
  return 0;
}

// function to load fontset into memory
void load_fontset() {
  for (int i = 0; i < 80; i++) {
    cpu->memory[i + 0x50] = fontSet[i];
  }

  bool containsNull;

  for (int i = 0; i < 80; i++) {
    if (cpu->memory[i + 0x50] == 0) {
      printf("Error: Somethign went wrong while loading fontset \n");
      containsNull = false;
      break;
    }
  }
  if (!containsNull) {
    printf("Font Loaded correctly into memory \n");
  }
}
