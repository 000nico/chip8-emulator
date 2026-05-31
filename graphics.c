#include "chip8.h"

void initGraphics(SDL_Window** window, SDL_Renderer** renderer){
    SDL_Init(SDL_INIT_VIDEO);

    *window = SDL_CreateWindow("CHIP-8", 640, 320, 0);

    *renderer = SDL_CreateRenderer(*window, NULL);
}

void cleanGraphics(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void draw(CHIP8* e){
    for(int x = 0; x < 64; x++){
        for(int y = 0; y < 32; y++){
            if(e->gfx[x][y]){
                SDL_FRect rect = { x * 10, y * 10, 10, 10 };
                SDL_SetRenderDrawColor(e->renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(e->renderer, &rect);
            }
        }
    }
}