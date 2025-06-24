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
- Targeting **bare-metal Microblaze V** systems (no OS dependencies)

## Demo

![UART CLI Demo](/docs/cmd_helper_video.gif)

---

## Getting Started with the Examples

This repository includes scripts and Makefiles to build and run example hardware, platform, and application components targeting the **Arty S7-50** board with **Vivado/Vitis 2025.1**.

### Prerequisites

- Linux environment (Windows and Mac users may need to adapt paths and tooling)
- AMD Vivado and Vitis 2025.1 installed

### Environment Setup

In new bash environments, prior to running any builds, source the Vivado and Vitis settings script:

```bash
source /path/to/Vivado/2025.1/settings64.sh
source /path/to/Vitis/2025.1/settings64.sh
```

## Build Steps

The examples are organized into three components with corresponding Makefiles:

1. **Hardware (example_hw)**
2. **Platform (example_platform)**
3. **Application (example_application)**

You can build all steps in sequence, or skip hardware and build platform and application directly using the stable `.xsa` included.

### 1. Build Hardware

```bash
cd example_hw
make hw
```
This generates the hardware design and exports the .xsa file for the Arty S7-50 in the `example_hw/build/out` directory. This will need to be transfered to the `example_platform/xsa` directory to replace the checked-in version.


### 2. Build Platform

```bash
cd ../example_platform
make platform
```
This creates the Vitis platform component from the stable .xsa in the `example_platform/xsa` directory. The stable .xsa is included in this folder to allow platform and application builds without rebuilding hardware.

### 3. Build Application

```bash
cd ../example_application
make app
```
This builds the example MicroBlaze V CLI application using the platform component. The final ELF binary is located at `example_application/build/arty_s7_riscv_app/build/arty_s7_riscv_app.elf`

## Notes

- The **hardware and platform components are specific to the Arty S7-50 board** and tested with Vivado/Vitis 2025.1.
- The **example application can be rebuilt for other MicroBlaze V systems**, but may require adaptation.
- This setup assumes a **clean Linux environment**. Windows and Mac users will need to adjust environment setup and commands accordingly.
- Before running `make`, **ensure you have sourced the Vivado and Vitis environment scripts**, e.g.:
  
  ```bash
  source /tools/Xilinx/2025.1/Vivado/settings64.sh
  source /tools/Xilinx/2025.1/Vitis/settings64.sh
  ```

  Feel free to open issues or contribute!