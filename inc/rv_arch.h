#ifndef RV_ARCH_H_
#define RV_ARCH_H_

#include <elf.h>

#include "stdr.h"

#define RV_CSR_MHARTID 0xf14
#define RV_CSR_MEPC 0x341
#define RV_CSR_COUNT 4096

#ifdef RV_EXT_64
#define RV_XLEN 64
typedef i64 xlen_t;
typedef u64 uxlen_t;
#define FXX "lx"
#define FXD "ld"
#else
#define RV_XLEN 32
typedef i32 xlen_t;
typedef u32 uxlen_t;
#define FXX "x"
#define FXD "d"
#endif

#ifdef RV_EXT_64
typedef Elf64_Ehdr rv_Elf_Ehdr;
typedef Elf64_Shdr rv_Elf_Shdr;
typedef Elf64_Phdr rv_Elf_Phdr;

#else
typedef Elf32_Ehdr rv_Elf_Ehdr;
typedef Elf32_Shdr rv_Elf_Shdr;
typedef Elf32_Phdr rv_Elf_Phdr;
#endif

#define RV_REGS_COUNT 32

typedef struct {
  uxlen_t pc;
  xlen_t x[RV_REGS_COUNT];
  u8* vmem;

#ifdef RV_EXT_ZICSR
  uxlen_t hartid;
  xlen_t** csr;
#endif
} regs_t;

xlen_t sext(u32 v, u8 bits);

#endif  // RV_ARCH_H_

#ifdef RV_ARCH_IMPLEMENTATION

xlen_t sext(u32 v, u8 bits) {
  xlen_t shift = 32 - bits;
  return (xlen_t)((i32)(v << shift) >> shift);
}

#undef RV_ARCH_IMPLEMENTATION
#endif  // RV_ARCH_IMPLEMENTATION