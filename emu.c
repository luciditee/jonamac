
/* For now, this program is configured only to emulate a 128k.
   Future iterations could, or should, context-switch based on
   command line flags. The bulk of this code will be moved into
   the Mac 128 emulator code later on */

#include <emu_sup_sys.h>
#include <emu_debug.h>
#include <emu_global.h>
#include <cmdflags.h>
#include <stdlib.h>
#include <stdio.h>
#include <emu_tui.h>

void emu_init() {
    // start the macintosh 128k
    start_128k_system();
}

int main(int argc, char** argv) {

    // exit if invalid args
    int arg = process_args(argc, argv);
    if (arg != 0)
        return arg;
        
    init_tui();

    // start the emulator right away if the 
    if (!use_tui_debugger)
        emu_init();

    return 0;
}
