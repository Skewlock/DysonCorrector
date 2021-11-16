#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "vmTools/vm.h"

uint32 *memory;
uint32 *reg;

int main(int argc, char *argv[])
{
    //init de la vm
    init();

    int running = 1;
    while (running)
    {
        reg[R_IR] = memoryRead(reg[R_PC]);
        switch (reg[R_IR])
        {
            case OP_NOP:
                break;

            case OP_END:
                running = 0;
                break;

            case OP_ADD:
                reg[R_R2] = reg[R_R0] + reg[R_R1];
                break;
            
            case OP_SUB:
                reg[R_R2] = reg[R_R0] - reg[R_R1];
                break;
                
            default:
                printf("error.");
                abort();
                break;
        }
        printf("%d\n", reg[R_IR]);
        reg[R_PC]++;
    }

    //shutdown de la vm
    shutdown();
}