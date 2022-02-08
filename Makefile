IDIR =./include
CC=gcc
EXENAME=jonamac
ODIR=.
LDIR =./lib
CPUDIR =/cpu
FLOATDIR =/softfloat
CPUOBJ=m68kcpu.o m68kops.o m68kdasm.o softfloat.o
LIBS=-lm
_DEPS = emu_cpu.h emu_global.h emu_macplus.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
_OBJ = sys_68000.o sys_global.o sys_rom.o sys_global.o cmdflags.o emu_debug.o emu.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ)) 
CFLAGS=-I$(LDIR)$(CPUDIR) -I$(LDIR)$(CPUDIR)$(FLOATDIR) -I$(IDIR) -O3 -Wall --pedantic

main: cpu $(EXENAME)

all: main

$(EXENAME): $(OBJ)
	$(CC) -o $@ $(CPUOBJ) $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cpu:
	cd $(LDIR)$(CPUDIR) && $(MAKE) && cp *.o ../.. && cp softfloat/*.o ../..

.PHONY: clean

clean:
	cd $(LDIR)$(CPUDIR) && $(MAKE) clean
	# rm $(EXENAME)
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
