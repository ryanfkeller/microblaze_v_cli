# microblaze_v_cli

A lightweight, bare-metal **command-line shell** for the MicroBlaze V RISC‑V soft-core, using AXI UARTLite for serial communication.

---

## Project Overview

`microblaze_v_cli` is a minimal UART-driven command-line interface designed for testing and diagnostics on bare-metal systems. Originally developed for the Arty S7 board, it is portable to any MicroBlaze V system with UARTLite support.

### Key Features

- Interactive UART shell with `cmd>` prompt and basic line editing (backspace, CRLF handling)
- Command registration via a simple function pointer table
- Manual argument parsing (no `strtok`, no dynamic memory)
- Lightweight and hardware-focused design suitable for embedded workflows

## Toolchain

- Built with **AMD Vivado/Vitis 2025.1**
- Targeting **bare-metal RISC-V** systems (no OS dependencies)

## Demo

![UART CLI Demo](/demo/cmd_helper_video.gif)