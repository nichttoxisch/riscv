#include "rv_op.h"

#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>

#include "rv_syscall.h"

#define pc (regs->pc)
#define x (regs->x)
#define vmem (regs->vmem)

#ifdef RV_EXT_ZICSR
#define csr (regs->csr)
#define hartid (regs->hartid)
#endif

#define rd (instr.rd)
#define rs1 (instr.rs1)
#define rs2 (instr.rs2)
#define imm (instr.imm)

uxlen_t rv_op_lui(rv_instr_t instr, regs_t* regs) {
  x[rd] = imm;
  return pc + 4;
}

uxlen_t rv_op_auipc(rv_instr_t instr, regs_t* regs) {
  x[rd] = (xlen_t)pc + imm;
  return pc + 4;
}

uxlen_t rv_op_jal(rv_instr_t instr, regs_t* regs) {
  x[rd] = (xlen_t)(pc + 4);
  return (uxlen_t)((xlen_t)pc + imm);
}

uxlen_t rv_op_jalr(rv_instr_t instr, regs_t* regs) {
  x[rd] = (xlen_t)pc + 4;
  return (uxlen_t)(x[rs1] + imm);
}

uxlen_t rv_op_beq(rv_instr_t instr, regs_t* regs) {
  if (x[rs1] == x[rs2]) {
    return pc + (uxlen_t)imm;
  }
  return pc + 4;
}

uxlen_t rv_op_bne(rv_instr_t instr, regs_t* regs) {
  if (x[rs1] != x[rs2]) {
    return pc + (uxlen_t)imm;
  }
  return pc + 4;
}

uxlen_t rv_op_blt(rv_instr_t instr, regs_t* regs) {
  if (x[rs1] < x[rs2]) {
    return pc + (uxlen_t)imm;
  }
  return pc + 4;
}

uxlen_t rv_op_bge(rv_instr_t instr, regs_t* regs) {
  if (x[rs1] >= x[rs2]) {
    return pc + (uxlen_t)imm;
  }
  return pc + 4;
}

uxlen_t rv_op_bltu(rv_instr_t instr, regs_t* regs) {
  if ((u32)x[rs1] < (u32)x[rs2]) {
    return pc + (uxlen_t)imm;
  }
  return pc + 4;
}

uxlen_t rv_op_bgeu(rv_instr_t instr, regs_t* regs) {
  if ((u32)x[rs1] >= (u32)x[rs2]) {
    return pc + (uxlen_t)imm;
  }
  return pc + 4;
}

uxlen_t rv_op_lb(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_lh(rv_instr_t instr, regs_t* regs) {
  xlen_t addr = x[rs1] + imm;
  x[rd] = (xlen_t)(*(i16*)(&vmem[addr]));
  return pc + 4;
}

uxlen_t rv_op_lw(rv_instr_t instr, regs_t* regs) {
  xlen_t addr = x[rs1] + imm;
  x[rd] = (xlen_t)(*(i32*)(&vmem[addr]));
  return pc + 4;
}

uxlen_t rv_op_lbu(rv_instr_t instr, regs_t* regs) {
  xlen_t addr = x[rs1] + imm;
  x[rd] = (xlen_t)(*(u8*)(&vmem[addr]));
  return pc + 4;
}

uxlen_t rv_op_lhu(rv_instr_t instr, regs_t* regs) {
  xlen_t addr = x[rs1] + imm;
  x[rd] = (xlen_t)(*(u16*)(&vmem[addr]));
  return pc + 4;
}

uxlen_t rv_op_sb(rv_instr_t instr, regs_t* regs) {
  // printf("sb x%ld, %" FXD "(x%ld)\n", rs2, imm, rs1);
  xlen_t addr = x[rs1] + imm;
  // printf("addr: %" FXD "\n", addr);
  *(u8*)(&vmem[addr]) = (u8)(x[rs2] & 0xff);
  return pc + 4;
}

uxlen_t rv_op_sh(rv_instr_t instr, regs_t* regs) {
  xlen_t addr = x[rs1] + imm;
  *(u16*)(&vmem[addr]) = (u16)(x[rs2] & 0xffff);
  return pc + 4;
}

uxlen_t rv_op_sw(rv_instr_t instr, regs_t* regs) {
  xlen_t addr = x[rs1] + imm;
  *(u32*)(&vmem[addr]) = (u32)((u32)x[rs2] & 0xffffffff);
  return pc + 4;
}

uxlen_t rv_op_addi(rv_instr_t instr, regs_t* regs) {
  x[rd] = x[rs1] + imm;
  return pc + 4;
}

uxlen_t rv_op_slti(rv_instr_t instr, regs_t* regs) {
  x[rd] = (x[rs1] < imm) ? 1 : 0;
  return pc + 4;
}

uxlen_t rv_op_sltiu(rv_instr_t instr, regs_t* regs) {
  x[rd] = ((u32)x[rs1] < (u32)imm) ? 1 : 0;
  return pc + 4;
}

uxlen_t rv_op_xori(rv_instr_t instr, regs_t* regs) {
  x[rd] = x[rs1] ^ imm;
  return pc + 4;
}

uxlen_t rv_op_ori(rv_instr_t instr, regs_t* regs) {
  x[rd] = x[rs1] | imm;
  return pc + 4;
}

uxlen_t rv_op_andi(rv_instr_t instr, regs_t* regs) {
  x[rd] = x[rs1] & imm;
  return pc + 4;
}

uxlen_t rv_op_slli(rv_instr_t instr, regs_t* regs) {
  x[rd] = (xlen_t)((uxlen_t)x[rs1] << imm);
  return pc + 4;
}

uxlen_t rv_op_srli(rv_instr_t instr, regs_t* regs) {
  x[rd] = (xlen_t)((uxlen_t)x[rs1] >> imm);
  return pc + 4;
}

uxlen_t rv_op_srai(rv_instr_t instr, regs_t* regs) {
  x[rd] = x[rs1] >> imm;
  return pc + 4;
}

uxlen_t rv_op_add(rv_instr_t instr, regs_t* regs) {
  x[rd] = x[rs1] + x[rs2];
  return pc + 4;
}

uxlen_t rv_op_sub(rv_instr_t instr, regs_t* regs) {
  x[rd] = x[rs1] - x[rs2];
  return pc + 4;
}

uxlen_t rv_op_sll(rv_instr_t instr, regs_t* regs) {
  x[rd] = (xlen_t)((uxlen_t)x[rs1] << (uxlen_t)x[rs2]);
  return pc + 4;
}

uxlen_t rv_op_slt(rv_instr_t instr, regs_t* regs) {
  x[rd] = x[rs1] < x[rs2] ? 1 : 0;
  return pc + 4;
}

uxlen_t rv_op_sltu(rv_instr_t instr, regs_t* regs) {
  x[rd] = (uxlen_t)x[rs1] < (uxlen_t)x[rs2] ? 1 : 0;
  return pc + 4;
}

uxlen_t rv_op_xor(rv_instr_t instr, regs_t* regs) {
  x[rd] = x[rs1] ^ x[rs2];
  return pc + 4;
}

uxlen_t rv_op_srl(rv_instr_t instr, regs_t* regs) {
#ifdef RV_EXT_64
  x[rd] = (xlen_t)((uxlen_t)x[rs1] >> ((uxlen_t)x[rs2] & 0b111111));
#else
  x[rd] = (xlen_t)((uxlen_t)x[rs1] >> ((uxlen_t)x[rs2] & 0b11111));
#endif
  return pc + 4;
}

uxlen_t rv_op_sra(rv_instr_t instr, regs_t* regs) {
#ifdef RV_EXT_64
  x[rd] = (xlen_t)(x[rs1] >> (x[rs2] & 0b111111));
#else
  x[rd] = (xlen_t)(x[rs1] >> (x[rs2] & 0b11111));
#endif
  return pc + 4;
}

uxlen_t rv_op_or(rv_instr_t instr, regs_t* regs) {
  x[rd] = x[rs1] | x[rs2];
  return pc + 4;
}

uxlen_t rv_op_and(rv_instr_t instr, regs_t* regs) {
  x[rd] = x[rs1] & x[rs2];
  return pc + 4;
}

uxlen_t rv_op_fence(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_fence_tso(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_pause(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_ecall(rv_instr_t instr, regs_t* regs) {
  (void)instr;

  xlen_t a0 = x[0xa + 0];
  xlen_t a1 = x[0xa + 1];
  xlen_t a2 = x[0xa + 2];
  xlen_t a3 = x[0xa + 3];
  xlen_t a4 = x[0xa + 4];
  xlen_t a5 = x[0xa + 5];

  (void)a3;
  (void)a4;
  (void)a5;

  xlen_t a7 = x[0xa + 7];

  switch (a7) {
    case RV_SYS_close: {
      x[0xa + 0] = (xlen_t)close((int)a0);
    } break;
    case RV_SYS_write: {
      x[0xa + 0] = (xlen_t)write((int)a0, (void*)&vmem[a1], (usize)a2);
    } break;
    case RV_SYS_fstat: {
      struct stat buf;
      x[0xa + 0] = (xlen_t)fstat((int)a0, &buf);
    } break;
    case RV_SYS_exit: {
      _exit((int)a0);
    } break;
    case RV_SYS_brk: {
      // x[0xa + 0] = (xlen_t)brk((void*)&vmem[a0]);
      x[0xa + 0] = 0;
    } break;

    default: {
      assert(0);
    } break;
  }

  return pc + 4;
}
uxlen_t rv_op_ebreak(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

#ifdef RV_PRIV
uxlen_t rv_op_sret(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_mret(rv_instr_t instr, regs_t* regs) {
  (void)instr;

  printf("TODO: Proper implementation\n");
  return (uxlen_t)(csr[hartid][RV_CSR_MEPC]);
}

uxlen_t rv_op_mnret(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_wfi(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_sfence_vma(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hfence_vvma(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hfence_gvma(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hlv_b(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hlv_bu(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hlv_h(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hlv_hu(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hlv_w(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hlvx_hu(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hlvx_wu(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hsv_b(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hsv_h(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hsv_w(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

#ifdef RV_EXT_64
uxlen_t rv_op_hlv_wu(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hlv_d(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hsv_d(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

#endif
uxlen_t rv_op_sinval_vma(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_sfence_w_inval(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_sfence_inval_ir(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hinval_vvma(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_hinval_gvma(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}
#endif

#ifdef RV_EXT_64
uxlen_t rv_op_lwu(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_ld(rv_instr_t instr, regs_t* regs) {
  xlen_t addr = x[rs1] + imm;
  x[rd] = (xlen_t)(*(i64*)(&vmem[addr]));
  return pc + 4;
}

uxlen_t rv_op_sd(rv_instr_t instr, regs_t* regs) {
  xlen_t addr = x[rs1] + imm;
  *(u64*)(&vmem[addr]) = (u64)x[rs2];
  return pc + 4;
}

uxlen_t rv_op_addiw(rv_instr_t instr, regs_t* regs) {
  xlen_t a = sext((u32)x[rs1], 32);
  x[rd] = sext((u32)(a + imm), 32);
  return pc + 4;
}

uxlen_t rv_op_slliw(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_srliw(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_sraiw(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_addw(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_subw(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_sllw(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_srlw(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_sraw(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}
#endif

#ifdef RV_EXT_ZICSR
uxlen_t rv_op_csrrw(rv_instr_t instr, regs_t* regs) {
  x[rd] = csr[hartid][imm];
  csr[hartid][imm] = x[rs1];
  return pc + 4;
}

uxlen_t rv_op_csrrs(rv_instr_t instr, regs_t* regs) {
  xlen_t v = csr[hartid][imm];
  xlen_t m = x[rs1];
  csr[hartid][imm] = v | m;
  x[rd] = v;
  return pc + 4;
}

uxlen_t rv_op_csrrc(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_csrrwi(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_csrrsi(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_csrrci(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

#endif

#ifdef RV_EXT_M
uxlen_t rv_op_mul(rv_instr_t instr, regs_t* regs) {
  x[rd] = x[rs1] * x[rs2];
  return pc + 4;
}

uxlen_t rv_op_mulh(rv_instr_t instr, regs_t* regs) {
  x[rd] = (xlen_t)(((i64)x[rs1] * (i64)x[rs2]) >> 32);
  return pc + 4;
}

uxlen_t rv_op_mulhsu(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_mulhu(rv_instr_t instr, regs_t* regs) {
  x[rd] = (xlen_t)(((u64)x[rs1] * (u64)x[rs2]) >> 32);
  return pc + 4;
}

uxlen_t rv_op_div(rv_instr_t instr, regs_t* regs) {
  x[rd] = x[rs1] / x[rs2];
  return pc + 4;
}

uxlen_t rv_op_divu(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_rem(rv_instr_t instr, regs_t* regs) {
  x[rd] = x[rs1] % x[rs2];
  return pc + 4;
}

uxlen_t rv_op_remu(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

#ifdef RV_EXT_64
uxlen_t rv_op_mulw(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_divw(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_divuw(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_remw(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_remuw(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}
#endif
#endif

#ifdef RV_EXT_A
uxlen_t rv_op_lr_w(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_lc_w(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amoswap_w(rv_instr_t instr, regs_t* regs) {
  // xlen_t rl = (imm >> 0) & 1;
  // xlen_t aq = (imm >> 1) & 1;
  xlen_t addr = x[rs1];
  i32 v = *(i32*)(&vmem[addr]);
  *(i32*)(&vmem[addr]) = (i32)x[rs2];
  x[rd] = (xlen_t)v;

  return pc + 4;
}
uxlen_t rv_op_amoadd_w(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amoxor_w(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amoand_w(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amoor_w(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amomin_w(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amomax_w(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amominu_w(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amomaxu_w(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

#ifdef RV_EXT_64
uxlen_t rv_op_lr_d(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_lc_d(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amoswap_d(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}
uxlen_t rv_op_amoadd_d(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amoxor_d(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amoand_d(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amoor_d(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amomin_d(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amomax_d(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amominu_d(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}

uxlen_t rv_op_amomaxu_d(rv_instr_t instr, regs_t* regs) {
  (void)instr;
  (void)regs;
  assert(0);
}
#endif
#endif