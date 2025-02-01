#ifndef RV_INSTR_H_
#define RV_INSTR_H_

#include "rv_arch.h"
#include "stdr.h"

struct s_rv_instr_t;

typedef uxlen_t (*rv_operation_f)(struct s_rv_instr_t, regs_t* x);

typedef struct s_rv_instr_t {
  u32 opcode;
  usize rd;
  usize rs1;
  usize rs2;
  xlen_t imm;

  rv_operation_f op;
} rv_instr_t;

rv_instr_t rv_instr_parse(u32 instruction);

#endif  // RV_INSTR_H_

#ifdef RV_INSTR_IMPLEMENTATION


#undef RV_INSTR_IMPLEMENTATION
#endif  // RV_INSTR_IMPLEMENTATION