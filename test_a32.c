#include <stdio.h>
#include <stdint.h>
#include "decode_a32.c"

int main() {
    // uint32_t inst = 0xE3A0F101; 
    // disas_arm_insn(inst);
    uint32_t inst2 = 0xE0412003; 
    disas_arm_insn(inst2);
    printf("\n");
    printf("done!\n");
    return 1;
}