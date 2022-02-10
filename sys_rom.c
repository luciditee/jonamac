
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <emu_rom.h>

extern char* rom_filename;
extern emu_size_t staged_mem_size;
emu_rom_mask_t* system_rom_mask = NULL;
emu_byte_t* rom = NULL;
long rom_size = 0;
FILE* fptr = NULL;

bool validate_rom_path() {
    EMU_DBG("validating ROM file input");
    if (rom_filename == NULL) {
        EMU_ERRS("no ROM filename passed");
        return false;
    }
    EMU_DBG(rom_filename);

    fptr = fopen(rom_filename, "r");
    if (fptr == NULL) {
        EMU_ERRS("could not open specified ROM file:");
        EMU_ERRS(rom_filename);
        return false;
    }
    EMU_DBG("file open ok");
    
    fseek(fptr, 0L, SEEK_END);    
    rom_size = ftell(fptr);
    if (rom_size > MAX_ROM) {
        EMU_ERRS("specified ROM file exceeds max size:");
        EMU_ERRLH(MAX_ROM);
        return false;
    }
    EMU_DBG("size ok");

    rewind(fptr);
    return true;
}

void map_rom() {
    EMU_DBG("allocating/mapping rom to its address");

    system_rom_mask = malloc(sizeof(emu_rom_mask_t));
    system_rom_mask->start_addr = rom_address;
    system_rom_mask->stop_addr = rom_address + rom_size;

    rom = malloc(rom_size+1);
    
    for (int i = 0; i < rom_size; i++)
        rom[i] = fgetc(fptr);
    
    fclose(fptr);

    EMU_DBG("rom map complete");
}

/* note: uses systemwide address, not rom-local address */
emu_byte_t read_rom_byte(emu_size_t addr) {
    emu_size_t addr_adj = addr - system_rom_mask->start_addr;
    return rom[addr_adj & system_rom_mask->stop_addr];
}

bool is_rom_addr(emu_size_t addr) {
    if (system_rom_mask == NULL)
        return false;
    if (addr < system_rom_mask->start_addr)
        return false;
    return true;
}

emu_size_t mask_rom_addr(emu_size_t addr) {
    if (!is_rom_addr(addr))
        return addr;
    return addr & system_rom_mask->stop_addr;
}

bool validate_rom_addr() {
    EMU_DBG("validating rom can be mapped to address:");
    EMU_DBGH(rom_address);

    emu_size_t size = (staged_mem_size * 1024);
    if (size < rom_size) {
        EMU_ERRS("rom size bigger than emulated address space");
        return false;
    }

    if ((rom_size + rom_address) > size) {
        EMU_ERRS("rom size + offset exceeds emulated address space");
        return false;
    }

    return true;
}
