#include <stdio.h>
#include <stdint.h>


void load_rom(Chip8 * chip8, const char *rom_name);
void innit(Chip8 *chip8);
void reset(Chip8 *chip8);
void fetch_opcode(Chip8 *chip8);
void exec_opcode(Chip8 *chip8);
