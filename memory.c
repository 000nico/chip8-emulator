#include "chip8.h"
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>

void initEmulator(CHIP8* e){
    unsigned char fontset[80] = { 
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

    e->pc = 0x200;
    e->opcode = 0;
    e->I = 0;
    e->sp = 0;

    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 32; j++){
            e->gfx[i][j] = 0;
        }
    }

    for(int i = 0; i < 16; i++){
        e->V[i] = 0;
        e->stack[i] = 0;
    }
    
    for(int i = 0; i < 4096; i++)
        e->memory[i] = 0;

    srand(time(NULL));
    memcpy(e->memory, fontset, 80);
}

int openGame(CHIP8* e, char fileName[], int size){
    char path[256];
    strcpy(path, "games/");
    strcat(path, fileName);
    
    FILE* rom = fopen(path, "rb");

    if(!rom){
        printf("error opening ROM");
        return 0;
    }

    else{
        fread(&e->memory[0x200], 1, size, rom);
        fclose(rom);
        return 1;
    }
}

long gameSize(char fileName[]){
    char path[256];
    strcpy(path, "games/");
    strcat(path, fileName);

    FILE* rom = fopen(path, "rb");
    long size;
    if(!rom)
        return 0;

    else{
        fseek(rom, 0, SEEK_END);
        size = ftell(rom);
        fclose(rom);
    }

    return size;
}

void timersHandler(CHIP8* e){
    if(e->delay_timer > 0)
        e->delay_timer--;

    if(e->sound_timer > 0)
        e->sound_timer--;
}