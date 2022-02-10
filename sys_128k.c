
#include <emu_mac128k.h>
#include <emu_rom.h>

bool has_boot_staged = false;

#define DEVICE_VIA      0
#define DEVICE_IWM      1
#define DEVICE_SCCWRITE 2
#define DEVICE_SCCREAD  3
#define DEVICE_VIDEO    4

const emu_device_range_t devices[] = {
    /* DEVICE_VIA - The VIA bus master/interrupt chip */
    { 0xE80000, 0xF00000 },

    /* DEVICE_IWM - The Integrated Woz Machine (IWM) disk controller */
    { 0xD00000, 0xE00000 },

    /* DEVICE_SCCWRITE - The Zilog SCC serial controller (write addr) */
    { 0xB00000, 0xC00000 },

    /* DEVICE_SCCREAD - Ditto, but for reading*/
    { 0x900000, 0xA00000 }
};

emu_size_t device_read_128k(emu_size_t address, unsigned int wordMask, 
    emu_size_t* output)
{
    // TODO: find the device memory we want and read values from it
    return 0;
}

emu_size_t device_write_128k(emu_size_t address, emu_size_t data,
    unsigned int wordMask) 
{
    // TODO: find the device memory we want and write values to it
    return 0;
}

/* the 128k (and derivative systems) have their ROM mapped
   to 0x400000. the 68k's reset vector starts the PC at 
   address 0, and loads a pointer into memory to tell the PC
   where to start the machine from. on the mac, this is simple
   combinational logic that ORs the CPU into accessing address
   0x400000 instead of 0x000000, and then as soon as the address
   bus starts showing 0x400000, this logic disables itself and
   the ROM's initialization routines can run.

   this is also the area where memory-mapped devices have data
   shunted over to them. */
emu_size_t memory_redirect_128k(emu_size_t address) {
    /* this is a callback from the byte-by-byte memory reading
       logic in sys_global.c */
    if (has_boot_staged)
        return address;
    
    if (address >= rom_address) {
        has_boot_staged = true;
        return address;
    }

    return address | rom_address;
}

void start_128k_system() {
    // init cpu & system ROM
    cpu(initialize)();
    sys_preboot_init();

    // set memory callbacks
    read_byte_redirect_callback = memory_redirect_128k;
    read_device_callback = device_read_128k;
    write_device_callback = device_write_128k;
}


