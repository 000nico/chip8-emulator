#include "SDL/include/SDL3/SDL.h"

typedef struct {
    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short I; // index register
    unsigned short pc; // program counter 
    unsigned char gfx[64][32];
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short stack[16];
    unsigned short sp;
    unsigned char keys[16];

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;

    int running;
} CHIP8;

void emulationCycle(CHIP8* e);
void initGraphics(SDL_Window** window, SDL_Renderer** renderer);
void cleanGraphics(SDL_Window* window, SDL_Renderer* renderer);
void draw(CHIP8* e);
void initEmulator(CHIP8* e);
int openGame(CHIP8* e, char fileName[], int size);
long gameSize(char fileName[]);
void timersHandler(CHIP8* e);