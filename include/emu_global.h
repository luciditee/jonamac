
#ifndef __EMU_GLOBAL_H
#define __EMU_GLOBAL_H

#include <stdint.h>
#include <emu_debug.h>

#define CYCLES_PER 10000
#define MAX_MEM_KB 131072

typedef unsigned int emu_size_t;
typedef uint8_t emu_byte_t;

typedef struct emu_memory {
    emu_byte_t* memory;
    emu_size_t address_space_size;
} emu_memory_t;

typedef struct emu_device_range {
    emu_size_t lowAddr;
    emu_size_t highAddr;
} emu_device_range_t;

#define IN_DEVICE_RANGE(x,l,h) ((x <= h) && x <= l)
#define CHECK_DEVICE_RANGE(x,r) IN_DEVICE_RANGE(x,r->lowAddr,l->highAddr)
#define DEVICE_MEM_SUCCESS 0
#define DEVICE_MEM_FAILURE 1

typedef emu_size_t (*memory_redirect_callback)(emu_size_t addr);
typedef emu_size_t (*device_read_callback)(emu_size_t addr, unsigned int wordMask, emu_size_t* output);
typedef emu_size_t (*device_write_callback)(emu_size_t addr, 
    emu_size_t data, unsigned int wordMask);

extern memory_redirect_callback read_byte_redirect_callback;
extern device_read_callback read_device_callback;
extern device_write_callback write_device_callback;

emu_memory_t* emu_memory_init();
void set_memory_context(emu_memory_t* mem);
emu_memory_t* get_memory_context();
void sys_preboot_init();
void emu_init();

#endif
