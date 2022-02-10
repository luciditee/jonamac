
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cmdflags.h>
#include <emu_rom.h>

#define DATA_DELIMITER '='
#define DH_NOT_DOUBLE_HYPHEN    -1
#define DH_OKAY                 0
#define DH_MISSING_REQD_FLAG    1
#define DH_BAD_VALIDATION       2


/* helper typedefs */
typedef void (*arg_operation)(char* t);
typedef bool (*arg_validation)();
typedef struct cmdline_flag_prim {
    const char* flag;
    bool is_required;
    bool data_required;
    arg_operation operation;
    arg_validation validate;
    bool seen;
} cmdline_flag_dh_prim_t;

/* pseudo-globals for this unit */
char* rom_filename = NULL;
char* monitor_resp = NULL;
emu_size_t rom_address = 0;
extern emu_size_t staged_mem_size;

/* command line arg function pointers */
void stage_rom_path(char* path);
void stage_rom_addr(char* addr_str);
void stage_memory(char* size_str);
void enable_monitor(char* val);
extern bool validate_rom_path();
extern bool validate_rom_addr();
extern bool validate_memory();
int post_parse();
void enable_debug(char* ignoreme);

/* update prim count anytime a flag is added! */
/* this flag list also serves as an order-of-execution for
   each of the stage_* functions and the validate_* functions,
   such that you can always assume validate_memory ran before
   validate_rom_path, etc. */
#define DH_FLAG_PRIM_COUNT 4
cmdline_flag_dh_prim_t flags[] = {
    { "debug", false, false, enable_debug, NULL, false },
    { "usemonitor", false, true, enable_monitor, NULL, false },
    { "mem", true, true, stage_memory, validate_memory, false },
    { "rom", true, true, stage_rom_path, validate_rom_path, false  },
    { "addr", true, true, stage_rom_addr, validate_rom_addr, false }
};

/* given the full-length arg string, get the flag component out */
char* dh_extract_flag(char* arg, size_t len) {
    char* ret = malloc(len+1);
    size_t j = 0;
    for (size_t i = 2; i < len; i++, j++) {
        if (arg[i] == DATA_DELIMITER)
            break;
        ret[j] = arg[i];
    }

    ret[j+1] = '\0';
    return ret;
}

/* given the full-length arg string and the known length of the
   flag component, get the data from the arg string */
char* dh_extract_data(char* arg, size_t len, size_t flag_len) {
    if ((flag_len + 1) >= len)
        return NULL;
    return (arg + flag_len + 1);
}

/* parser that handles double-hyphen arguments */
int dh_switch(char* arg) {
    size_t len = strlen(arg);

    // not a double-hyphen--ignore
    if (arg[0] != arg[1] || arg[0] != '-')
        return -1;

    // extract what flag is being parsed, and then offset by
    // the number of hyphens (which is always 2) when parsing
    // the flag's data
    char* flag = dh_extract_flag(arg, len);
    char* data = dh_extract_data(arg, len, strlen(flag)+2);

    for (int i = 0; i < DH_FLAG_PRIM_COUNT; i++) {
        if (strcmp(flag, flags[i].flag) == 0) {
            if (data == NULL && flags[i].data_required)
                return -1;
            flags[i].operation(data);
            flags[i].seen = true;
        }
    }

    return 0;
}

/* iterates over doublehyphen arguments */
int double_hyphen(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        size_t len = strlen(argv[i]);
        if (len <= 2)
            continue; // ignore empty
        int result = dh_switch(argv[i]);
        if (result > 0) // return invalid/unknown flags
            return result;
    }

    return 0;
}

void stage_rom_path(char* path) {
    rom_filename = path;
    return;
}

void stage_rom_addr(char* addr_str) {
    unsigned long ul_value = strtoul(addr_str, NULL, 16);
    rom_address = (emu_size_t)ul_value;
    return;
}

void stage_memory(char* size_str) {
    staged_mem_size = (emu_size_t)atoi(size_str);
}

void enable_monitor(char* val) {
    monitor_resp = val;
}

int process_args(int argc, char *argv[]) {
    int d = double_hyphen(argc, argv);
    if (d != 0)
        return d;
    
    return post_parse();
}

int post_parse() {
    // iterate double-hyphens first
    for (int i = 0; i < DH_FLAG_PRIM_COUNT; i++) {
        if (flags[i].is_required && !flags[i].seen) {
            fprintf( stderr, 
                "err:\t\tmissing required argument: --%s\n", 
                flags[i].flag
                );
            return DH_MISSING_REQD_FLAG;
        }

        if (flags[i].seen && flags[i].validate != NULL) {
            if (!flags[i].validate()) {
                fprintf( stderr,
                    "err:\t\tfatal error in argument `--%s'\n",
                    flags[i].flag
                    );
                return DH_BAD_VALIDATION;
            }
        }
    }

    return DH_OKAY;
}

void enable_debug(char* ignoreme) {
    emu_debug_enabled = true;
    EMU_PRINTLN("enabling debug prints to stdout");
    EMU_DBG("verbose debugging enabled");
}
