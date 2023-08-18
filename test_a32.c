#include <stdio.h>
#include <stdint.h>
#include "decode_a32.c"

int main() {
    uint32_t inst = 0xE3A0F101; 
    disas_arm_insn(inst);
    printf("done.\n");
    return 1;
}