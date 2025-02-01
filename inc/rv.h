#ifndef RV_H_
#define RV_H_

#include "stdr.h"

#ifdef RV_RV64I
#define XLEN 64
typedef i64 xlen_t;
typedef u64 uxlen_t;
#else
#define XLEN 32
typedef i32 xlen_t;
typedef u32 uxlen_t;
#endif

typedef enum {
  RV_X0,
  RV_RA,
  RV_SP,
  RV_TP,
  RV_GP,
  RV_T0,
  RV_T1,
  RV_T2,
  RV_S0,
  RV_S1,
  RV_A0,
  RV_A1,
  RV_A2,
  RV_A3,
  RV_A4,
  RV_A5,
  RV_A6,
  RV_A7,
  RV_S2,
  RV_S3,
  RV_S4,
  RV_S5,
  RV_S6,
  RV_S7,
  RV_S8,
  RV_S9,
  RV_S10,
  RV_S11,
  RV_T3,
  RV_T4,
  RV_T5,
  RV_T6,
} xname_t;

static char *XNAME[32] = {
    "x0", "ra", "sp", "gp", "tp",  "t0",  "t1", "t2", "s0", "s1", "a0",
    "a1", "a2", "a3", "a4", "a5",  "a6",  "a7", "s2", "s3", "s4", "s5",
    "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6",
};

typedef struct {
  xlen_t x[32];
  uxlen_t pc;

  uint8_t *memory;
} rv_cpu_t;

uint32_t rv_cpu_read32(rv_cpu_t *cpu, uxlen_t addr);

struct rv_instr_s;

typedef void (*operation_f)(rv_cpu_t *, struct rv_instr_s);

typedef struct rv_instr_s {
  operation_f execute;
  uint32_t rd;
  uint32_t rs1;
  uint32_t rs2;
  xlen_t imm;
} rv_instr_t;

rv_instr_t rv_instr_decode(uint32_t code);
void rv_instr_disasm(char *buf, rv_instr_t instr);
void rv_cpu_load_elf32(rv_cpu_t *cpu, const char *fileename);
void rv_cpu_load_elf64(rv_cpu_t *cpu, const char *filename);

typedef struct {
  uint32_t opcode : 7;
  uint32_t rd : 5;
  uint32_t funct3 : 3;
  uint32_t rs1 : 5;
  uint32_t rs2 : 5;
  uint32_t funct7 : 7;
} rv_instr_format_r_t;

typedef struct {
  uint32_t opcode : 7;
  uint32_t rd : 5;
  uint32_t funct3 : 3;
  uint32_t rs1 : 5;
  uint32_t imm0 : 11;
} rv_instr_format_i_t;

typedef struct {
  uint32_t opcode : 7;
  uint32_t imm0 : 5;
  uint32_t funct3 : 3;
  uint32_t rs1 : 5;
  uint32_t rs2 : 5;
  uint32_t imm5 : 7;
} rv_instr_format_s_t;

typedef struct {
  uint32_t opcode : 7;
  uint32_t imm11 : 1;
  uint32_t imm1 : 4;
  uint32_t funct3 : 3;
  uint32_t rs1 : 5;
  uint32_t rs2 : 5;
  uint32_t imm5 : 6;
  uint32_t imm12 : 1;
} rv_instr_format_b_t;

typedef struct {
  uint32_t opcode : 7;
  uint32_t rd : 5;
  uint32_t imm12 : 20;
} rv_instr_format_u_t;

typedef struct {
  uint32_t opcode : 7;
  uint32_t rd : 5;
  uint32_t imm12 : 8;
  uint32_t imm11 : 1;
  uint32_t imm1 : 10;
  uint32_t imm20 : 1;
} rv_instr_format_j_t;

void rv_instr_lui(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_auipc(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_jal(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_jalr(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_beq(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_bne(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_blt(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_bge(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_bltu(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_bgeu(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_lb(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_lh(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_lw(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_lbu(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_lhu(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_sb(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_sh(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_sw(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_addi(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_slti(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_sltiu(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_xori(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_ori(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_andi(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_slli(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_srli(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_srai(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_add(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_sub(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_sll(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_slt(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_sltu(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_xor(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_srl(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_sra(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_or(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_and(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_fence(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_fence_tso(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_pause(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_ecall(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_ebreak(rv_cpu_t *cpu, rv_instr_t instr);

#ifdef RV_RV64I
void rv_instr_lwu(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_ld(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_sd(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_addiw(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_slliw(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_srliw(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_sraiw(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_addw(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_subw(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_sllw(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_srlw(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_sraw(rv_cpu_t *cpu, rv_instr_t instr);
#endif

#ifdef RV_RVZifencei
void rv_instr_fence_I(rv_cpu_t *cpu, rv_instr_t instr);
#endif

#ifdef RV_RVZicsr
void rv_instr_csrrw(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_csrrs(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_csrrc(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_csrrwi(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_csrrsi(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_csrrci(rv_cpu_t *cpu, rv_instr_t instr);
#endif

#ifdef RV_RV32M
void rv_instr_mul(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_mulh(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_mulhsu(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_div(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_divu(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_rem(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_remu(rv_cpu_t *cpu, rv_instr_t instr);
#endif

#ifdef RV_RV64M
#ifndef RV_RV32M
#error "RV64M expects RV32M"
#endif

void rv_instr_mulw(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_divw(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_divuw(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_remw(rv_cpu_t *cpu, rv_instr_t instr);
void rv_instr_remuw(rv_cpu_t *cpu, rv_instr_t instr);
#endif

#endif
