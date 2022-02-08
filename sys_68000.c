
#define SYSTYPE  M68000
#define INTTYPE  M68K_CPU_TYPE_68000
#define ADDRMASK 0x00FFFFFF

#include <emu_cpu.h>

void cpu(initialize) (emu_memory_t* mem) {
    /* for memory_init to work, memory_staged_size must be set
       in sys_global.c. typically done as cmdline args */
    mem = emu_memory_init();
    set_memory_context(mem);
    m68k_init();
    m68k_set_cpu_type(INTTYPE);
    m68k_pulse_reset();
}

