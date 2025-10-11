#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <stdio.h>

int main(void) {

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_Init failed:  %s", SDL_GetError());
    return 1;
  }

  else {
    printf("SDL Seems to work \n");

    return 0;
  }
}
