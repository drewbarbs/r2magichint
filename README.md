# radare2 `ahm` plugin: Add Hint - Magic (constant)

Adds a new command, `ahm`, to
[`r2`](https://github.com/radare/radare2). This command creates an
analysis hint replacing an immediate with a custom string in
disassembly:

    [0x00000a3b]> pd 3
    |           0x00000a3b      ba02000000     mov edx, 2
    |           0x00000a40      89c7           mov edi, eax
    |           0x00000a42      e8c9feffff     call sym.imp.lseek
    [0x00000a3b]> ahm SEEK_END
    [0x00000a3b]> pd 3
    |           0x00000a3b      ba02000000     mov edx, SEEK_END
    |           0x00000a40      89c7           mov edi, eax
    |           0x00000a42      e8c9feffff     call sym.imp.lseek

Use `ah-` to remove the annotation:

    [0x00000a3b]> ah-
    [0x00000a3b]> pd 3
    |           0x00000a3b      ba02000000     mov edx, 2
    |           0x00000a40      89c7           mov edi, eax
    |           0x00000a42      e8c9feffff     call sym.imp.lseek

## Installation
This plugin has only been built/tested on Linux.

First, make sure the radare2 `lib` directory is on your `pkg-config`
path. If it has been installed using `./sys/user.sh`, then do:

    export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$HOME/bin/prefix/radare2/lib/pkgconfig

Then, build and install the plugin:

    make && make install

## Details
This plugin is a quick hack using the machinery behind the `ta`
command, which is used to change an immediate to a structure offset:

    [0x000006f8]> pd 1
    |           0x000006f8      488b4808       mov rcx, qword [rax + 8]    ; [0x8:8]=0
    [0x000006f8]> ! cat test.h | grep -A4 some_struct
    struct some_struct {
      int x;
      int y;
      char *str;
    };
    [0x000006f8]> to test.h
    [0x000006f8]> ta some_struct.str
    [0x000006f8]> pd 1
    |           0x000006f8      488b4808       mov rcx, qword [rax + some_struct.str] ; [0x8:8]=0

`ta` will check that the given struct is defined in the type database,
and that the offset of the given member matches the offset in the
target instruction, then add an analysis hint causing `r2` to replace
the offset with `<struct>.<member>`. This plugin adds a similar
analysis hint, but using an arbitrary string (without spaces) to
replace the immediate.

**Note**: this has only been tested on x86 binaries, and is written by
an amateur `r2` user. It is sure to break soon.
