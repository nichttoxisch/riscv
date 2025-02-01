#include "../inc/rv_elf.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void read_elf_header64(int32_t fd, Elf64_Ehdr *elf_header) {
  assert(elf_header != NULL);
  assert(lseek(fd, (off_t)0, SEEK_SET) == (off_t)0);
  assert(read(fd, (void *)elf_header, sizeof(Elf64_Ehdr)) ==
         sizeof(Elf64_Ehdr));
}

void read_elf_header32(int32_t fd, Elf32_Ehdr *elf_header) {
  assert(elf_header != NULL);
  assert(lseek(fd, (off_t)0, SEEK_SET) == (off_t)0);
  assert(read(fd, (void *)elf_header, sizeof(Elf32_Ehdr)) ==
         sizeof(Elf32_Ehdr));
}

void read_section_header_table64(int32_t fd, Elf64_Ehdr eh,
                                 Elf64_Shdr sh_table[]) {
  uint32_t i;

  assert(lseek(fd, (off_t)eh.e_shoff, SEEK_SET) == (off_t)eh.e_shoff);

  for (i = 0; i < eh.e_shnum; i++) {
    assert(read(fd, (void *)&sh_table[i], eh.e_shentsize) == eh.e_shentsize);
  }
}

void read_section_header_table32(int32_t fd, Elf32_Ehdr eh,
                                 Elf32_Shdr sh_table[]) {
  uint32_t i;

  assert(lseek(fd, (off_t)eh.e_shoff, SEEK_SET) == (off_t)eh.e_shoff);

  for (i = 0; i < eh.e_shnum; i++) {
    assert(read(fd, (void *)&sh_table[i], eh.e_shentsize) == eh.e_shentsize);
  }
}

char *read_section64(int32_t fd, Elf64_Shdr sh) {
  char *buff = malloc(sh.sh_size);
  if (!buff) {
    printf("%s:Failed to allocate %ldbytes\n", __func__, sh.sh_size);
  }

  assert(buff != NULL);
  assert(lseek(fd, (off_t)sh.sh_offset, SEEK_SET) == (off_t)sh.sh_offset);
  assert(read(fd, (void *)buff, sh.sh_size) == (ssize_t)sh.sh_size);

  return buff;
}

char *read_section32(int32_t fd, Elf32_Shdr sh) {
  char *buff = malloc(sh.sh_size);
  if (!buff) {
    printf("%s:Failed to allocate %dbytes\n", __func__, sh.sh_size);
  }

  assert(buff != NULL);
  assert(lseek(fd, (off_t)sh.sh_offset, SEEK_SET) == (off_t)sh.sh_offset);
  assert(read(fd, (void *)buff, sh.sh_size) == sh.sh_size);

  return buff;
}
