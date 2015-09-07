#include "DisplayRaw.h"

SDL_Window* DisplayRaw::win = nullptr;
SDL_Renderer* DisplayRaw::ren = nullptr;

void DisplayRaw::init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_SHOWN, &win, &ren);
    SDL_RenderClear(DisplayRaw::ren);
}

void DisplayRaw::post(short* frameData) {
    SDL_RenderClear(DisplayRaw::ren);
}

void DisplayRaw::destroy() {
    SDL_DestroyRenderer(DisplayRaw::ren);
    SDL_DestroyWindow(DisplayRaw::win);
    SDL_Quit();
}
