
#include "rv_instr.h"
#include "stdr.h"

#define RV_ARCH_IMPLEMENTATION
#include "rv_arch.h"

#define RV_ELF_IMPLEMENTATION
#include "rv_elf.h"

#ifdef RV_EXT_ZICSR
#define RV_NHARTS 4
xlen_t csr[RV_NHARTS][RV_CSR_COUNT];
#endif

#define VMEM_CAP (1024 * 1024)

i32 main(i32 argc, const cstr_t argv[]) {
  if (argc <= 1) {
    printf("Usage: %s <filename>\n", argv[0]);
    exit(1);
  }
  const cstr_t filename = argv[1];
  u8* vmem = malloc(VMEM_CAP);

  memset(vmem, 0xfa, VMEM_CAP);  // For debugging reasons

  rv_elf_t elf = rv_elf_load(filename);

  for (usize i = 0; i < elf.sections_count; i++) {
    rv_elf_section_t section = elf.sections[i];
    memset(&vmem[section.addr], 0, section.size);
    memcpy(&vmem[section.addr], section.data, section.data_size);
  }

  rv_elf_free(elf);

  regs_t regs = {0};
  regs.pc = elf.entry;
  regs.x[0] = 0;
  regs.x[2] = VMEM_CAP - 8;
  regs.x[10] = 0;

  regs.vmem = vmem;

#ifdef RV_EXT_ZICSR
  for (usize i = 0; i < RV_NHARTS; i++) {
    regs.csr[i][RV_CSR_MHARTID] = 0;
  }
#endif

  usize ii = 0;
  while (1) {
    regs.x[0] = 0;

    u32 instruction = *(uint32_t*)&vmem[regs.pc];
#ifdef RV_EXT_64
    printf("%04ld %08lx %08x\n", ii, regs.pc, instruction);
#else
    printf("%04ld %08x %08x\n", ii, regs.pc, instruction);
#endif

    rv_instr_t instr = rv_instr_parse(instruction);
    regs.pc = instr.op(instr, &regs);

    ii++;
  }

  return 0;
}
