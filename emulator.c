#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#include "chip8.h"
#include "emulator.h"

void load_rom(Chip8 *chip8, const char *rom_name) {

    FILE * rom = fopen(rom_name,"rb");
    fread(&(chip8->memory[START_ADRESS]), sizeof(uint8_t), MAX_ROM_SIZE, rom);
    fclose(rom);

}

void innit(Chip8 *chip8) {

    chip8->pc = START_ADRESS;


    for(int i = 0; i < FONTSET_SIZE; i++) {
            chip8->memory[FONTSET_START_ADDRESS+i] = FONTSET[i];
    }

}

void reset(Chip8 *chip8) {
    memset(chip8->registers,0,sizeof(chip8->registers));
    memset(chip8->memory,0,sizeof(chip8->memory));
    memset(chip8->stack,0,sizeof(chip8->stack));
    memset(chip8->keypad,0,sizeof(chip8->keypad));
    memset(chip8->video,0,sizeof(chip8->video));
    chip8->pc = START_ADRESS;
    chip8->sp = 0;
    chip8->index = 0;
    chip8->delayTimer = 0;
    chip8->soundTimer = 0;
    chip8->opcode = 0;
}

void fetch_opcode(Chip8 *chip8) {
    uint8_t hi = chip8->memory[chip8->pc];
    uint8_t lo = chip8->memory[chip8->pc+1];
    uint16_t opcode = ((0 | hi)<<8) | lo;
    chip8->opcode = opcode;
}

void exec_opcode(Chip8 *chip8) {
    uint8_t first_nibble = (chip8->opcode &  0xF000)>>12;
    uint8_t last_nibble = (chip8->opcode & 0xF);
    uint8_t last_two_nibbles = (chip8->opcode & 0xFF);
    printf("%x\n", chip8->opcode);
    if(!chip8->opcode)
        exit(1);
    switch(first_nibble){
        case 1:
            jump(chip8);
            break;
        case 2:
            call_subroutine(chip8);
            break;
        case 3:
            se_Vx_kk(chip8);
            break;
        case 4:
            sne_Vx_kk(chip8);
            break;
        case 5:
            se_Vx_Vy(chip8);
            break;
        case 6:
            ld_Vx(chip8);
            break;
        case 7:
            add_Vx_imm(chip8);
            break;
        case 9:
            sne_Vx_Vy(chip8);
            break;
        case 0xA:
            ldi(chip8);
            break;
        case 0xB:
            jump_V0(chip8);
            break;
        case 0xC:
            rnd(chip8);
            break;
        case 0xD:
            drw(chip8);
            break;
        case 8:
            switch(last_nibble) {
                case 0:
                    move_Vx_Vy(chip8);
                    break;
                case 1:
                    or_Vx_Vy(chip8);
                    break;
                case 2:
                    and_Vx_Vy(chip8);
                    break;
                case 3:
                    xor_Vx_Vy(chip8);
                    break;
                case 4:
                    add_Vx_Vy(chip8);
                    break;
                case 5:
                    sub_Vx_Vy(chip8);
                    break;
                case 6:
                    shr(chip8);
                    break;
                case 7:
                    subn_Vx_Vy(chip8);
                    break;
                case 0xE:
                    shl(chip8);
                    break;
            }
            break;
        case 0:
            switch(last_nibble) {
                case 0:
                    cls(chip8);
                    printf("%s\n", "Correu tudo bem :)");
                    break;
                case 0xE:
                    return_from_subroutine(chip8);
                    break;
            }
            break;
        case 0xE:
            switch(last_nibble) {
                case 1:
                    sknp(chip8);
                    break;
                case 0xE:
                    skp(chip8);
                    break;
            }
            break;
        case 0xF:
            switch(last_two_nibbles) {
                case 0x07:
                    ld_Vx_dt(chip8);
                    break;
                case 0x0A:
                    ld_Vx_k(chip8);
                    break;
                case 0x15:
                    ld_dt_Vx(chip8);
                    break;
                case 0x18:
                    ld_st_Vx(chip8);
                    break;
                case 0x1E:
                    add_i_Vx(chip8);
                    break;
                case 0x29:
                    ld_F_Vx(chip8);
                    break;
                case 0x33:
                    st_bcd_Vx(chip8);
                    break;
                case 0x55:
                    st_V_regs(chip8);
                    break;
                case 0x65:
                    ld_V_regs(chip8);
                    break;

            }
            break;
    }
}
