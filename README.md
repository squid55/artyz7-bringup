# Arty Z7-10 Bare-metal Hello (Bring-up Stage)

Bare-metal application running on Zynq-7000 (ARM Cortex-A9).
This code is executed **after FSBL** and **before any OS or SSBL**.

## Boot Position
BootROM → FSBL → **Bare-metal App (this repo)** → (optional) U-Boot → Linux

## Purpose
- Validate early bring-up after FSBL
- Verify UART input/output
- Understand linker script and memory layout
- Establish first user-controlled execution point

## Source Structure
- src/helloworld.c
  - UART input parsing
  - Numeric validation
  - Simple execution loop
- src/platform.c / platform.h
  - Platform initialization (UART, caches)
- src/lscript.ld
  - Memory layout and section placement
- Xilinx.spec
  - BSP specification for Vitis

## Environment
- Board: Arty Z7-10
- SoC: Zynq-7000
- CPU: ARM Cortex-A9
- Toolchain: Xilinx Vitis
- Execution mode: Bare-metal (no MMU, no OS)

## Notes
- No user/kernel separation at this stage
- Direct hardware access
- Debuggable via JTAG
