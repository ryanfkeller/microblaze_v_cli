#pragma once

#include "cli_types.h"
#include <cstdint>

namespace cli_core {

    /**
     * Abstract interface for CLI I/O operations
     * Platform-specific implementations should inherit from this interface
     */
    class CliIoInterface {
    public:
        virtual ~CliIoInterface() = default;
        
        // Output methods
        virtual void send_raw(const char* str) = 0;
        virtual void send_line(const char* str) = 0;
        virtual void send_fmt(const char* fmt, ...) = 0;
        virtual void send_byte(uint8_t byte) = 0;

        // Input methods
        virtual uint8_t get_byte() = 0;

        // Optional terminal control methods
        // (Can be overridden for enhanced/alternate functionality)
        virtual void clear_screen() {
            send_raw("\x1b[2J\r\n");
        }

        virtual void send_prompt(const char* prompt = DEFAULT_PROMPT) {
            send_raw(prompt);
        }

        virtual void handle_backspace() {
            send_raw("\b \b");
        }

        virtual void send_newline() {
            send_raw("\r\n");
        }
    };
}