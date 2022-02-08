
#include <m68k.h>
#include <emu_global.h>

#ifndef __EMU_CPU_H
#define __EMU_CPU_H 1

/* 
 * For generating function names and making it easier for me to later
 * jump between different types of CPUs, have distinct functionality
 * for each, but maintain a central interface of sorts.
 * 
 * This is my janky 'interface' implementation, coming from .NET-land
 */
#define CONCAT1(a, b) a ## b
#define CONCAT(a, b) CONCAT1(a, b)
#define cpu(x) CONCAT(SYSTYPE, x)

#endif
