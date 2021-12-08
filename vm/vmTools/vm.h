#ifndef VM_H
#define VM_H
#define RAM_SIZE 2147483647

typedef unsigned long int uint64;
typedef unsigned char byte;

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
    R_CR,   // carry register
    R_PC,   // program counter
    R_IR,   // Instruction Register
    R_SP,   // Stack Pointer
    R_LR,   // Link register
    R_COUNT
};

/*
    a          =   Register a (R_Ra)
    b          =   Register b (R_Rb)
    XX          =   8-bit constant
    NNNNNNNN    =   memory address
*/
enum {

    //general instructions
    OP_NOP = 0x0000000000000000, //0x0000000000000000   Do nothing
    OP_END = 0xFF00000000000000, //0xFF00000000000000   End of the VM

    //arithmetical instructions
    OP_ADD = 0x1000000000000000, //0x10ab000000000000   Add Ra and Rb in Ra
    OP_SUB = 0x1100000000000000, //0x11ab000000000000   Substract Rb to Ra, store res in Rn
    OP_SET = 0X1200000000000000, //0x12aXX00000000000   Set the Ra value to the constant
    OP_ADC = 0x1300000000000000, //0x13aXX00000000000   Add a constant to Ra (carry flag unchanged)
    OP_MOV = 0x1400000000000000, //0x14ab000000000000   Copy Ra value in Rb
    
    //flow instructions
    OP_JMP = 0x2000000000000000, //0x20NNNNNNNN000000   Jump to the address
    //OP_BRL = 0x2100000000000000, //0x21NNNNNNNN000000   Go to subroutine pointed by the address
    //OP_BRX = 0x2200000000000000, //0x2200000000000000   Return to the main routine

    //RAM instructions
    OP_ADR = 0x3000000000000000, //0x30NNNNNNNN000000   Set R_ADR to the address given
    OP_LDR = 0x3100000000000000, //0x31NNNNNNNNa00000   Load in Ra the value at the given addrqess
    OP_STR = 0x3200000000000000, //0x32NNNNNNNNa00000   Store in RAM the value of Ra in the given address and following
    OP_STO = 0x3300000000000000, //0x33NNNNNNNNa00000   Store a byte

    //conditionnal instructions
    OP_EQU = 0x4000000000000000, //0x40aXX00000000000   Skip the next instruction if Ra != XX
    OP_NEQ = 0x4100000000000000, //0x41aXX00000000000   Skip the next instruction if Ra == XX
    OP_REQ = 0x4200000000000000, //0x42ab000000000000   Skip the next instruction if Ra == Rb

    //bitwise operations
    OP_BOR = 0x5000000000000000, //0x50ab000000000000   Set Ra to Ra OR Rb (bitwise)
    OP_AND = 0x5100000000000000, //0x51ab000000000000   Set Ra to Ra AND Rb (bitwise)
    OP_XOR = 0x5200000000000000  //0x52ab000000000000   Set Ra to Ra XOR Rb (bitwise)
};

void init(void);
void loadProgram(void);
uint64 loadInstruction(void);
void shutdown(void);
uint64 memoryRead(uint64 adress);
void displayRegisters(void);
void memoryWrite(uint64 adress, unsigned char value);
#endif