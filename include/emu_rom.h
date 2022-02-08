
#ifndef __EMU_ROM_H
#define __EMU_ROM_H

#include <emu_global.h>

/* none of the classic Mac ROMs exceed 4KiB, so
   if a file appears to have more than that, we
   can immediately write it off */
#define MAX_ROM 4194304L

typedef struct emu_rom_mask {
    emu_mem_t start_addr;
    emu_mem_t stop_addr;
} emu_rom_mask_t;

typedef struct emu_rom_mapping {
    char* rom_path;
    unsigned int rom_path_len;
    emu_rom_mask_t mask;
} emu_rom_mapping_t;

emu_rom_mapping_t load_rom(char* filepath, emu_mem_t address);
void map_rom();
bool is_rom_addr(emu_mem_t addr);
emu_mem_t mask_rom_addr(emu_mem_t addr);
emu_byte_t read_rom_byte(emu_mem_t addr);

#endif
