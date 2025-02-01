#ifndef RV_ELF_H_
#define RV_ELF_H_

#include "rv_arch.h"
#include "stdr.h"

typedef struct {
  usize addr;
  usize size;
  void* data;
  usize data_size;
} rv_elf_section_t;

typedef struct {
  rv_elf_section_t sections[5];
  usize sections_count;
  uxlen_t entry;

  void* __buffer;
} rv_elf_t;

rv_elf_t rv_elf_load(const cstr_t filename);
void rv_elf_free(rv_elf_t elf);

#endif  // RV_ELF_H_

#ifdef RV_ELF_IMPLEMENTATION

#include <assert.h>
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

rv_elf_t rv_elf_load(const cstr_t filename) {
  rv_elf_t elf = {0};

  FILE* f = fopen(filename, "r");
  assert(f != NULL);

  fseek(f, 0, SEEK_END);
  size_t size = (size_t)ftell(f);
  fseek(f, 0, SEEK_SET);

  u8* buffer = malloc(size + 1);
  fread(buffer, size, 1, f);
  fclose(f);

  rv_Elf_Ehdr* ehdr = (rv_Elf_Ehdr*)buffer;

  assert(memcmp(ehdr->e_ident, ELFMAG, SELFMAG) == 0);  // Magic
  assert(ehdr->e_type == ET_EXEC);                      // Executable
  assert(ehdr->e_machine == EM_RISCV);                  // Riscv

#ifdef RV_EXT_64
  assert(ehdr->e_ident[EI_CLASS] == ELFCLASS64);
#else
  assert(ehdr->e_ident[EI_CLASS] == ELFCLASS32);
#endif

  elf.entry = ehdr->e_entry;
  elf.__buffer = buffer;

  rv_Elf_Phdr* phdr = (rv_Elf_Phdr*)&buffer[ehdr->e_phoff];
  for (int i = 0; i < ehdr->e_phnum; i++) {
    // printf("[%d] Type: %x Offset: 0x%08" FXX " Virtual Address: 0x%08" FXX
    //        " Physical Address: 0x%08" FXX " File Size: %" FXD
    //        " Memory Size: %" FXD " Flags: 0x%x Align: %" FXD "\n",
    //        i, phdr[i].p_type, phdr[i].p_offset, phdr[i].p_vaddr,
    //        phdr[i].p_paddr, phdr[i].p_filesz, phdr[i].p_memsz,
    //        phdr[i].p_flags, phdr[i].p_align);

    if (phdr[i].p_type != PT_LOAD) continue;

    rv_elf_section_t section = {0};
    section.data_size = phdr[i].p_filesz;
    section.size = phdr[i].p_memsz;
    section.addr = phdr[i].p_vaddr;
    section.data = &buffer[phdr[i].p_offset];
    elf.sections[elf.sections_count++] = section;
    assert(elf.sections_count < sizeof(elf.sections) / sizeof(elf.sections[0]));
  }

  // rv_Elf_Shdr* shdr = (rv_Elf_Shdr*)&buffer[ehdr->e_shoff];
  // char* shstrtab = (char*)&buffer[shdr[ehdr->e_shstrndx].sh_offset];

  for (int i = 0; i < ehdr->e_shnum; i++) {
    // assert(shdr[i].sh_type != SHT_RELA && shdr[i].sh_type != SHT_REL);

    //   printf(
    //       "[%02d]  Name: %s Type: 0x%x Address: 0x%x  Offset: 0x%x Size: %u "
    //       "Flags: "
    //       "0x%x\n",
    //       i, shstrtab + shdr[i].sh_name, shdr[i].sh_type, shdr[i].sh_addr,
    //       shdr[i].sh_offset, shdr[i].sh_size, shdr[i].sh_flags);
  }

  return elf;
}

void rv_elf_section_free(rv_elf_section_t section) { free(section.data); }

void rv_elf_free(rv_elf_t elf) { free(elf.__buffer); }

#undef RV_ELF_IMPLEMENTATION
#endif  // RV_ELF_IMPLEMENTATION
