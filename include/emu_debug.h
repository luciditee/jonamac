
#ifndef __EMU_DEBUG_H
#define __EMU_DEBUG_H
#include <stdbool.h>
#include <stdio.h>

#define EMU_VERSION "0.1a"
#define EMU_NAME "jonamac"

extern bool emu_debug_enabled;

#define EMU_PRINTLN(x) fprintf(stdout, "%s:\t%s\n", "emu", x)

#define EMU_ERRS(...) fprintf(stderr, "\x1b[31merr:\t%s\033[0m\n", __VA_ARGS__)
#define EMU_ERRH(...) fprintf(stderr, "\x1b[31merr:\t%X\033[0m\n", __VA_ARGS__)
#define EMU_ERRI(...) fprintf(stderr, "\x1b[31merr:\t%d\033[0m\n", __VA_ARGS__)
#define EMU_ERRLI(...) fprintf(stderr, "\x1b[31merr:\t%ld\033[0m\n", __VA_ARGS__)
#define EMU_ERRLH(...) fprintf(stderr, "\x1b[31merr:\t%lX\033[0m\n", __VA_ARGS__)

#define EMU_DBG(...) if (emu_debug_enabled) \
    fprintf(stdout, "dbg:\t%s\n", __VA_ARGS__)
#define EMU_DBGH(...) if (emu_debug_enabled) \
    fprintf(stdout, "dbg:\t%X\n", __VA_ARGS__)
#define EMU_DBGI(...) if (emu_debug_enabled) \
    fprintf(stdout, "dbg:\t%d\n", __VA_ARGS__)
#define EMU_DBGLH(...) if (emu_debug_enabled) \
    fprintf(stdout, "dbg:\t%lX\n", __VA_ARGS__)
#define EMU_DBGLH(...) if (emu_debug_enabled) \
    fprintf(stdout, "dbg:\t%lX\n", __VA_ARGS__)

#endif
