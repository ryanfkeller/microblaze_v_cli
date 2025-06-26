# CLI Core Library

A lightweight, embedded-friendly command-line interface library for bare-metal systems.

## Features

- **Hardware Agnostic**: Works with any I/O transport (UART, USB, TCP, etc.)
- **Zero Dynamic Allocation**: All commands registered statically at compile time
- **Template-Based**: Supports any application-specific context type
- **Extensible**: Easy to add new commands and I/O adapters
- **Embedded Optimized**: Minimal RAM usage, ROM-based command storage

## Quick Start

### 1. Define Your Application Context

```cpp
// app_context.h
struct MyAppContext {
    UartHandler& uart;
    GpioDriver& gpio;
    // Add your drivers/peripherals here
};
```

### 2. Create Commands

```cpp
// app_commands.cpp
#include "cli_types.h"

void my_command(int argc, char* argv[], MyAppContext* ctx) {
    ctx->uart.send_line("Hello from my command!");
}

const cli_core::CommandDefinition<MyAppContext> commands[] = {
    CLI_REGISTER_COMMAND(hello, my_command, "Says hello"),
};
const size_t command_count = sizeof(commands) / sizeof(commands[0]);
```

### 3. Implement I/O Adapter

```cpp
// For UART (adapter provided):
#include "uart_cli_adapter.h"
cli_core::UartCliAdapter uart_adapter(my_uart_handler);

// For custom I/O, implement CliIoInterface:
class MyCustomAdapter : public cli_core::CliIoInterface {
    // Implement required methods...
};
```

### 4. Initialize and Run

```cpp
// main.cpp
#include "cli_engine.h"

int main() {
    // Initialize hardware and context
    MyAppContext context{uart, gpio};
    cli_core::UartCliAdapter io_adapter(uart);
    
    // Create and configure CLI
    cli_core::CliEngine<MyAppContext> cli(io_adapter, context);
    cli.register_commands(commands, command_count);
    
    // Run CLI loop
    cli.run();
    return 0;
}
```

## Architecture

```
cli_core/
├── include/                    # Public API headers
│   ├── cli_engine.h           # Main CLI engine
│   ├── cli_io_interface.h     # I/O abstraction
│   └── cli_types.h            # Common types and macros
├── platform_adapters/         # Hardware-specific adapters
│   ├── include/uart_cli_adapter.h
│   └── src/uart_cli_adapter.cpp
└── README.md
```

## API Reference

### CliEngine<ContextType>

Main CLI engine class.

**Constructor:**
```cpp
CliEngine(CliIoInterface& io, ContextType& context, const char* prompt = "mbv> ")
```

**Methods:**
- `register_commands(commands, count)` - Register command array
- `run()` - Start interactive CLI loop
- `execute_command(command_line)` - Execute single command
- `print_help()` - Print available commands

### CliIoInterface

Abstract interface for I/O operations. Implement this for custom hardware.

**Required Methods:**
- `send_raw(str)` - Send string without newline
- `send_line(str)` - Send string with newline
- `send_fmt(fmt, ...)` - Send formatted string
- `send_byte(byte)` - Send single byte
- `get_byte()` - Receive single byte

**Optional Methods:**
- `clear_screen()` - Clear terminal
- `send_prompt(prompt)` - Send command prompt
- `handle_backspace()` - Handle backspace key

### Command Registration

Use the `CLI_REGISTER_COMMAND` macro for easy registration:

```cpp
const cli_core::CommandDefinition<MyContext> my_commands[] = {
    CLI_REGISTER_COMMAND(cmd_name, function_ptr, "Help text"),
    // Add more commands...
};
```

**Command Function Signature:**
```cpp
void command_function(int argc, char* argv[], MyContext* ctx);
```

## Platform Adapters

### UART Adapter

Provided adapter for UART-based I/O. Requires a UartHandler with this interface:

```cpp
class UartHandler {
public:
    void send_raw(const char* str);
    void send_line(const char* str);
    void send_fmt(const char* fmt, ...);
    void send_byte(uint8_t byte);
    uint8_t get_byte();
};
```

### Custom Adapters

Create your own adapter by inheriting from `CliIoInterface`:

```cpp
class MyAdapter : public cli_core::CliIoInterface {
public:
    void send_raw(const char* str) override { /* implementation */ }
    // Implement other required methods...
};
```

## Configuration

Modify `cli_types.h` to customize:

- `MAX_ARGS` - Maximum command arguments (default: 10)
- `CMD_BUFFER_SIZE` - Input buffer size (default: 256)
- `DEFAULT_PROMPT` - Default command prompt (default: "mbv> ")

## Integration

### Copy Files Method
Copy the required files to your project:
- All files from `include/`
- Platform adapter files you need
- Include directories in your build

### Makefile Integration
See `example_application/Makefile` for a copy-based integration example.

### Git Submodule
Add as submodule and reference headers:
```bash
git submodule add <repo-url> libs/cli_core
```

## Examples

See `../microblaze/example_application/` for a complete working example with:
- UART I/O over Microblaze
- GPIO LED control commands
- Custom application context
- Makefile integration