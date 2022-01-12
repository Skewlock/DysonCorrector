#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "vmTools/vm.h"

byte *memory;
uint64 *reg;

int main(void)
{
    //init de la vm
    init();

    int running = 1;
    while (running)
    {
        reg[R_IR] = loadInstruction();
        printf("Executing instruction: 0x%lX\n", reg[R_IR]);
        switch (reg[R_IR] & 0xFF00000000000000)
        {
            case OP_NOP:
                break;

            case OP_END:
                running = 0;
                break;

            case OP_ADD:
                //C
                if (reg[(reg[R_IR] & 0x00F0000000000000) >> 52] > (0xFFFFFFFFFFFFFFFF - (reg[(reg[R_IR] & 0x000F000000000000) >> 48])))
                    reg[R_SR] = reg[R_SR] | 0x0000000000000002;
                else
                    reg[R_SR] = reg[R_SR] & 0xFFFFFFFFFFFFFFFD;

                reg[(reg[R_IR] & 0x00F0000000000000) >> 52] += reg[(reg[R_IR] & 0x000F000000000000) >> 48];

                //Z
                if ((reg[(reg[R_IR] & 0x00F0000000000000) >> 52] == 0) && ((reg[R_SR] & 0x0000000000000002) == 0))
                    reg[R_SR] = reg[R_SR] | 0x0000000000000004;
                else
                    reg[R_SR] = reg[R_SR] & 0xFFFFFFFFFFFFFFFB;

                //N
                if ((reg[(reg[R_IR] & 0x00F0000000000000) >> 52] & 0x8000000000000000) >> 63 == 1)
                    reg[R_SR] = reg[R_SR] | 0x0000000000000008;
                else
                    reg[R_SR] = reg[R_SR] & 0xFFFFFFFFFFFFFFF7;
                
                //V
                
                break;
            
            case OP_SUB:
                // check the carry
                if (reg[(reg[R_IR] & 0x00F0000000000000) >> 52] < reg[(reg[R_IR] & 0x000F000000000000) >> 48])
                    reg[R_SR] = reg[R_SR] | (1 << 2);
                else
                    reg[R_SR] = reg[R_SR] & (0 << 2);
                reg[(reg[R_IR] & 0x00F0000000000000) >> 52] -= reg[(reg[R_IR] & 0x000F000000000000) >> 48]; 
                break;
            
            case OP_SET:
                reg[(reg[R_IR] & 0x00F0000000000000) >> 52] = ((reg[R_IR] & 0x000FF00000000000) >> 44);
                break;

            case OP_ADC:
                reg[(reg[R_IR] & 0x00F0000000000000) >> 52] += (reg[R_IR] & 0x000FF00000000000) >> 44;
                break;

            case OP_MOV:
                reg[(reg[R_IR] & 0x0010000000000000) >> 52] = reg[(reg[R_IR] & 0x0001000000000000) >> 48];
                break;

            case OP_JMP:
                reg[R_PC] = ((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) - 8; // -8 because PC still increment in the loop
                break;

            case OP_BRL:
                reg[R_LR] = reg[R_PC] + 8;
                reg[R_PC] = ((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) - 8; // -8 for the same reasons

                //save the context after the jump. save the LR and the Register used
                break;
            
            case OP_BRX:
                //restore the context saved before calling that.
                reg[R_PC] = reg[R_LR];
                break;
            
            case OP_LDR:
                reg[(reg[R_IR] & 0x0000000000F00000) >> 20] = (memoryRead((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) << 56 
                | memoryRead(((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) + 1) << 48 | memoryRead(((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) + 2) << 40
                | memoryRead(((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) + 3) << 32 | memoryRead(((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) + 4) << 24
                | memoryRead(((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) + 5) << 16 | memoryRead(((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) + 6) << 8
                | memoryRead(((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) + 7));
                break;
            
            case OP_STR:
                memoryWrite((reg[R_IR] & 0x00FFFFFFFF000000) >> 24, reg[(reg[R_IR] & 0x0000000000F00000) >> 20] >> 56);
                memoryWrite(((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) + 1, reg[(reg[R_IR] & 0x0000000000F00000) >> 20] >> 48);
                memoryWrite(((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) + 2, reg[(reg[R_IR] & 0x0000000000F00000) >> 20] >> 40);
                memoryWrite(((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) + 3, reg[(reg[R_IR] & 0x0000000000F00000) >> 20] >> 32);
                memoryWrite(((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) + 4, reg[(reg[R_IR] & 0x0000000000F00000) >> 20] >> 24);
                memoryWrite(((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) + 5, reg[(reg[R_IR] & 0x0000000000F00000) >> 20] >> 16);
                memoryWrite(((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) + 6, reg[(reg[R_IR] & 0x0000000000F00000) >> 20] >> 8);
                memoryWrite(((reg[R_IR] & 0x00FFFFFFFF000000) >> 24) + 7, reg[(reg[R_IR] & 0x0000000000F00000) >> 20]);
                break;

            case OP_STB:
                memoryWrite((reg[R_IR] & 0x00FFFFFFFF000000) >> 24, (reg[(reg[R_IR] & 0x0000000000F00000) >> 20] & 0x00000000000000FF));
                break;
            
            case OP_LDB:
                reg[(reg[R_IR] & 0x0000000000F00000) >> 20] = memoryRead(reg[R_IR] & 0x00FFFFFFFF000000);
                break;

            case OP_EQU:
                if (reg[(reg[R_IR] & 0x00F0000000000000) >> 52] != (reg[R_IR] & 0x000FF00000000000) >> 44)
                    reg[R_PC] += 8;
                break;
            
            case OP_NEQ:
                if (reg[(reg[R_IR] & 0x00F0000000000000) >> 52] == (reg[R_IR] & 0x000FF00000000000) >> 44)
                    reg[R_PC] += 8;
                break;
            
            case OP_REQ:
                if (reg[(reg[R_IR] & 0x00F0000000000000) >> 52] != reg[(reg[R_IR] & 0x000F000000000000) >> 48])
                    reg[R_PC] += 8;
                break;

            case OP_BOR:
                reg[(reg[R_IR] & 0x00F0000000000000) >> 52] |= reg[(reg[R_IR] & 0x000F000000000000) >> 48];
                break;
            
            case OP_AND:
                reg[(reg[R_IR] & 0x00F0000000000000) >> 52] &= reg[(reg[R_IR] & 0x000F000000000000) >> 48];
                break;

            case OP_XOR:
                reg[(reg[R_IR] & 0x00F0000000000000) >> 52] ^= reg[(reg[R_IR] & 0x000F000000000000) >> 48];
                break;

            default:
                fprintf(stderr, "Error: unknown opcode: 0x%lX\nShutting down the vm.\n", reg[R_IR]);
                shutdown();
                break;
        }
        displayRegisters();
        reg[R_PC] += 8;
    }
    //shutdown de la vm
    shutdown();
}