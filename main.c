#include <stdio.h>
#include <stdint.h>
#include "decode_a32.c"
#include "elf_parser/elf_parser.c"
/*capstone centos 安装 yum isntall capstone; yum install capstone-devel*/
#include <capstone/capstone.h>

int test_capstone(u_int32_t* code, int size) {
    csh handle;
	cs_insn *insn;
	size_t count;

	if (cs_open(CS_ARCH_ARM, CS_MODE_ARM | CS_MODE_THUMB, &handle) != CS_ERR_OK)
		return -1;
	count = cs_disasm(handle, (uint8_t*)code, size, 0x1000, 0, &insn);
	if (count > 0) {
		size_t j;
		for (j = 0; j < count; j++) {
			printf("0x%"PRIx64":\t%s\t\t%s\n", insn[j].address, insn[j].mnemonic,
					insn[j].op_str);
		}
		cs_free(insn, count);
	} else
		printf("ERROR: Failed to disassemble given code!\n");

	cs_close(&handle);
}

void test_get_elf_instruction() {
    u_int32_t* res = NULL;
    int size;
    // 不知道为什么第一次取不出来值，第二次才能取出来
    res = get_all_text_insts_fix32("./test/hello.arm", &size);
    printf("====\n");
    res = get_all_text_insts_fix32("./test/hello.arm", &size);
    printf("====\n");
    test_capstone(res, size * 4);
    if(size) {
        for(int i = 0; i < 20; i++) {
            printf("%x\n", *res);
            disas_arm_insn(*res);
            // test_capstone(res);
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