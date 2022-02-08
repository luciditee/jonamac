# Jonamac

An early Macintosh emulator, named for the Jonamac apple variety. Heavy work in progress; please don't expect this to be of any use until this message is removed from the readme.

## License & Acknowledgements

Released under the Mozilla Public License (see `LICENSE` for details). Incorporates the [Musashi](https://github.com/kstenerud/Musashi) 680x0 CPU emulator, which is available under the MIT license.

## Building
### Prerequisites

A recent version of gcc or clang should be enough, along with your respective system's C math library. You should also have GNU Make installed.

### Compiling
A simple `make` suffices. Remove stray object files with `make clean`. No `make install` is supplied yet as the program is not yet considered stable.

## Invoking

You'll need a Macintosh 128K ROM to use. I don't supply one so as to not run afoul of Apple's intellectual property. They're easily found out there, or if you already own a 128K, you can dump it using an EEPROM programmer of your own.

The emulator is intended to be configurable in a manner [similar to QEMU](https://www.qemu.org/docs/master/system/invocation.html).

At minimum, you must supply flags `--mem`, `--rom`, and `--addr` to indicate the amount of memory the system should have, the ROM file to use, and what memory address it should map to. For a 128K, this means you would use

```
$ ./jonamac --mem=128 --rom=./mac128.ROM --addr=0x400000
```

Different systems, as support gets added, will have different settings. Documentation will be updated to reflect this.

### Flags

Will update as features are added.

```
    --debug         Enables verbose debugging
    --mem=XXX       Specifies the amount of RAM, in kilobytes, the emulated
                    machine should have.
    --rom=XXX       Specifies the path to the ROM file that the emulated
                    machine should use.
    --addr=XXX      Specifies the physical memory address that the ROM should
                    be mapped to.
```

## Background

The Macintosh emulation scene, especially surrounding the Motorola 68000-based models pre-PowerPC, [sucks](https://invisibleup.com/articles/30/). The canonical early Macintosh emulator, [Mini vMac](https://github.com/ryandesign/minivmac), is a remarkable program, but a bit of a kludge. Its origins reside in the Universal Amiga Emulator (UAE), so as to provide a backend to operate the old [vMac](https://en.wikipedia.org/wiki/VMac) backend, which ran on the Amiga. In other words, you're emulating a cut-down Amiga, emulating a Mac emulator, which thunks Macintosh ABI calls down to something UAE can understand. On top of this, reconfiguring the emulator requires completely recompiling it, and on-the-fly settings changes require you to navigate an annoying keyboard-shortcut-based interface, marginally less annoying than using Vim for the first time.

The alternative is [Basilisk II](https://github.com/cebix/macemu), which focuses more on Macintosh II-class systems, specifically the Macintosh IIci and Macintosh Quadra 900. Basilisk II is quite capable, but has severe user experience shortcomings that I take issue with, and is itself known for longstanding bugs (for example, don't forget to uncheck the A/ROSE extension when installing System 7).

The community around it has largely just worked around these shortcomings and become numb/adapted to them. While this isn't a huge deal on its own, it's an absolute nightmare for anyone just coming into the hobby. I didn't personally grow up around Macintosh computers, but I was always _extremely_ interested in them due to how they shook the home computer landscape to its core in the mid-80s, and how they were the most Internet-prepared machines you'd find in early 90's households, predating Windows 95. However, I had to learn the hard way that things aren't always so simple with these machines, as they may be with old DOS machines.

The fact of the matter is, to work with any old Mac hardware, an absurd amount of bootstrapping is required to get from "acquire a retro Macintosh computer" to "effectively use the retro Macintosh computer," and not everyone is effectively prepared to do this.

This emulator is me trying to reconcile *why* these things are difficult to write, and if they aren't as difficult as initially appraised, perhaps it'll turn into something nice. What I'd like for it to be is an emulator that doesn't make me hate myself to use it, and in terms of blue-sky, I'd like it to be a multitool for people who are emulating old machines so as to interface with actual, physical old machines (i.e. generating hard disk images).

Hopefully it works out. It probably won't, because the ASD-brain will burn out... eventually. Let's see where things end up!
