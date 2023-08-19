#include <stdio.h>
#include <stdint.h>
#include "decode_a32.c"
#include "elf_parser/elf_parser.c"

void test_get_elf_instruction() {
    u_int32_t* res = NULL;
    int size;
    // 不知道为什么第一次取不出来值，第二次才能取出来
    res = get_all_text_insts_fix32("./test/hello.arm", &size);
    printf("====\n");
    res = get_all_text_insts_fix32("./test/hello.arm", &size);
    printf("====\n");
    if(size) {
        for(int i = 0; i < 20; i++) {
            disas_arm_insn(*res);
            res++;
        }
    }
}


int main() {
    test_get_elf_instruction();
    // uint32_t inst = 0xE3A0F101; 
    // disas_arm_insn(inst);
    uint32_t inst2 = 0xE3A0F101; 
    inst2 = 0xE1510002;
    disas_arm_insn(inst2);
    printf("\n");
    printf("done!\n");
    return 1;
}