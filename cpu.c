#include "chip8.h"
#include<stdlib.h>

void emulationCycle(CHIP8* e){
    e->opcode = e->memory[e->pc] << 8 | e->memory[e->pc + 1];
    e->pc += 2;

    switch (e->opcode & 0xF000){
        case 0x0000:
            switch(e->opcode & 0x00FF){
                case 0x00E0:
                    memset(e->gfx, 0, sizeof(e->gfx));
                    break;

                case 0x00EE:
                    e->sp--;
                    e->pc = e->stack[e->sp];
                    break;
            }
            break;

        case 0x1000:
            e->pc = e->opcode & 0x0FFF;
            break;

        case 0x2000:
            e->stack[e->sp] = e->pc;
            e->sp++;
            e->pc = e->opcode & 0x0FFF;
            break;

        case 0x3000:
            if(e->V[(e->opcode & 0x0F00) >> 8] == (e->opcode & 0x00FF))
                e->pc += 2;
            break;

        case 0x4000:
            if(e->V[(e->opcode & 0x0F00) >> 8] != (e->opcode & 0x00FF))
                e->pc += 2;
            break;

        case 0x5000:
            if(e->V[(e->opcode & 0x0F00) >> 8] == e->V[(e->opcode & 0x00F0) >> 4])
                e->pc += 2;
            break;

        case 0x6000:
            e->V[(e->opcode & 0x0F00) >> 8] = e->opcode & 0x00FF;
            break;

        case 0x7000:
            e->V[(e->opcode & 0x0F00) >> 8] += e->opcode & 0x00FF;
            break;

        case 0x8000:
            switch(e->opcode & 0x000F) {
                case 0x0:
                    e->V[(e->opcode & 0x0F00) >> 8] = e->V[(e->opcode & 0x00F0) >> 4];
                    break;

                case 0x1:
                    e->V[(e->opcode & 0x0F00) >> 8] |= e->V[(e->opcode & 0x00F0) >> 4];
                    break;

                case 0x2:
                    e->V[(e->opcode & 0x0F00) >> 8] &= e->V[(e->opcode & 0x00F0) >> 4];
                    break;

                case 0x3:
                    e->V[(e->opcode & 0x0F00) >> 8] ^= e->V[(e->opcode & 0x00F0) >> 4];
                    break;

                case 0x4:
                    if((e->V[(e->opcode & 0x0F00) >> 8] + e->V[(e->opcode & 0x00F0) >> 4]) > 255)
                        e->V[0xF] = 1;
                    else
                        e->V[0xF] = 0;
                    e->V[(e->opcode & 0x0F00) >> 8] += e->V[(e->opcode & 0x00F0) >> 4];
                    break;

                case 0x5:
                    if(e->V[(e->opcode & 0x0F00) >> 8] > e->V[(e->opcode & 0x00F0) >> 4])
                        e->V[0xF] = 1;
                    else
                        e->V[0xF] = 0;
                    e->V[(e->opcode & 0x0F00) >> 8] -= e->V[(e->opcode & 0x00F0) >> 4];
                    break;

                case 0x6:
                    e->V[0xF] = e->V[(e->opcode & 0x0F00) >> 8] & 0x1;
                    e->V[(e->opcode & 0x0F00) >> 8] >>= 1;
                    break;

                case 0x7:
                    if(e->V[(e->opcode & 0x00F0) >> 4] > e->V[(e->opcode & 0x0F00) >> 8])
                        e->V[0xF] = 1;
                    else
                        e->V[0xF] = 0;
                    e->V[(e->opcode & 0x0F00) >> 8] = e->V[(e->opcode & 0x00F0) >> 4] - e->V[(e->opcode & 0x0F00) >> 8];
                    break;

                case 0xE:
                    e->V[0xF] = (e->V[(e->opcode & 0x0F00) >> 8] & 0x80) >> 7;
                    e->V[(e->opcode & 0x0F00) >> 8] <<= 1;
                    break;
            }
            break;

        case 0x9000:
            if(e->V[(e->opcode & 0x0F00) >> 8] != e->V[(e->opcode & 0x00F0) >> 4])
                e->pc += 2;
            break;

        case 0xA000:
            e->I = e->opcode & 0x0FFF;
            break;

        case 0xB000:
            e->pc = (e->opcode & 0x0FFF) + e->V[0x0];
            break;

        case 0xC000:
            e->V[(e->opcode & 0x0F00) >> 8] = (rand() % 256) & (e->opcode & 0x00FF);
            break;

        case 0xD000: {
            unsigned char x = (e->opcode & 0x0F00) >> 8;
            unsigned char y = (e->opcode & 0x00F0) >> 4;
            unsigned char n = e->opcode & 0x000F;

            e->V[0xF] = 0;

            for(int row = 0; row < n; row++){
                unsigned char byte = e->memory[e->I + row];

                for(int col = 0; col < 8; col++){
                    unsigned char pixel = (byte >> (7 - col)) & 0x1;

                    if(pixel == 1){
                        int px = (e->V[x] + col) % 64;
                        int py = (e->V[y] + row) % 32;

                        if(e->gfx[px][py] == 1)
                            e->V[0xF] = 1;

                        e->gfx[px][py] ^= 1;
                    }
                }
            }
            break;
        }

        case 0xE000:
            switch(e->opcode & 0x00FF) {
                case 0x9E:
                    if(e->keys[e->V[(e->opcode & 0x0F00) >> 8]] == 1)
                        e->pc += 2;
                    break;

                case 0xA1:
                    if(e->keys[e->V[(e->opcode & 0x0F00) >> 8]] == 0)
                        e->pc += 2;
                    break;
            }
            break;

        case 0xF000:
            switch(e->opcode & 0x00FF) {
                case 0x07:
                    e->V[(e->opcode & 0x0F00) >> 8] = e->delay_timer;
                    break;

                case 0x15:
                    e->delay_timer = e->V[(e->opcode & 0x0F00) >> 8];
                    break;

                case 0x18:
                    e->sound_timer = e->V[(e->opcode & 0x0F00) >> 8];
                    break;

                case 0x1E:
                    e->I += e->V[(e->opcode & 0x0F00) >> 8];
                    break;

                case 0x55: {
                    for(int i = 0; i <= ((e->opcode & 0x0F00) >> 8); i++)
                        e->memory[e->I + i] = e->V[i];
                    break;
                }

                case 0x65: {
                    for(int i = 0; i <= ((e->opcode & 0x0F00) >> 8); i++)
                        e->V[i] = e->memory[e->I + i];
                    break;
                }

                case 0x0A: {
                    for(int i = 0; i < 16; i++){
                        if(e->keys[i]){
                            e->V[(e->opcode & 0x0F00) >> 8] = i;
                            e->pc += 2;
                            break;
                        }
                    }
                    e->pc -= 2;  // cancelar el avance inicial si no hay tecla
                    break;
                }

                case 0x33: {
                    e->memory[e->I]   = e->V[(e->opcode & 0x0F00) >> 8] / 100;
                    e->memory[e->I+1] = (e->V[(e->opcode & 0x0F00) >> 8] / 10) % 10;
                    e->memory[e->I+2] = e->V[(e->opcode & 0x0F00) >> 8] % 10;
                    break;
                }

                case 0x29:
                    e->I = e->V[(e->opcode & 0x0F00) >> 8] * 5;
                    break;
            }
            break;
    }
}