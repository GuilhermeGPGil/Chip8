#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "chip8.h"
#include "emulator.h"

Chip8 *chip8;

int main (int argc, char *argv[]) {

    if(argc < 2) {
        fprintf(stderr,"No rom given\n");
        exit(EXIT_FAILURE);
    }
    chip8 = malloc(sizeof(Chip8));

    srand(time(NULL));

    innit(chip8);
    
    load_rom(chip8,argv[1]);


    while(1){
        fetch_opcode(chip8);
        chip8->pc = chip8->pc+2;
        exec_opcode(chip8);
    }

}
