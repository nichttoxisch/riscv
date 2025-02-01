
#include <assert.h>
#include <stdio.h>

#define STDR_IMPLEMENTATION
#include "stdr.h"

static char* OBJS[] = {"rv_elf", "rv_instr", "rv_op", "rv"};
#define OBJS_COUNT (sizeof(OBJS) / sizeof(OBJS[0]))

i32 main(void) {
  FILE* f = fopen("build.ninja", "w");
  assert(f != NULL);

  fprintf(f, "inc = -I./inc -I.\n");

  fprintf(f, "cflags = -Wall -Wextra -Wconversion -Wunused -ggdb -std=c11 $\n");
  fprintf(f, "  -fstack-protector-strong -fno-omit-frame-pointer $\n");
  fprintf(f, "  -fsanitize=address -fsanitize=undefined\n");

  fprintf(f, "libs = -static-libsan\n");

  fprintf(f, "rule cc\n");
  fprintf(f, "  depfile = $out.d\n");
  fprintf(f, "  command = clang -MD -MF $out.d $cflags $inc $in -c -o $out\n");

  fprintf(f, "rule link\n");
  fprintf(f, "  command = clang $cflags $in $libs -o $out \n");

  fprintf(f, "build rv32i: link ");
  for (usize i = 0; i < OBJS_COUNT; i++) {
    fprintf(f, "out/%s.o ", OBJS[i]);
  }
  fprintf(f, "\n");

  for (usize i = 0; i < OBJS_COUNT; i++) {
    fprintf(f, "build out/%s.o: cc src/%s.c\n", OBJS[i], OBJS[i]);
  }

  fclose(f);

  cmd("ninja");

  return 0;
}