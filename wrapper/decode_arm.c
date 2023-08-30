#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../generated/decode-a32-1.inc"
#include "../generated/decode-a32-2.inc"

#define true 1
#define false 0

typedef int bool;
typedef int bool;

static inline uint32_t extract32(uint32_t value, int start, int length)
{
    // assert(start >= 0 && length > 0 && length <= 32 - start);
    return (value >> start) & (~0U >> (32 - length));
}

static inline int32_t sextract32(uint32_t value, int start, int length)
{
    // assert(start >= 0 && length > 0 && length <= 32 - start);
    /* Note that this implementation relies on right shift of signed
     * integers being an arithmetic shift.
     */
    return ((int32_t)(value << (32 - length - start))) >> (32 - length);
}

static inline uint32_t deposit32(uint32_t value, int start, int length,
                                 uint32_t fieldval)
{
    uint32_t mask;
    // assert(start >= 0 && length > 0 && length <= 32 - start);
    mask = (~0U >> (32 - length)) << start;
    return (value & ~mask) | ((fieldval << start) & mask);
}

static inline int negate(int x)
{
    return -x;
}

static inline int plus_1(int x)
{
    return x + 1;
}

static inline int plus_2(int x)
{
    return x + 2;
}

static inline int times_2(int x)
{
    return x * 2;
}

static inline int times_4(int x)
{
    return x * 4;
}

static inline int times_2_plus_1(int x)
{
    return x * 2 + 1;
}

static inline int rsub_64(int x)
{
    return 64 - x;
}

static inline int rsub_32(int x)
{
    return 32 - x;
}

static inline int rsub_16(int x)
{
    return 16 - x;
}

static inline int rsub_8(int x)
{
    return 8 - x;
}


static u_info* decode_inst(unsigned int insn)
{
    return disas_a32(insn);
}