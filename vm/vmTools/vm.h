#ifndef VM_H
#define VM_H
#define ULONG_MAX 4294967295

typedef unsigned long int uint32;

enum {
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_R8,
    R_R9,
    R_PC, // program counter
    R_IR, // Instruction Register
    R_PSR, //Processor status register
    R_COUNT
};

enum {
    OP_NOP = 0, // do nothing
    OP_END, // end of the VM
    OP_ADD, //Add R0 to R1, store res in R2
    OP_SUB //Substract R0 to R1, store res in R2 
};

void init(void);
void loadProgram(void);
void shutdown(void);
uint32 memoryRead(uint32 adress);
void memoryWrite(uint32 adress, uint32 value);
#endif