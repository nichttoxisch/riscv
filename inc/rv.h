#ifndef RV_H_
#define RV_H_

#include "stdr.h"

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

#endif  // RV_H_