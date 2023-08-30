# Tiger指令解析器

## 提供的接口

* 多条指令解析
decode_muti_inst()

* 单条指令解析
decode_inst()

* elf文件解析 decode_elf()
    * 主要用于测试


## 代码生成选项
* python参数 --decode_content=opcode,operands,reg_info
    * python3 gen_decoder.py --decode_content=decode // 仅解析opcode
    * python3 gen_decoder.py --decode_content=operands // 仅解析operands
    * python3 gen_decoder.py --decode_content=opcode，operands // 解析opcode和operands

* 默认选项生成的指令解析器会解析所有的指令信息

## 目录结构
```
├── common
│   └── elf_parser      // 解析elf文件的工具
├── generated           // 存放生成的代码位置
│   ├── decode-a32.inc
│   └── decode-a32-uncond.inc
├── isa                 // 存放指令解析描述
│   ├── arm             // arm指令解析描述
│   │   ├── a32.isa
│   │   └── a32-uncond.isa
│   └── riscv           // riscv指令解析描述
│       ├── riscv16.isa
│       └── riscv32.isa
├── isa_parser          // isa parser
│   └── gen_decoder.py
├── include          // 使用本项目需要引入的头文件
│   └── tiger_decoder.h
├── readme.md
├── isa_parser          // 一些脚本
│   │── run_isa.sh
│   └── run_main.sh
├── spec_2006_arm       // spec 2006 文件
├── SPEC2006_ARM.tar
├── test                // 一些测试文件
│   ├── run_spec.c
│   └── main.c
└── wrapper             // 指令解析器wrapper，包含预处理器
    ├── decode_arm.c
    └── decode_riscv.c
```

## capstone安装
capstone centos 安装 
```
yum isntall capstone;
yum install capstone-devel
```


使用过程
```
#include <capstone/capstone.h>
```
