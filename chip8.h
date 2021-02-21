#include <stdio.h>
#include <stdint.h>

#define NUM_REGISTERS 16
#define MEM_SIZE 4096
#define STACK_SIZE 16
#define KEYPAD_SIZE 16
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define START_ADRESS 0x200
#define FONTSET_SIZE 80
#define FONTSET_START_ADDRESS 0x50
#define V0 0
#define FONT_CHARS_LEN 5
#define VF 0xF
#define MAX_ROM_SIZE MEM_SIZE - START_ADRESS

typedef struct Chip8 Chip8;

const static uint8_t FONTSET[FONTSET_SIZE] = {
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

struct Chip8 {
    uint8_t registers[NUM_REGISTERS];
    uint8_t memory[MEM_SIZE];
    uint16_t stack[STACK_SIZE];
    uint8_t keypad[KEYPAD_SIZE];
    uint32_t video[SCREEN_WIDTH*SCREEN_HEIGHT];
    uint16_t pc;
    uint8_t sp;
    uint16_t index;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint16_t opcode;
};


void cls(Chip8 *chip8);                         // 00E0
void return_from_subroutine(Chip8 *chip8);      // 00EE
void jump(Chip8 *chip8);                        // 1NNN
void call_subroutine(Chip8 *chip8);             // 2NNN
void se_Vx_kk(Chip8 *chip8);                    // 3XKK
void sne_Vx_kk(Chip8 *chip8);                   // 4XKK
void se_Vx_Vy(Chip8 *chip8);                    // 5XY0
void ld_Vx(Chip8 *chip8);                       // 6XKK
void add_Vx_imm(Chip8 *chip8);                  // 7XKK
void move_Vx_Vy(Chip8 *chip8);                  // 8XY0
void or_Vx_Vy(Chip8 *chip8);                    // 8XY1
void and_Vx_Vy(Chip8 *chip8);                   // 8XY2
void xor_Vx_Vy(Chip8 *chip8);                   // 8XY3
void add_Vx_Vy(Chip8 *chip8);                   // 8XY4
void sub_Vx_Vy(Chip8 *chip8);                   // 8XY5
void shr(Chip8 *chip8);                         // 8XY6
void subn_Vx_Vy(Chip8 *chip8);                  // 8XY7
void shl(Chip8 *chip8);                         // 8XYE
void sne_Vx_Vy(Chip8 *chip8);                   // 9XY0
void ldi(Chip8 *chip8);                         // ANNN
void jump_V0(Chip8 *chip8);                     // BNNN
void rnd(Chip8 *chip8);                         // CXKK
void drw(Chip8 *chip8);                         // DXYN
void skp(Chip8 *chip8);                         // EX9E
void sknp(Chip8 *chip8);                        // EXA1
void ld_Vx_dt(Chip8 *chip8);                    // FX07
void ld_Vx_k(Chip8 *chip8);                     // FX0A
void ld_dt_Vx(Chip8 *chip8);                    // FX15
void ld_st_Vx(Chip8 *chip8);                    // FX18
void add_i_Vx(Chip8 *chip8);                    // FX1E
void ld_F_Vx(Chip8 *chip8);                     // FX29
void st_bcd_Vx(Chip8 *chip8);                   // FX33
void st_V_regs(Chip8 *chip8);                   // FX55
void ld_V_regs(Chip8 *chip8);                   // FX65
