#include "cpu.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include <stdio.h>

#define BGCOLOR 0x000000FF
#define FGCOLOR 0xFFFFFFFF

static const SDL_Keycode key_map[KEY_SZ] = {
    SDLK_X, SDLK_1, SDLK_2, SDLK_3, SDLK_Q, SDLK_W, SDLK_E, SDLK_A,
    SDLK_S, SDLK_D, SDLK_Z, SDLK_C, SDLK_4, SDLK_R, SDLK_F, SDLK_V};
// Keymap used later to compare input events
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;

char isRunning;

int initSDL() {

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
    SDL_Log("Error: Couldn't initialize SDL: %s \n", SDL_GetError());
    return -1;
  }

  if (!SDL_SetAppMetadata("Chip8 Emulator", "0.0.1",
                          "com.spekkie300.chip8_emulator")) {
    SDL_Log("Error: Couldn't set app metadata: %s \n", SDL_GetError());
    return -1;
  }

  window = SDL_CreateWindow("Chip8 Emulator", (SCR_W * SCALE), (SCR_H * SCALE),
                            SDL_WINDOW_INPUT_FOCUS);

  if (window == NULL) {
    SDL_Log("Error: Couldn't create window: %s \n", SDL_GetError());
    return -1;
  }

  renderer = SDL_CreateRenderer(window, NULL);
  if (renderer == NULL) {
    SDL_Log("Error: Couldn't create renderer: %s \n", SDL_GetError());
    return -1;
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_STREAMING, SCR_W, SCR_H);
  SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
  if (texture == NULL) {
    SDL_Log("Error: Couldn't create texture: %s \n", SDL_GetError());
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black background
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  printf("Succesfully initialized SDL \n");
  return 1;
}

void drawSDL() {
  void *pixels;
  int pitch;

  SDL_LockTexture(texture, NULL, &pixels, &pitch);

  SDL_FRect fill = {0, 0, SCR_W * SCALE, SCR_H * SCALE};
  uint8_t r = (BGCOLOR >> 24);
  uint8_t g = (BGCOLOR >> 16) & 0xFF;
  uint8_t b = (BGCOLOR >> 8) & 0xFF;
  uint8_t a = BGCOLOR & 0xFF;

  for (int px = 0; px < PX_SZ; px++) {
    uint32_t value = (cpu->screenBuf[px] > 1) ? 1 : cpu->screenBuf[px];
    ((uint32_t *)pixels)[px] = (value) ? FGCOLOR : BGCOLOR;
  }

  SDL_UnlockTexture(texture);
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderFillRect(renderer, &fill);
  SDL_RenderTexture(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void detectInputSDL() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_EVENT_QUIT:
      isRunning = 0;
      break;
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
      // Handle key events directly
      for (int key = 0; key < KEY_SZ; key++) {
        if (e.key.key == key_map[key]) {
          cpu->keyStates[key] = (e.type == SDL_EVENT_KEY_DOWN) ? 1 : 0;
          break;
        }
      }
      // Handle escape key
      if (e.key.key == SDLK_ESCAPE && e.type == SDL_EVENT_KEY_DOWN) {
        isRunning = 0;
      }
      break;
    }
  }
}

void exitSDL() {
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    printf("Usage ./chip8 <image-file> [-m]  \n");
    return -1;
  };

  if (initSDL() == 1) {
    cpu_init();
    read_image_file(argv[1]);
    cpu->isPaused = 0;
  }

  if (argc == 3 && !strcmp(argv[2], "-m")) {
    printf("-m flag set, starting in modern mode \n");
    cpu->quirky = false;
  } else {
    cpu->quirky = true;
  }

  isRunning = 1;

  uint64_t prev_time = SDL_GetTicks();
  while (isRunning) {

    detectInputSDL();
    cpu_cycle();

    if (cpu->drawFlag == 1) {
      drawSDL();
      cpu->drawFlag = 0;
    }

    uint64_t curr_time = SDL_GetTicks();
    uint64_t elapsed_time = curr_time - prev_time;
    if (elapsed_time < 16) {
      SDL_Delay(16 - elapsed_time);
    }
    prev_time = curr_time;
  }
  cpu_cleanup();
  exitSDL();
  return 0;
}
