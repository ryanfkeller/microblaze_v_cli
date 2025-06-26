# microblaze_v_cli

A lightweight, bare-metal **command-line shell** for the **MicroBlaze V RISC‑V** soft-core, built using a modular CLI library and targeting serial communication via AXI UARTLite.

---

## Overview

`microblaze_v_cli` combines a reusable CLI core library with an example application tailored for diagnostics and development on **bare-metal MicroBlaze V systems**. Originally developed for the **Arty S7-50** board, this project is portable to any platform with compatible IO driver devlopment.

---

## Repository Structure

```
microblaze_v_cli/
├── cli_core/              # Standalone CLI library (hardware-agnostic)
├── example_application/   # Demo CLI application for Arty S7-50
├── example_platform/      # Vitis platform component
├── example_hw/            # Vivado hardware design
└── docs/                  # Documentation and media
```

---

## CLI Features

The `cli_core` library is:

- **Hardware Agnostic**: Pluggable I/O interface (UART, USB, etc.)
- **Zero Dynamic Allocation**: Statically registered commands
- **Template-Based**: Supports any user-defined context
- **Extensible**: Add custom commands and transports easily
- **Optimized for Embedded**: Minimal memory footprint

In the example application:

- UART CLI shell with `mbv>` prompt
- Line editing: backspace support, CRLF handling
- Manual argument parsing (no `strtok`)
- Commands implemented for LED GPIO control and testing

See [`cli_core/README.md`](./cli_core/README.md) for full CLI engine documentation.

---

## UART CLI Demo

![UART CLI Demo](/docs/cmd_helper_video.gif)

---

## Toolchain

- **Vivado/Vitis 2025.1**
- **MicroBlaze V**, bare-metal (no OS)
- Tested on **Linux** (CLI + Makefile flow)

---

## Getting Started

### Prerequisites

- Linux system with Vivado/Vitis 2025.1 installed
- `make`, `bash`, and standard Unix tooling

Before building, source the Xilinx environment:

```bash
source /path/to/Vivado/2025.1/settings64.sh
source /path/to/Vitis/2025.1/settings64.sh
```

---

## Build Instructions

You can build hardware, platform, and application components using the provided Makefiles. A prebuilt `.xsa` is included to skip hardware steps if needed.

### 1. Build Hardware

```bash
cd example_hw
make hw
```

Generates `.xsa` in `example_hw/build/out/`. To use this output, copy it to `example_platform/xsa/`.

### 2. Build Platform

```bash
cd ../example_platform
make platform
```

Builds a Vitis platform using the `.xsa` from the previous step (or the included stable version).

### 3. Build Application

```bash
cd ../example_application
make app
```

Builds the example CLI application using `cli_core`. Output ELF is:

```
example_application/build/arty_s7_riscv_app/build/arty_s7_riscv_app.elf
```

---

## Project Notes

- **Platform-specific**: Hardware and platform builds are currently specific to the Arty S7-50.
- **Portable CLI**: The CLI core is reusable and decoupled from UART; other transports can be added.
- **Modifiable Application Context**: Easily adapt the `AppContext` to control other peripherals.
- **Cross-platform developers**: Windows/macOS users may need to adapt paths and shell tools.

---

## Integration Options

You can use the CLI core library via:

- **Copy-and-include**: Add `cli_core/include/` and optional platform adapter sources
- **Git Submodule**:
  ```bash
  git submodule add <repo-url> libs/cli_core
  ```

See the example application for both integration and usage patterns.

---

## Resources

- [`cli_core/README.md`](./cli_core/README.md): CLI architecture and usage
- [`example_application/`](./example_application/): GPIO command examples and CLI shell setup
- [`docs/`](./docs/): Demo GIFs and diagrams (if any)

---

## Contributing

Pull requests and issues are welcome! This project is intended for educational and development purposes in bare-metal RISC-V systems.