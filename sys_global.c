
#include <stdlib.h>
#include <stdio.h>
#include <emu_rom.h>
#include <emu_global.h>

emu_memory_t* memory_context = NULL;
emu_mem_t staged_mem_size = 0;
extern emu_rom_mask_t* system_rom_mask;
extern long rom_size;

bool validate_memory() {
    emu_mem_t size = staged_mem_size * 1024;

    if (size > (MAX_MEM_KB*1024)) {
        EMU_ERRS("staged memory too high (bytes):");
        EMU_ERRI(size);
        EMU_ERRS("total address space cannot exceed the following maximum (bytes):");
        EMU_ERRI(MAX_MEM_KB*1024);
        return false;
    }

    // Memory is input from the shell in KiB so we multiply
    // it by 1024 to get the size in bytes when we boot
    staged_mem_size = size;
    EMU_DBG("memory size set to byte size:");
    EMU_DBGI(size);

    return true;
}

emu_memory_t* emu_memory_init() {
    emu_byte_t* mem = calloc(sizeof(emu_byte_t) * staged_mem_size,
        sizeof(emu_byte_t));
    emu_memory_t* ret = malloc(sizeof(emu_memory_t));
    ret->memory = mem;
    ret->address_space_size = staged_mem_size;
    return ret;
}

void sys_init() {
    EMU_DBG("entering sys_init()");
    map_rom();
}

void sys_cleanup() {
    free(system_rom_mask);
}

void set_memory_context(emu_memory_t* mem) {
    memory_context = mem;
}

emu_memory_t* get_memory_context() {
    return memory_context;
}

/* Skeleton functions for the 68k */
/* Required to link with Musashi */
// TODO: mask all of these against current system ADDRMASK

emu_mem_t read_single_byte(emu_mem_t address) {
    if (is_rom_addr(address)) {
        return (emu_mem_t)read_rom_byte(address);
    }

    return (emu_mem_t)(memory_context->memory[address]);
}

emu_mem_t m68k_read_memory_8(emu_mem_t address) {
    return read_single_byte(address);
}

emu_mem_t m68k_read_memory_16(emu_mem_t address) {
    return read_single_byte(address) & (read_single_byte(address+1) << 8);
}

emu_mem_t m68k_read_memory_32(emu_mem_t address) {
    return read_single_byte(address)
        & (read_single_byte(address+1) << 8)
        & (read_single_byte(address+2) << 16)
        & (read_single_byte(address+3) << 24);
}

void m68k_write_memory_8(emu_mem_t address, emu_mem_t value) {
    if (is_rom_addr(address))
        return;
    memory_context->memory[address] = (emu_byte_t)(0x000000FF & value);
}

void m68k_write_memory_16(emu_mem_t address, emu_mem_t value) {
    if (is_rom_addr(address))
        return;
    memory_context->memory[address] = (emu_byte_t)(0x000000FF & value);
    memory_context->memory[address+1] = (emu_byte_t)((0x0000FF00 & value) >> 8);
}

void m68k_write_memory_32(emu_mem_t address, emu_mem_t value) {
    if (is_rom_addr(address))
        return;
    memory_context->memory[address] = (emu_byte_t)(0x000000FF & value);
    memory_context->memory[address+1] = (emu_byte_t)((0x0000FF00 & value) >> 8);
    memory_context->memory[address+2] = (emu_byte_t)((0x00FF0000 & value) >> 16);
    memory_context->memory[address+3] = (emu_byte_t)((0xFF000000 & value) >> 24);
}

/* Below this line are placeholders until disassembly code is ported over */
/* You're still expected to implement these w/ Musashi, even though the readme
   doesn't really touch on this (had to dig into their example to confirm) */
emu_mem_t m68k_read_disassembler_16(emu_mem_t addr) {
    return 0;
}

emu_mem_t m68k_read_disassembler_32(emu_mem_t addr) {
    return 0;
}