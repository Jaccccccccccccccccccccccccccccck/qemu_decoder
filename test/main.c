#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include "decode_a32.c"
#include "elf_parser/elf_parser.c"
/*capstone centos 安装 yum isntall capstone; yum install capstone-devel*/
#include <capstone/capstone.h>

int test_capstone(u_int32_t* code, int size) {
    csh handle;
	cs_insn *insn;
	size_t count;

	if (cs_open(CS_ARCH_ARM, CS_MODE_ARM, &handle) != CS_ERR_OK)
		return -1;
	count = cs_disasm(handle, (uint8_t*)code, size, 0x1000, 0, &insn);
	if (count > 0) {
		size_t j;
		for (j = 0; j < count; j++) {
			// printf("0x%"PRIx64":\t%s\t\t%s\n", insn[j].address, insn[j].mnemonic,
			// 		insn[j].op_str);
            printf("%s\n", insn[j].mnemonic);
		}
		cs_free(insn, count);
	} else
		printf("ERROR: Failed to disassemble given code!\n");

	cs_close(&handle);
}

char* capstone_get_opcode_one_inst(u_int32_t* code, char* opcode) {
    csh handle;
	cs_insn *insn;
	size_t count;

	if (cs_open(CS_ARCH_ARM, CS_MODE_ARM, &handle) != CS_ERR_OK)
		return (char*)-1;
	count = cs_disasm(handle, (uint8_t*)code, 4, 0x1000, 0, &insn);
    // opcode = "invalid";
	if (count == 1) {
        // printf("111");
        strcpy(opcode, insn[0].mnemonic);
		cs_free(insn, count);
	} else
		printf("ERROR: Failed to disassemble given code!\n");

	cs_close(&handle);
    return opcode;
}

char* processString(char* input) {
    // 寻找字符串中是否存在'_'
    const char* underscore = strchr(input, '_');
    
    if (underscore != NULL) {
        // 计算 '_' 的位置
        size_t position = underscore - input;
        
        // 创建一个新的字符串来存储 '_' 前的字符
        char* result = (char*)malloc(32); // 需要包含字符串结束符 '\0'
        strncpy(result, input, position); // 复制 '_' 前的部分
        result[position] = '\0'; // 添加字符串结束符
        
        return result;
    } else {
        // 如果没有找到 '_', 直接返回原字符串
        return input; // 使用 strdup 复制原字符串
    }
}

void toLowerCase(char* str) {
    while (*str) {
        *str = tolower(*str);
        str++;
    }
}

void pirnt_diff(char* str1, char* str2) {
    if(strcmp(str1, str2)) {
        printf("%s %s", str1, str2);
    }
}

void test_get_elf_instruction() {
    u_int32_t* res = NULL;
    int size;
    // 不知道为什么第一次取不出来值，第二次才能取出来
    res = get_all_text_insts_fix32("./spec_2006_arm/400.perlbench/exe/perlbench_base.arm-static", &size);
    printf("====\n");

    int equal_count = 0, not_equal_count = 0;
    if(size) {
        for(int i = 0; i < size; i++) {
            
            u_info* u = disas_arm_insn(*res);
            toLowerCase(u->f_empty.opcode);
            // printf("%s  ", processString(u->f_empty.opcode));
            char* opcode1 = processString(u->f_empty.opcode);
            
            char opcode2[30];
            capstone_get_opcode_one_inst(res, opcode2);
            
            if(strcmp(opcode1, opcode2)) {
                not_equal_count++;
                printf("%x :", *res);
                printf("%s ", opcode1);
                printf("%s\n", opcode2);
            } else {
                equal_count++;
            }
            res++;
        }
    }
    printf("total: %d, equal: %d, not_equal: %d\n", size, equal_count, not_equal_count);
}

int main() {
    test_get_elf_instruction();
    // uint32_t inst = 0xE3A0F101; 
    // disas_arm_insn(inst);
    uint32_t inst2 = 0xE3A0F101; 
    inst2 = 0xeaffffe0;
    disas_arm_insn(inst2);
    printf("\n");
    printf("done!\n");
    return 1;
}