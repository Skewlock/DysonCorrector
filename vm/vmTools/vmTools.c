#include <unistd.h>
#include <stdlib.h>
#include "vm.h"

extern uint32 *memory;
extern uint32 *reg;
const int program[] = {0, 0, 0, 0, 0, 1};
int program_size = 6;

void init(void)
// initialize the vm
{
    memory = malloc(ULONG_MAX);
    reg = malloc(R_COUNT);
    reg[R_PC] = 0;
    loadProgram();
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

uint32 memoryRead(uint32 adress)
// Read the value at the RAM adress
{
    return (memory[adress]);
}

void memoryWrite(uint32 adress, uint32 value)
// write the given value in the adress
{
    memory[adress] = value;
}