#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "vm.h"

extern byte *memory;
extern uint64 *reg;
const char program[] = {0x12, 0x31, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x31, 0x79, 0x99, 0x99, 0x90, 0x50, 0x00, 0x00,
                        0x00, 0x4f, 0xe4, 0x68, 0xab, 0x30, 0x00, 0x00,
                        0x00, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
int program_size = 40;

void init(void)
// initialize the vm
{
    memory = malloc(RAM_SIZE * sizeof(byte));
    printf("%d\n", R_COUNT);
    reg = malloc(R_COUNT * sizeof(uint64));
    for (int i = 0; i < R_COUNT; i++)
    {
        reg[i] = 0;
    }
    loadProgram();
    reg[R_SP] = 0xFFFFFFFF;
    memory[0x79999990] = 0x12;
    memory[0x79999991] = 0x23;
    memory[0x79999992] = 0x34;
    memory[0x79999993] = 0x45;
    memory[0x79999994] = 0x56;
    memory[0x79999995] = 0x67;
    memory[0x79999996] = 0x78;
    memory[0x79999997] = 0x89;
}

void loadProgram(void)
// load the program in the RAM
{
    for (int i = 0; i < program_size; i++)
    {
        memoryWrite(i, program[i]);
    }
}

void shutdown(void)
// shutdown the vm
{
    free(memory);
    free(reg);
}

uint64 memoryRead(uint64 adress)
// Read the value at the RAM adress
{
    return ((byte)memory[adress]);
}

void memoryWrite(uint64 adress, byte value)
// write the given value in the adress
{
    memory[adress] = value;
}

void displayRegisters(void)
{
    for (int i = 0; i < R_COUNT; i++)
    {
        printf("R %d\t:\t0x%lX\n", i, reg[i]);
    }
}

uint64 loadInstruction(void)
{
    uint64 opcode;
    opcode = (memoryRead(reg[R_PC]) << 56) | (memoryRead(reg[R_PC] + 1) << 48)
    | (memoryRead(reg[R_PC] + 2) << 40) | (memoryRead(reg[R_PC] + 3) << 32)
    | (memoryRead(reg[R_PC] + 4) << 24) | (memoryRead(reg[R_PC] + 5) << 16)
    | (memoryRead(reg[R_PC] + 6) << 8) | (memoryRead(reg[R_PC] + 7));
    return (opcode);
}