
#ifndef __EMU_GLOBAL_H
#define __EMU_GLOBAL_H

#include <stdint.h>
#include <emu_debug.h>

#define CYCLES_PER 10000
#define MAX_MEM_KB 131072

typedef unsigned int emu_mem_t;
typedef uint8_t emu_byte_t;

typedef struct emu_memory {
    emu_byte_t* memory;
    emu_mem_t address_space_size;
} emu_memory_t;

emu_memory_t* emu_memory_init();

void set_memory_context(emu_memory_t* mem);
emu_memory_t* get_memory_context();
void sys_init();

#endif
