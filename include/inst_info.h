#include <stdint.h>

typedef struct {
    char name[16];  // 操作数名称
    int value;      // 操作数数值 
} op_t;
// 指令信息结构
typedef struct {
    char mnemonic[32];              // 指令助记符
    uint8_t size;                   // 指令字节长度
    uint8_t op_count;               // 操作数数量
    uint8_t src_reg_cnt;            // 源寄存器数量
    uint8_t dst_reg_cnt;            // 目标寄存器数量
    op_t operands[36];              // 操作数列表
} inst_info;
