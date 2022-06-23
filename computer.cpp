#include <iostream>

using namespace std;

struct Computer {
    unsigned char status;
    bool simulating;  

    bool halted = false;

    unsigned char ROM[0x80];
    unsigned char RAM[0x40];
    unsigned char MEMORY[0xFF];

    unsigned char PC = 0x00;

    unsigned char A;
    unsigned char B;
};

unsigned char peek(struct Computer *state, unsigned short addr) {
    return state->MEMORY[addr];
}

void poke(struct Computer *state, unsigned short addr, unsigned char data) {
    state->MEMORY[addr] = data;
}

static inline void mem_copy(struct Computer *state, unsigned short addr, unsigned char *data, unsigned long length) {
    unsigned long i = 0;
    while (i != length) {
        poke(state, addr + i, data[i]);
        i++;
    }
}

static inline void mem_set(struct Computer *state, unsigned short addr, unsigned char data, unsigned long length) {
    while (length-- != 0) {
        poke(state, addr++, data);
    }
}

void step(struct Computer *state) {
    unsigned char PC0 = peek(state, state->PC++);
    unsigned char ins = (PC0 & 0xF0) >> 4;

    switch (ins) {
        case 0x00: // NOP
            break;
        case 0x01: // LDA
            state->A = peek(state, PC0 & 0xF);
            break;
        case 0x02: // ADD
            state->B = PC0 & 0xF;
            state->A += state->B;
            break;
        case 0x03: // SUB
            state->B = PC0 & 0xF;
            state->A -= state->B;
            break;
        case 0x04: // STA
            poke(state, PC0 & 0xF, state->A);
            break;
        case 0x05: // LDI
            state->A = PC0 & 0xF;
            break;
        case 0x06: // JMP
            state->PC = PC0 & 0xF;
            break;
        case 0x0F: // HLT
            state->halted = true;
            break;
    }

}

int simulate(struct Computer *state) {
    state->simulating = true;

    for(int i = 0; i < 10; ++i) {
        cout << "Address: " << i << "  Data: " << static_cast<unsigned>(state->MEMORY[i]) << endl;
    }

    while(!state->halted) {
        step(state);
    }

    cout << static_cast<unsigned>(state->PC) << endl;
    cout << static_cast<unsigned>(state->A) << endl;

    return 0;
}

int main() {
    struct Computer state;
    unsigned char data[7] = {0x52, 0x27, 0x31, 0xF0, 0, 0, 0};
    mem_copy(&state, 0, data, 7);
    // mem_set(&state, 0, 0x00, 6);
    // poke(&state, 0, 0x49);
    // poke(&state, 1, 0x59);
    // poke(&state, 2, 0x6B);

    simulate(&state);
    
    return 0;
}
