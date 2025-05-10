This repo contains tools, scripts, and snippets of code relating to mm.

It's licensed under the GPLv2.

## Contents

### drgn scripts

#### get_anon

`scripts/drgn/get_anon`

Rough script that retrieves kernel metadata about anon_vma state for a given PID
and and virtual address. Usage: `./get_anon <pid> <addr>`.

You must have a working `drgn` instance - sadly arch linux doesn't provide debug
symbols so us arch users have to find workarounds. Still useful for qemu
environments though.
