#include "rv_instr.h"

#include <stdio.h>
#include <stdlib.h>

#include "rv_op.h"

typedef struct {
  u32 opcode : 7;
  u32 rd : 5;
  u32 funct3 : 3;
  u32 rs1 : 5;
  u32 rs2 : 5;
  u32 funct7 : 7;
} rv_instr_type_r_t;

typedef struct {
  u32 opcode : 7;
  u32 rd : 5;
  u32 funct3 : 3;
  u32 rs1 : 5;
  u32 imm_0 : 12;
} rv_instr_type_i_t;

typedef struct {
  u32 opcode : 7;
  u32 imm_0 : 5;
  u32 funct3 : 3;
  u32 rs1 : 5;
  u32 rs2 : 5;
  u32 imm_5 : 7;
} rv_instr_type_s_t;

typedef struct {
  u32 opcode : 7;
  u32 imm_11 : 1;
  u32 imm_1 : 4;
  u32 funct3 : 3;
  u32 rs1 : 5;
  u32 rs2 : 5;
  u32 imm_5 : 6;
  u32 imm_12 : 1;
} rv_instr_type_b_t;

typedef struct {
  u32 opcode : 7;
  u32 rd : 5;
  u32 imm_12 : 20;
} rv_instr_type_u_t;

typedef struct {
  u32 opcode : 7;
  u32 rd : 5;
  u32 imm_12 : 8;
  u32 imm_11 : 1;
  u32 imm_1 : 10;
  u32 imm_20 : 1;
} rv_instr_type_j_t;

void rv_instruction_undefined(u32 instruction) {
  printf("[ERROR] Undefined instruction: %08x\n", instruction);
  exit(1);
}

rv_instr_t rv_instr_parse(u32 instruction) {
  rv_instr_t instr;
  instr.opcode = instruction & 0b1111111;

  instr.rd = (usize)-1;
  instr.rs1 = (usize)-1;
  instr.rs2 = (usize)-1;

  switch (instr.opcode) {
    case 0b0110111: {
      rv_instr_type_u_t parsed = *(rv_instr_type_u_t*)&instruction;
      instr.imm = sext((u32)((u32)parsed.imm_12 << 12), 32);
      instr.rd = parsed.rd;
      instr.op = rv_op_lui;
    } break;
    case 0b0010111: {
      rv_instr_type_u_t parsed = *(rv_instr_type_u_t*)&instruction;
      instr.imm = sext((u32)((u32)(parsed.imm_12) << 12), 32);
      instr.rd = parsed.rd;
      instr.op = rv_op_auipc;
    } break;
    case 0b0001111: {
      rv_instr_type_i_t parsed = *(rv_instr_type_i_t*)&instruction;
      instr.imm = sext((u32)(parsed.imm_0), 12);
      instr.rd = parsed.rd;
      instr.rs1 = parsed.rs1;
      if (parsed.funct3 == 0b000) {
        instr.op = rv_op_fence;
      } else {
        rv_instruction_undefined(instruction);
      }
    } break;
#ifdef RV_EXT_A
    case 0b0101111: {
      rv_instr_type_r_t parsed = *(rv_instr_type_r_t*)&instruction;
      instr.rd = parsed.rd;
      instr.rs1 = parsed.rs1;
      instr.rs2 = parsed.rs2;
      instr.imm = parsed.funct7;
      switch (parsed.funct3) {
        case 0b010: {
          instr.op = rv_op_amoswap_w;
        } break;
        default:
          rv_instruction_undefined(instruction);
          break;
      }
    } break;
#endif
    case 0b0010011: {
      rv_instr_type_i_t parsed = *(rv_instr_type_i_t*)&instruction;
      instr.imm = sext(parsed.imm_0, 12);
      instr.rd = parsed.rd;
      instr.rs1 = parsed.rs1;

      switch (parsed.funct3) {
        case 0b000: {
          instr.op = rv_op_addi;
        } break;
        case 0b001: {
#ifdef RV_EXT_64
          xlen_t shtyp = instr.imm >> 6;
          instr.imm = instr.imm & 0b111111;
#else
          xlen_t shtyp = instr.imm >> 5;
          instr.imm = instr.imm & 0b11111;
#endif
          if (shtyp == 0b00000) {
            instr.op = rv_op_slli;
          } else {
            rv_instruction_undefined(instruction);
          }
        } break;
        case 0b101: {
#ifdef RV_EXT_64
          xlen_t shtyp = instr.imm >> 6;
          instr.imm = instr.imm & 0b111111;
#else
          xlen_t shtyp = instr.imm >> 5;
          instr.imm = instr.imm & 0b11111;
#endif
          if (shtyp == 0b0100000) {
            instr.op = rv_op_srai;
          } else if (shtyp == 0b0000000) {
            instr.op = rv_op_srli;
          } else {
            rv_instruction_undefined(instruction);
          }
        } break;
        case 0b011: {
          instr.op = rv_op_sltiu;
        } break;

        case 0b100: {
          instr.op = rv_op_xori;
        } break;
        case 0b111: {
          instr.op = rv_op_andi;
        } break;
        case 0b110: {
          instr.op = rv_op_ori;
        } break;
        default: {
          rv_instruction_undefined(instruction);
        } break;
      }
    } break;
#ifdef RV_EXT_64
    case 0b0011011: {
      rv_instr_type_i_t parsed = *(rv_instr_type_i_t*)&instruction;
      instr.imm = sext(parsed.imm_0, 12);
      instr.rd = parsed.rd;
      instr.rs1 = parsed.rs1;
      switch (parsed.funct3) {
        case 0b000: {
          instr.op = rv_op_addiw;
        } break;
        default: {
          rv_instruction_undefined(instruction);
        } break;
      }
    } break;
#endif
    case 0b0110011: {
      rv_instr_type_r_t parsed = *(rv_instr_type_r_t*)&instruction;
      instr.rd = parsed.rd;
      instr.rs1 = parsed.rs1;
      instr.rs2 = parsed.rs2;
      switch (parsed.funct3) {
        case 0b000: {
          if (parsed.funct7 == 0b0100000) {
            instr.op = rv_op_sub;
          } else if (parsed.funct7 == 0b0000000) {
            instr.op = rv_op_add;
          }
#ifdef RV_EXT_M
          else if (parsed.funct7 == 0b0000001) {
            instr.op = rv_op_mul;
          }
#endif
          else {
            rv_instruction_undefined(instruction);
          }
        } break;
        case 0b001: {
          if (parsed.funct7 == 0) {
            instr.op = rv_op_sll;
          }
#ifdef RV_EXT_M
          else if (parsed.funct7 == 0b0000001) {
            instr.op = rv_op_mulh;
          }
#endif
          else {
            rv_instruction_undefined(instruction);
          }
        } break;
        case 0b011: {
          if (parsed.funct7 == 0) {
            instr.op = rv_op_sltu;
          }
#ifdef RV_EXT_M
          else if (parsed.funct7 == 0b0000001) {
            instr.op = rv_op_mulhu;
          }
#endif
          else {
            rv_instruction_undefined(instruction);
          }
        } break;
        case 0b101: {
          if (parsed.funct7 == 0) {
            instr.op = rv_op_srl;
          } else if (parsed.funct7 == 0b100000) {
            instr.op = rv_op_sra;
          } else {
            rv_instruction_undefined(instruction);
          }
        } break;
        case 0b111: {
          if (parsed.funct7 == 0) {
            instr.op = rv_op_and;
          } else {
            rv_instruction_undefined(instruction);
          }
        } break;
        case 0b100: {
          if (parsed.funct7 == 0) {
            instr.op = rv_op_xor;
          }
#ifdef RV_EXT_M
          else if (parsed.funct7 == 0b0000001) {
            instr.op = rv_op_div;
          }
#endif
          else {
            rv_instruction_undefined(instruction);
          }
        } break;
        case 0b110: {
          if (parsed.funct7 == 0) {
            instr.op = rv_op_or;
          }
#ifdef RV_EXT_M
          else if (parsed.funct7 == 0b0000001) {
            instr.op = rv_op_rem;
          }
#endif
          else {
            rv_instruction_undefined(instruction);
          }
        } break;
        default: {
          rv_instruction_undefined(instruction);
        }
      }
    } break;
    case 0b1101111: {
      rv_instr_type_j_t parsed = *(rv_instr_type_j_t*)&instruction;
      u32 imm_r = (u32)(parsed.imm_1 << 1 | parsed.imm_11 << 11 |
                        parsed.imm_12 << 12 | parsed.imm_20 << 20);
      instr.imm = sext(imm_r, 21);
      instr.op = rv_op_jal;
      instr.rd = parsed.rd;
    } break;
    case 0b1100111: {
      rv_instr_type_i_t parsed = *(rv_instr_type_i_t*)&instruction;
      instr.imm = sext(parsed.imm_0, 12);
      instr.rd = parsed.rd;
      instr.rs1 = parsed.rs1;
      instr.op = rv_op_jalr;
    } break;
    case 0b1110011: {
      rv_instr_type_i_t parsed = *(rv_instr_type_i_t*)&instruction;
#ifdef RV_PRIV
      if (parsed.funct3 == 0b000 && parsed.imm_0 == 0b001100000010) {
        instr.rd = parsed.rd;
        instr.rs1 = parsed.rs1;
        instr.imm = sext(parsed.imm_0, 12);
        instr.op = rv_op_mret;
      } else
#endif
          if (parsed.funct3 == 0b000) {
        instr.rd = parsed.rd;
        instr.rs1 = parsed.rs1;
        instr.imm = sext(parsed.imm_0, 12);
        instr.op = rv_op_ecall;
      }
#ifdef RV_EXT_ZICSR
      else if (parsed.funct3 == 0b010) {
        instr.rd = parsed.rd;
        instr.rs1 = parsed.rs1;
        instr.imm = (xlen_t)parsed.imm_0;
        instr.op = rv_op_csrrs;
      } else if (parsed.funct3 == 0b001) {
        instr.rd = parsed.rd;
        instr.rs1 = parsed.rs1;
        instr.imm = (xlen_t)parsed.imm_0;
        instr.op = rv_op_csrrw;
      }
#endif
      else {
        rv_instruction_undefined(instruction);
      }
    } break;
    case 0b1100011: {
      rv_instr_type_b_t parsed = *(rv_instr_type_b_t*)&instruction;
      u32 imm_r = (u32)(parsed.imm_1 << 1 | parsed.imm_5 << 5 |
                        parsed.imm_11 << 11 | parsed.imm_12 << 12);
      instr.imm = sext(imm_r, 13);
      instr.rs1 = parsed.rs1;
      instr.rs2 = parsed.rs2;
      switch (parsed.funct3) {
        case 0b000: {
          instr.op = rv_op_beq;
        } break;
        case 0b001: {
          instr.op = rv_op_bne;
        } break;
        case 0b100: {
          instr.op = rv_op_blt;
        } break;
        case 0b110: {
          instr.op = rv_op_bltu;
        } break;
        case 0b111: {
          instr.op = rv_op_bgeu;
        } break;
        case 0b101: {
          instr.op = rv_op_bge;
        } break;
        default: {
          rv_instruction_undefined(instruction);
        } break;
      }
    } break;
    case 0b0000011: {
      rv_instr_type_i_t parsed = *(rv_instr_type_i_t*)&instruction;
      instr.imm = sext(parsed.imm_0, 12);
      instr.rd = parsed.rd;
      instr.rs1 = parsed.rs1;
      if (parsed.funct3 == 0b010) {
        instr.op = rv_op_lw;
      } else if (parsed.funct3 == 0b100) {
        instr.op = rv_op_lbu;
      } else if (parsed.funct3 == 0b001) {
        instr.op = rv_op_lh;
      } else if (parsed.funct3 == 0b101) {
        instr.op = rv_op_lhu;
      }
#ifdef RV_EXT_64
      else if (parsed.funct3 == 0b011) {
        instr.op = rv_op_ld;
      }
#endif
      else {
        rv_instruction_undefined(instruction);
      }
    } break;
    case 0b0100011: {
      rv_instr_type_s_t parsed = *(rv_instr_type_s_t*)&instruction;
      instr.imm = sext((u32)(parsed.imm_0 | parsed.imm_5 << 5), 12);
      instr.rs1 = parsed.rs1;
      instr.rs2 = parsed.rs2;
      if (parsed.funct3 == 0b000) {
        instr.op = rv_op_sb;
      } else if (parsed.funct3 == 0b001) {
        instr.op = rv_op_sh;
      } else if (parsed.funct3 == 0b010) {
        instr.op = rv_op_sw;
      }
#ifdef RV_EXT_64
      else if (parsed.funct3 == 0b011) {
        instr.op = rv_op_sd;
      }
#endif
      else {
        rv_instruction_undefined(instruction);
      }
    } break;
    default: {
      rv_instruction_undefined(instruction);
    }
  }

  return instr;
}