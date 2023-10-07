#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include "wrapper/decode_arm.c"
#include "common/elf_parser/elf_parser.c"
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
            // printf("%s\n", insn[j].mnemonic);
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
    res = get_all_text_insts_fix32("./spec_2006_arm/400.perlbench/exe/perlbench_base.arm-static", &size);
    printf("====\n");

    int equal_count = 0, not_equal_count = 0;
    if(size) {
        for(int i = 0; i < size; i++) {
            
            u_info* u = decode_inst(*res);
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
}

// 测试是否可以同时处理arm32和thumb指令？
void test_capstone_arm_thumb() {
    u_int32_t* res = NULL;
    int size;
    res = get_all_text_insts_fix32("./test/hello.arm", &size);
    printf("====\n");

    int equal_count = 0, not_equal_count = 0;
    test_capstone(res, size);
}

// 1. **MOV 指令（数据传输）：** - 指令：`MOV R0, #10` - 编码：`E3A0000A`
// 2. **ADD 指令（算术操作）：** - 指令：`ADD R1, R2, R3` - 编码：`E0801003`
// 3. **SUB 指令（算术操作）：** - 指令：`SUB R4, R4, #1` - 编码：`E2444001`
// 4. **LDR 指令（加载数据）：** - 指令：`LDR R5, [R6, #4]` - 编码：`E5965004`
// 5. **STR 指令（存储数据）：** - 指令：`STR R7, [R8, #-8]` - 编码：`E7887008`
// 6. **CMP 指令（比较操作）：** - 指令：`CMP R9, #0` - 编码：`E3590000`
// 7. **B 指令（分支）：     ** - 指令：`B label` - 编码：`EAFF0010`
// 8. **BL 指令（分支并链接，用于函数调用）：** - 指令：`BL function_address`  - 编码：`EB000020`
// 9. **AND 指令（逻辑操作）：** - 指令：`AND R10, R11, R12` - 编码：`E001100C`
// 10. **ORR 指令（逻辑操作）：** - 指令：`ORR R13, R13, #0xFF` - 编码：`E38D0FFF`

int main() {
    // test_get_elf_instruction();
    test_capstone_arm_thumb();
    // 测试ARM的指令
    // uint32_t insts[10];
    // insts[0] = 0xE3A0000A; // MOV 指令（数据传输）
    // insts[1] = 0xE0801003; // ADD 指令（算术操作）
    // insts[2] = 0xE2444001; // SUB 指令（算术操作）
    // insts[3] = 0xE5965004; // LDR 指令（加载数据）
    // insts[4] = 0xE7887008; // STR 指令（存储数据）
    // insts[5] = 0xE3590000; // CMP 指令（比较操作）
    // insts[6] = 0xEAFF0010; // B 指令（分支）
    // insts[7] = 0xEB000020; // BL 指令（分支并链接，用于函数调用）
    // insts[8] = 0xE001100C; // AND 指令（逻辑操作）
    // insts[9] = 0xE38D0FFF; // ORR 指令（逻辑操作）
    // for(int i = 0; i < 10; i++) {
    //     u_info* u  = decode_inst(insts[i]);
    //     printf("%x : %s\n", insts[i], processString(u->f_empty.opcode));
    // }
    // return 1;
}