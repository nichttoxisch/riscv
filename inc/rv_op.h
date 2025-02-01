#ifndef RV_OP_H_
#define RV_OP_H_

#include "rv_arch.h"
#include "rv_instr.h"

uxlen_t rv_op_lui(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_auipc(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_jal(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_jalr(rv_instr_t instr, regs_t* regs);

uxlen_t rv_op_beq(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_bne(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_blt(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_bge(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_bltu(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_bgeu(rv_instr_t instr, regs_t* regs);

uxlen_t rv_op_lb(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_lh(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_lw(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_lbu(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_lhu(rv_instr_t instr, regs_t* regs);

uxlen_t rv_op_sb(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_sh(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_sw(rv_instr_t instr, regs_t* regs);

uxlen_t rv_op_addi(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_slti(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_sltiu(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_xori(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_ori(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_andi(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_slli(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_srli(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_srai(rv_instr_t instr, regs_t* regs);

uxlen_t rv_op_add(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_sub(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_sll(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_slt(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_sltu(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_xor(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_srl(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_sra(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_or(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_and(rv_instr_t instr, regs_t* regs);

uxlen_t rv_op_fence(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_fence_tso(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_pause(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_ecall(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_ebreak(rv_instr_t instr, regs_t* regs);

#ifdef RV_PRIV
uxlen_t rv_op_sret(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_mret(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_mnret(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_wfi(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_sfence_vma(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hfence_vvma(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hfence_gvma(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hlv_b(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hlv_bu(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hlv_h(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hlv_hu(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hlv_w(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hlvx_hu(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hlvx_wu(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hsv_b(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hsv_h(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hsv_w(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_sinval_vma(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_sfence_w_inval(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_sfence_inval_ir(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hinval_vvma(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hinval_gvma(rv_instr_t instr, regs_t* regs);
#ifdef RV_EXT_64
uxlen_t rv_op_hlv_wu(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hlv_d(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_hsv_d(rv_instr_t instr, regs_t* regs);
#endif
#endif

#ifdef RV_EXT_64
uxlen_t rv_op_lwu(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_ld(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_sd(rv_instr_t instr, regs_t* regs);
// uxlen_t rv_op_slli(rv_instr_t instr, regs_t* regs);
// uxlen_t rv_op_srli(rv_instr_t instr, regs_t* regs);
// uxlen_t rv_op_srai(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_addiw(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_slliw(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_srliw(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_sraiw(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_addw(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_subw(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_sllw(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_srlw(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_sraw(rv_instr_t instr, regs_t* regs);
#endif

#ifdef RV_EXT_ZICSR
uxlen_t rv_op_csrrw(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_csrrs(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_csrrc(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_csrrwi(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_csrrsi(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_csrrci(rv_instr_t instr, regs_t* regs);
#endif

#ifdef RV_EXT_M
uxlen_t rv_op_mul(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_mulh(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_mulhsu(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_mulhu(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_div(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_divu(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_rem(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_remu(rv_instr_t instr, regs_t* regs);
#ifdef RV_EXT_64
uxlen_t rv_op_mulw(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_divw(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_divuw(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_remw(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_remuw(rv_instr_t instr, regs_t* regs);
#endif
#endif

#ifdef RV_EXT_A
uxlen_t rv_op_lr_w(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_lc_w(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amoswap_w(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amoadd_w(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amoxor_w(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amoand_w(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amoor_w(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amomin_w(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amomax_w(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amominu_w(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amomaxu_w(rv_instr_t instr, regs_t* regs);
#ifdef RV_EXT_64
uxlen_t rv_op_lr_d(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_lc_d(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amoswap_d(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amoadd_d(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amoxor_d(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amoand_d(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amoor_d(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amomin_d(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amomax_d(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amominu_d(rv_instr_t instr, regs_t* regs);
uxlen_t rv_op_amomaxu_d(rv_instr_t instr, regs_t* regs);
#endif
#endif

#endif  // RV_OP_H_
