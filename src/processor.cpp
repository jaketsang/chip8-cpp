#include <cstdint>
#include <fstream>

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONT_START_ADDRESS = 0x50;

const uint8_t FONT[80] =
{
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

class Chip8 {
public:
    Chip8() {
        pc = START_ADDRESS;
        for (unsigned int i = 0; i < 80; i++) {
            memory[FONT_START_ADDRESS + i] = FONT[i];
        }
    }
    uint8_t memory[4096]{};
    uint8_t registers[16]{};
    uint16_t index{};
    uint16_t pc{};          // program counter
    uint16_t stack[16]{};
    uint8_t sp{};
    uint8_t delayTimer{}; 
    uint8_t soundTimer{};
    uint8_t keyboard[16]{};
    uint32_t display[64 * 32]{};
    uint16_t opcode;

    void LoadROM(char const* filename);
    void OP_00E0();
    void OP_00EE();
    void OP_1nnn();
    void OP_2nnn();
    void OP_3xkk();
    void OP_4xkk();
    void OP_5xy0();
    void OP_6xkk();
    void OP_7xkk();
    void OP_8xy0();
    void OP_8xy1();
    void OP_8xy2();
    void OP_8xy3();
    void OP_8xy4();
    void OP_8xy5();
    void OP_8xy6();
    void OP_8xy7();
    void OP_8xyE();
    void OP_9xy0();
    void OP_Annn();
    void OP_Bnnn();
    void OP_Cxkk();
    void OP_Dxyn();
    void OP_Ex9E();
    void OP_Ex9E();
    void OP_ExA1();
    void OP_Fx07();
    void OP_Fx0A();
    void OP_Fx15();
    void OP_Fx18();
    void OP_Fx1E();
    void OP_Fx29();
    void OP_Fx33();
    void OP_Fx55();
    void OP_Fx65();
};



void Chip8::LoadROM(char const* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::in);

    if (file.is_open()) {
        // Load in memory from 0x200
        char c;
        int j = 512; // 0x200
        for (int i = 0x200; file.get(c); i++) {
            if (j >= 4096) {
                return;
            }
            memory[i] = (uint8_t) c;
        }
    }
    return;
}

// Instructions

// CLS - clear display
void Chip8::OP_00E0() {
    memset(display, 0, sizeof(display));
}

// RET - Return from subroutine
void Chip8::OP_00EE() {
    sp--;
    pc = stack[sp];
}

// JP addr - Jump to location nnn
void Chip8::OP_1nnn() {
    uint16_t addr = opcode & 0x0FFFu; // what is opcode
    pc = addr;
}

// CALL addr - Call subroutine at nnn
void Chip8::OP_2nnn() {
    uint16_t addr = opcode & 0xFFFu; 
    stack[sp] = pc;
    sp++;
    pc = addr;
}

// SE Vx, byte - skip next instruction if Vx = kk
void Chip8::OP_3xkk() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    if (registers[Vx] == byte) {
        pc += 2;
    }
}

// SNE Vx ,byte - skip not equal 
void Chip8::OP_4xkk() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    if (registers[Vx] != byte) {
        pc += 2;
    }
}