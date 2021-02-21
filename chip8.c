#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "chip8.h"


void cls(Chip8 *chip8) {
    memset(chip8->video, 0, sizeof(chip8->video));
}

void return_from_subroutine(Chip8 *chip8) {
    chip8->pc = chip8->stack[--chip8->sp];
}

void jump(Chip8 *chip8) {
    uint16_t address = chip8->opcode & 0x0FFF;

	chip8->pc = address;
}

void call_subroutine(Chip8 *chip8) {
    uint16_t address = chip8->opcode & 0x0FFF;

    chip8->stack[chip8->sp++] = chip8->pc;

    chip8->pc = address;
}

void se_Vx_kk(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t byte = chip8->opcode & 0x00FF;

	if (chip8->registers[Vx] == byte) {
		chip8->pc += 2;
	}
}

void sne_Vx_kk(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t byte = chip8->opcode & 0x00FF;

	if (chip8->registers[Vx] != byte) {
		chip8->pc += 2;
	}
}

void se_Vx_Vy(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00FF) >> 4;

	if (chip8->registers[Vx] == chip8->registers[Vy]) {
		chip8->pc += 2;
	}
}

void ld_Vx(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t byte = chip8->opcode & 0x00FF;

	chip8->registers[Vx] = byte;
}

void add_Vx_imm(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t byte = chip8->opcode & 0x00FF;

	chip8->registers[Vx] += byte;
}

void move_Vx_Vy(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00FF) >> 4;

    chip8->registers[Vx] = chip8->registers[Vy];
}

void or_Vx_Vy(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00FF) >> 4;

    chip8->registers[Vx] |= chip8->registers[Vy];
}

void and_Vx_Vy(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00FF) >> 4;

    chip8->registers[Vx] &= chip8->registers[Vy];
}

void xor_Vx_Vy(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00FF) >> 4;

    chip8->registers[Vx] ^= chip8->registers[Vy];
}

void add_Vx_Vy(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
    uint8_t Vy = (chip8->opcode & 0x00FF) >> 4;

    uint16_t sum = chip8->registers[Vx] + chip8->registers[Vy];

    if(sum > 0xFF)
        chip8->registers[VF] = 1;
    else
        chip8->registers[VF] = 0;

    chip8->registers[Vx] = sum & 0xFF;
}

void sub_Vx_Vy(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
    uint8_t Vy = (chip8->opcode & 0x00FF) >> 4;

    if(chip8->registers[Vx] > chip8->registers[Vy])
        chip8->registers[VF] = 1;
    else
        chip8->registers[VF] = 0;

    chip8->registers[Vx] -= chip8->registers[Vy];
}

void shr(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

    // Least significant bit
    chip8->registers[VF] = chip8->registers[Vx] & 0x1;

    chip8->registers[Vx] >>= 1;
}

void subn_Vx_Vy(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
    uint8_t Vy = (chip8->opcode & 0x00FF) >> 4;

    if(chip8->registers[Vy] >= chip8->registers[Vx])
        chip8->registers[VF] = 1;
    else
        chip8->registers[VF] = 0;

    chip8->registers[Vx] = chip8->registers[Vy] - chip8->registers[Vx];
}

void shl(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

    // Least significant bit
    chip8->registers[VF] = (chip8->registers[Vx] & 0x80) >> 7;

    chip8->registers[Vx] <<= 1;
}

void sne_Vx_Vy(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00F0) >> 4;

	if (chip8->registers[Vx] != chip8->registers[Vy])
		chip8->pc += 2;

}

void ldi(Chip8 *chip8) {
    uint16_t address = chip8->opcode & 0x0FFF;

	chip8->index = address;
}

void jump_V0(Chip8 *chip8) {
    uint16_t address = chip8->opcode & 0x0FFF;

	chip8->pc = chip8->registers[V0] + address;
}

void rnd(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t byte = chip8->opcode & 0x00FF;

    chip8->registers[Vx] = (rand() % 256) & byte;
}

void drw(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00F0) >> 4;
	uint8_t height = chip8->opcode & 0x000F;

	// Wrap if going beyond screen boundaries
	uint8_t xPos = chip8->registers[Vx] % SCREEN_WIDTH;
	uint8_t yPos = chip8->registers[Vy] % SCREEN_HEIGHT;

	chip8->registers[VF] = 0;

	for (int row = 0; row < height; ++row) {
		uint8_t spriteByte = chip8->memory[chip8->index + row];

		for (int col = 0; col < 8; ++col) {
			uint8_t spritePixel = spriteByte & (0x80 >> col);
			uint32_t* screenPixel = &chip8->video[(yPos + row) * SCREEN_WIDTH + (xPos + col)];

			// Sprite pixel is on
			if (spritePixel) {
				// Screen pixel also on - collision
				if (*screenPixel == 0xFFFFFFFF) {
					chip8->registers[VF] = 1;
				}

				// Effectively XOR with the sprite pixel
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

void skp(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t key = chip8->registers[Vx];

	if (chip8->keypad[key])
		chip8->pc += 2;

}

void sknp(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t key = chip8->registers[Vx];

	if (!chip8->keypad[key])
		chip8->pc += 2;
}

void ld_Vx_dt(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	chip8->registers[Vx] = chip8->delayTimer;
}

void ld_Vx_k(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
    for(int i = 0; i> 16; i++){
        if(chip8->keypad[i])
            chip8->registers[Vx] = i;
        else
            chip8->pc -= 2;
    }
}

void ld_dt_Vx(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	chip8->delayTimer = chip8->registers[Vx];
}

void ld_st_Vx(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	chip8->soundTimer = chip8->registers[Vx];
}

void add_i_Vx(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	chip8->index += chip8->registers[Vx];
}

void ld_F_Vx(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t digit = chip8->registers[Vx];

	chip8->index = FONTSET_START_ADDRESS + (FONT_CHARS_LEN * digit);
}

void st_bcd_Vx(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t value = chip8->registers[Vx];

	// Ones-place
	chip8->memory[chip8->index + 2] = value % 10;
	value /= 10;

	// Tens-place
	chip8->memory[chip8->index + 1] = value % 10;
	value /= 10;

	// Hundreds-place
	chip8->memory[chip8->index] = value % 10;
}

void st_V_regs(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	for (int i = 0; i = Vx; i++)
		chip8->memory[chip8->index + i] = chip8->registers[i];
}

void ld_V_regs(Chip8 *chip8) {
    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	for (int i = 0; i = Vx; i++)
        chip8->registers[i] = chip8->memory[chip8->index + i];
}
