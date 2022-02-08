
/* For now, this program is configured only to emulate a 128k.
   Future iterations could, or should, context-switch based on
   command line flags. The bulk of this code will be moved into
   the Mac 128 emulator code later on */

#include <emu_mac128k.h>
#include <emu_debug.h>
#include <cmdflags.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    fprintf(stdout, "starting %s version %s\n", EMU_NAME, EMU_VERSION);

    // exit if invalid args
    int arg = process_args(argc, argv);
    if (arg != 0)
        return arg;
    
    // init cpu & system ROM
    cpu(initialize)();
    sys_init();

    return 0;
}
