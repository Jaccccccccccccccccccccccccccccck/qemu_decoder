/*
 * decodeELF function implementation
 */
#include <array>
#include <string> 
#include <err.h>
#include <fcntl.h>
#include <gelf.h>
#include <libelf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <bsd/vis.h>
#include <string.h>
#include <vector> 



/*
get 32-bits insts from elf .text
*/
void
get_all_text_insts_fix32(const char *file_path, std::vector<u_int32_t>& res) {
    int fd;
    Elf* e;
    char *name, pc[4 * sizeof(char)];
    uint32_t *p;
    Elf_Scn* scn;
    Elf_Data* data;

    GElf_Shdr shdr;
    size_t n, shstrndx, sz;
    if (elf_version(EV_CURRENT) == EV_NONE)
        errx(EX_SOFTWARE,
             " ELF library initialization "
             " failed : % s ",
             elf_errmsg(-1));
    if ((fd = open(file_path, O_RDONLY, 0)) < 0)
        err(EX_NOINPUT, " open \" %s \" failed ", file_path);
    if ((e = elf_begin(fd, ELF_C_READ, NULL)) == NULL)
        errx(EX_SOFTWARE, " elf_begin () failed : % s . ",
             elf_errmsg(-1));
    if (elf_kind(e) != ELF_K_ELF)
        errx(EX_DATAERR, " % s is not an ELF object . ",
             file_path);
    if (elf_getshdrstrndx(e, &shstrndx) != 0)
        errx(EX_SOFTWARE, " elf_getshdrstrndx () failed : % s . ",
               elf_errmsg(-1));
    scn = NULL;
    while ((scn = elf_nextscn(e, scn)) != NULL) {
        if (gelf_getshdr(scn, &shdr) != &shdr) 
            errx(EX_SOFTWARE, " getshdr () failed : % s . ", elf_errmsg(-1));
        if ((name = elf_strptr(e, shstrndx, shdr.sh_name)) == NULL)
            errx(EX_SOFTWARE, " elf_strptr () failed : % s . ",
                   elf_errmsg(-1));
        // (void)printf(" Section %-4.4jd %s \n ", (uintmax_t)elf_ndxscn(scn), name);
        data = elf_getdata(scn, data);
        // if (shdr.sh_flags & 0x4) {
        //     printf("is a executable section \n");
        // } else {
        //     printf("is not a executable section \n");
        // }
        n = 0;
        if (strcmp(name, ".text") == 0 && data != NULL) {
            p = (u_int32_t *) data->d_buf;
            while ((((char *)p)<(char*)data -> d_buf + data->d_size)) {
                res.push_back(*p);
                p++;
            }
        }
    }

    if ((scn = elf_getscn(e, shstrndx)) == NULL)
        errx(EX_SOFTWARE, " getscn () failed : % s . ",
               elf_errmsg(-1));
    if (gelf_getshdr(scn, &shdr) != &shdr)
        errx(EX_SOFTWARE, " getshdr ( shstrndx ) failed : % s . ",
             elf_errmsg(-1));
    (void)putchar('\n');
    (void)elf_end(e);
    (void)close(fd);
}

std::vector<StaticInstPtr>
Decoder::decode_elf_arm(const char *file_path)
{
    std::vector<u_int32_t> insts;
    std::cout << "get inst from elf file: " << file_path << std::endl;
    get_all_text_insts_fix32(file_path, insts);
    std::vector<StaticInstPtr> res(insts.size());
    for(int i = 0; i < insts.size(); i++) {
        res[i] = gem5::ArmISA::Decoder::decode_inst(insts[i]);
    }
    for (int i = 0; i < 1000 && i < insts.size(); i++) {
        if(res[i]->isMemRef()) {
            std::cout << i << " : " << std::hex << insts[i] << " " << res[i]->getName() <<  " is mem " << std::endl;
        }
    }
    return res;
}

std::vector<MemInfo*>
Decoder::decode_elf_arm_mem_info(const char *file_path)
{
    std::vector<u_int32_t> insts;
    std::cout << "get inst from elf file: " << file_path << std::endl;
    get_all_text_insts_fix32(file_path, insts);
    std::vector<MemInfo*> res(insts.size());
    for(int i = 0; i < insts.size(); i++) {
        res[i] = gem5::ArmISA::Decoder::decode_mem(insts[i]);
    }
    for (int i = 0; i < 1000 && i < insts.size(); i++) {
        if(res[i]->isMem) {
            std::cout << i << " : " << std::hex << insts[i] << " " <<  " is mem " << std::endl;
        }
    }
    return res;
}

void
Decoder::compare_decode_mem_info(const char *file_path)
{
    std::vector<u_int32_t> insts;
    std::cout << "get inst from elf file: " << file_path << std::endl;
    get_all_text_insts_fix32(file_path, insts);
    std::vector<MemInfo*> res_mem_info(insts.size());
    std::vector<StaticInstPtr> res_inst(insts.size());
    for(int i = 0; i < insts.size(); i++) {
        res_mem_info[i] = gem5::ArmISA::Decoder::decode_mem(insts[i]);
        res_inst[i] = gem5::ArmISA::Decoder::decode_inst(insts[i]);
    }
    for (int i = 0; i < 1000 && i < insts.size(); i++) {
        if(res_mem_info[i]->isMem != res_inst[i]->isMemRef()) {
            std::cout << i << " : " << std::hex << insts[i] << " inst " << res_inst[i]->getName() <<  ", mem info is_mem: " << res_mem_info[i]->isMem <<
            ", inst info is_mem:" << res_inst[i]->isMemRef() << std::endl;
            std::cout << res_inst[i]->isMacroop() << std::endl;
        }
    }
}
