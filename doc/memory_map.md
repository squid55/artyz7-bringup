# Zynq-7000 Memory Map (Bare-metal Bring-up)

This document explains the physical memory layout used by the
bare-metal application executed after FSBL on Arty Z7-10.

## Boot Context

BootROM → FSBL → Bare-metal Application (this repo)

- DDR is initialized by FSBL
- MMU is disabled
- Physical addressing only

## Major Memory Regions

| Region | Address Range | Description |
|------|---------------|-------------|
| OCM | 0x00000000 ~ 0x0003FFFF | BootROM / FSBL |
| DDR | 0x00100000 ~ | Application code and data |

This application is fully linked to DDR.

## Linker Script Overview (lscript.ld)

The linker script controls how sections are placed in memory.

### Entry Point
- `_start` is the entry symbol
- Control is transferred here by FSBL

### Sections

- `.text` : executable code
- `.rodata` : read-only constants
- `.data` : initialized globals
- `.bss` : zero-initialized globals

### Stack / Heap

- Stack grows downward from top of DDR
- Heap grows upward after `.bss`

Assumptions:
- Single core
- No virtual memory
- No OS services

## Why This Matters

Understanding this layout is essential for:
- Early boot debugging
- DMA-safe buffer design
- U-Boot / Linux transition
- Preventing memory corruption

## Next

This memory model will be compared with:
- U-Boot memory usage
- Linux kernel virtual memory layout
