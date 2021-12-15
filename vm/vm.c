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
                // check the carry
                if (reg[(reg[R_IR] & 0x00F0000000000000) >> 52] > (0xFF - ((reg[R_IR] & 0x000F000000000000) >> 48)))
                    reg[R_CR] = 1; // carry register
                else 
                    reg[R_CR] = 0;
                reg[(reg[R_IR] & 0x00F0000000000000) >> 52] += reg[(reg[R_IR] & 0x000F000000000000) >> 48];
                break;
            
            case OP_SUB:
                // check the carry
                if (reg[(reg[R_IR] & 0x00F0000000000000) >> 52] < reg[(reg[R_IR] & 0x000F000000000000) >> 48])
                    reg[R_CR] = 1; // carry register
                else
                    reg[R_CR] = 0;
                reg[(reg[R_IR] & 0x00F0000000000000) >> 52] -= reg[(reg[R_IR] & 0x000F000000000000) >> 48]; 
                break;
            
            case OP_SET:
                reg[(reg[R_IR] & 0x00F0000000000000) >> 52] = ((reg[R_IR] & 0x000FF00000000000) >> 44);
                break;

            case OP_ADC:
                reg[(reg[R_IR] & 0x00F0000000000000) >> 52] += (reg[R_IR] & 0x000FF00000000000) >> 44;
                break;
            
            case OP_ADR:
                reg[R_ADR] = (reg[R_IR] & 0x00FFFFFFFF000000) >> 24;
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

            case OP_STO:
                memoryWrite((reg[R_IR] & 0x00FFFFFFFF000000) >> 24, reg[(reg[R_IR] & 0x0000000000F00000) >> 20]);
                break;
            
            case OP_LDO:
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
                printf("Unknown OpCode: 0x%lX\n", reg[R_IR]);
                exit(0);
                break;
        }
        displayRegisters();
        reg[R_PC] += 8;
    }
    //shutdown de la vm
    shutdown();
}