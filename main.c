#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include "chip8.h"

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("use: chip8.exe <rom>\n");
        return 1;
    }
    
    CHIP8 chip8;
    chip8.running = 1;

    initEmulator(&chip8);
    initGraphics(&chip8.window, &chip8.renderer);
    if(!openGame(&chip8, argv[1], gameSize(argv[1])))
        chip8.running = 0;
    
    while(chip8.running){
        for(int i = 0; i < 10; i++){
            emulationCycle(&chip8);
            timersHandler(&chip8);
        }

        // msg loop
        while(SDL_PollEvent(&chip8.event)){
            if(chip8.event.type == SDL_EVENT_QUIT)
                chip8.running = 0;

            if(chip8.event.type == SDL_EVENT_KEY_DOWN){
                switch (chip8.event.key.key) {
                    case SDLK_1: chip8.keys[0x1] = 1; break;
                    case SDLK_2: chip8.keys[0x2] = 1; break;
                    case SDLK_3: chip8.keys[0x3] = 1; break;
                    case SDLK_4: chip8.keys[0xC] = 1; break;
                    case SDLK_Q: chip8.keys[0x4] = 1; break;
                    case SDLK_W: chip8.keys[0x5] = 1; break;
                    case SDLK_E: chip8.keys[0x6] = 1; break;
                    case SDLK_R: chip8.keys[0xD] = 1; break;
                    case SDLK_A: chip8.keys[0x7] = 1; break;
                    case SDLK_S: chip8.keys[0x8] = 1; break;
                    case SDLK_D: chip8.keys[0x9] = 1; break;
                    case SDLK_F: chip8.keys[0xE] = 1; break;
                    case SDLK_Z: chip8.keys[0xA] = 1; break;
                    case SDLK_X: chip8.keys[0x0] = 1; break;
                    case SDLK_C: chip8.keys[0xB] = 1; break;
                    case SDLK_V: chip8.keys[0xF] = 1; break;
                }
            }

            if(chip8.event.type == SDL_EVENT_KEY_UP){
                switch(chip8.event.key.key){
                    case SDLK_1: chip8.keys[0x1] = 0; break;
                    case SDLK_2: chip8.keys[0x2] = 0; break;
                    case SDLK_3: chip8.keys[0x3] = 0; break;
                    case SDLK_4: chip8.keys[0xC] = 0; break;
                    case SDLK_Q: chip8.keys[0x4] = 0; break;
                    case SDLK_W: chip8.keys[0x5] = 0; break;
                    case SDLK_E: chip8.keys[0x6] = 0; break;
                    case SDLK_R: chip8.keys[0xD] = 0; break;
                    case SDLK_A: chip8.keys[0x7] = 0; break;
                    case SDLK_S: chip8.keys[0x8] = 0; break;
                    case SDLK_D: chip8.keys[0x9] = 0; break;
                    case SDLK_F: chip8.keys[0xE] = 0; break;
                    case SDLK_Z: chip8.keys[0xA] = 0; break;
                    case SDLK_X: chip8.keys[0x0] = 0; break;
                    case SDLK_C: chip8.keys[0xB] = 0; break;
                    case SDLK_V: chip8.keys[0xF] = 0; break;
                }
            }
        }

        // clean screen
        SDL_SetRenderDrawColor(chip8.renderer, 0, 0, 0, 255);
        SDL_RenderClear(chip8.renderer);

        // draw loop
        draw(&chip8);

        SDL_RenderPresent(chip8.renderer);

        SDL_Delay(16);
    }

    cleanGraphics(chip8.window, chip8.renderer);
}