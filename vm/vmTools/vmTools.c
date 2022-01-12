#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "vm.h"

extern byte *memory;
extern uint64 *reg;
const char program[] = {0x30, 0x00, 0x00, 0xFF, 0xF0, 0x10, 0x00, 0x00,
                        0x12, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x10, 0x12, 0x00, 0x68, 0xab, 0x30, 0x00, 0x00,
                        0x30, 0x00, 0x00, 0xFF, 0xF0, 0x10, 0x00, 0x00,
                        0x12, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x10, 0x12, 0x00, 0x68, 0xab, 0x30, 0x00, 0x00,
                        0x00, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
int program_size = 64;

void init(void)
// initialize the vm
{
    printf("Initialization:\n----------\n");
    printf("[!]: Loading %d bytes of memory\n", RAM_SIZE);
    memory = malloc(RAM_SIZE * sizeof(byte));
    printf("[!]: Loading %d 32 bits registers\n", R_COUNT);
    reg = malloc(R_COUNT * sizeof(uint64));
    for (int i = 0; i < R_COUNT; i++)
    {
        printf("[!]: Initialize R%d at 0\n", i);
        reg[i] = 0;
    }
    printf("[!]: Loading program\n");
    loadProgram();
    printf("Loading stack pointer on 0xFFFFFFFF\n");
    reg[R_SP] = 0xFFFFFFFF;
    memory[0xFFF0] = 0xFF;
    memory[0xFFF1] = 0xFF;
    memory[0xFFF2] = 0xFF;
    memory[0xFFF3] = 0xFF;
    memory[0xFFF4] = 0xFF;
    memory[0xFFF5] = 0xFF;
    memory[0xFFF6] = 0xFF;
    memory[0xFFF7] = 0xFF;
    printf("\nReady !\n\n----------\n");
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
    printf("Freeing memory and registers\n");
    free(memory);
    free(reg);
    printf("Shutting down the VM\n");
    exit(0);
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
    printf("Displaying all registers:\n");
    for (int i = 0; i < R_COUNT; i++)
    {
        printf("R %d\t:\t0x%lX\n", i, reg[i]);
    }
    printf("\n");
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