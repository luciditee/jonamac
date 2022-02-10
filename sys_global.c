
#include <stdlib.h>
#include <stdio.h>
#include <emu_rom.h>
#include <emu_global.h>

#define OUTSIDE_MEM_RANGE(x) x >= physical_memory->address_space_size
#define POSSIBLE_DEVICE_MEM(x) (!is_rom_addr(x) && OUTSIDE_MEM_RANGE(x))

emu_memory_t* physical_memory = NULL;
emu_size_t staged_mem_size = 0;
extern emu_rom_mask_t* system_rom_mask;
extern long rom_size;
memory_redirect_callback read_byte_redirect_callback = NULL;
device_read_callback read_device_callback = NULL;
device_write_callback  write_device_callback = NULL;

bool validate_memory() {
    emu_size_t size = staged_mem_size * 1024;

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

void sys_preboot_init() {
    EMU_DBG("entering sys_init()");
    map_rom();
}

void sys_cleanup() {
    free(system_rom_mask);
}

void set_memory_context(emu_memory_t* mem) {
    physical_memory = mem;
}

emu_memory_t* get_memory_context() {
    return physical_memory;
}

/* Skeleton functions for the 68k */
/* Required to link with Musashi */
// TODO: mask all of these against current system ADDRMASK

emu_size_t read_single_byte(emu_size_t address) {
    emu_size_t modified_addr = address;
    if (read_byte_redirect_callback != NULL)
        modified_addr = read_byte_redirect_callback(address);

    if (is_rom_addr(modified_addr))
        return (emu_size_t)read_rom_byte(modified_addr);

    /* if we read past whatever physical memory limitations we have
       and it wasn't a ROM address, we should throw a bus error. */
    // TODO: bus error handling. for now it just returns a 0
    if (OUTSIDE_MEM_RANGE(modified_addr))
        return 0;

    return (emu_size_t)(physical_memory->memory[modified_addr]);
}

// TODO: POSSIBLE_DEVICE_MEM should be replaced with a function that checks
// if the current emulated machine has a device at the specified address
emu_size_t m68k_read_memory_8(emu_size_t address) {
    if (POSSIBLE_DEVICE_MEM(address) && read_device_callback != NULL) {
        emu_size_t* ret;
        if (read_device_callback(address, 0xFF, ret) != DEVICE_MEM_SUCCESS) {
            return *ret;
        } else {
            // TODO: Bus error?
            EMU_DBG("Failure to read device");
            EMU_DBGH(address);
            return 0;
        }
    }
        

    return read_single_byte(address);
}

emu_size_t m68k_read_memory_16(emu_size_t address) {
    if (POSSIBLE_DEVICE_MEM(address) && read_device_callback != NULL) {
        emu_size_t* ret;
        if (read_device_callback(address, 0xFFFF, ret) != DEVICE_MEM_SUCCESS) {
            return *ret;
        } else {
            // TODO: Bus error?
            EMU_DBG("Failure to read device");
            EMU_DBGH(address);
            return 0;
        }
    }

    return read_single_byte(address) & (read_single_byte(address+1) << 8);
}

emu_size_t m68k_read_memory_32(emu_size_t address) {
    if (POSSIBLE_DEVICE_MEM(address) && read_device_callback != NULL) {
        emu_size_t* ret;
        if (read_device_callback(address, 0xFFFFFFFF, ret) != DEVICE_MEM_SUCCESS) {
            return *ret;
        } else {
            // TODO: Bus error?
            EMU_DBG("Failure to read device");
            EMU_DBGH(address);
            return 0;
        }
    }

    return read_single_byte(address)
        & (read_single_byte(address+1) << 8)
        & (read_single_byte(address+2) << 16)
        & (read_single_byte(address+3) << 24);
}

void m68k_write_memory_8(emu_size_t address, emu_size_t value) {
    if (POSSIBLE_DEVICE_MEM(address) && write_device_callback != NULL)
        return write_device_callback(address, value, 8);

    if (is_rom_addr(address) || OUTSIDE_MEM_RANGE(address))
        return;
    physical_memory->memory[address] = (emu_byte_t)(0x000000FF & value);
}

void m68k_write_memory_16(emu_size_t address, emu_size_t value) {
    if (POSSIBLE_DEVICE_MEM(address) && write_device_callback != NULL)
        return write_device_callback(address, value, 16);

    if (is_rom_addr(address) || OUTSIDE_MEM_RANGE(address))
        return;
    physical_memory->memory[address] = (emu_byte_t)(0x000000FF & value);
    physical_memory->memory[address+1] = (emu_byte_t)((0x0000FF00 & value) >> 8);
}

void m68k_write_memory_32(emu_size_t address, emu_size_t value) {
    if (POSSIBLE_DEVICE_MEM(address) && write_device_callback != NULL)
        return write_device_callback(address, value, 32);

    if (is_rom_addr(address) || OUTSIDE_MEM_RANGE(address))
        return;
    physical_memory->memory[address] = (emu_byte_t)(0x000000FF & value);
    physical_memory->memory[address+1] = (emu_byte_t)((0x0000FF00 & value) >> 8);
    physical_memory->memory[address+2] = (emu_byte_t)((0x00FF0000 & value) >> 16);
    physical_memory->memory[address+3] = (emu_byte_t)((0xFF000000 & value) >> 24);
}

/* Below this line are placeholders until disassembly code is ported over */
/* You're still expected to implement these w/ Musashi, even though the readme
   doesn't really touch on this (had to dig into their example to confirm) */
emu_size_t m68k_read_disassembler_16(emu_size_t addr) {
    return 0;
}

emu_size_t m68k_read_disassembler_32(emu_size_t addr) {
    return 0;
}