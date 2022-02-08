

/* The Mac 128k uses a plain 68000 CPU, and is "32-bit dirty",
   meaning it uses the uppermost 8b of the 32 address lines
   for memory management flags, since the regular 68000 only
   has a 24b address bus itself */
#ifndef SYSTYPE
#define SYSTYPE M68000
#define INTTYPE M68K_CPU_TYPE_68000
#define ADDRMASK 0x00FFFFFF
#endif

#ifndef __EMU_MACPLUS_H
#define __EMU_MACPLUS_H

#include <emu_global.h>
#include <emu_cpu.h>

void cpu(initialize)();

#endif
