#pragma once

#include "cli_io_interface.h"
#include <cstdarg>

// Forward declaration -- users will need to provide their own UART handler
class UartHandler;

namespace cli_core {

    /**
     * UART adapter that implements the CLI I/O interface.
     * This adapter works with any UART handler that provides the expected interface.
     */
    class UartCliAdapter : public CliIoInterface {
        public:
            /**
             * Constructor
             * @param uart_handler Reference to a UART handler implementation
             */
            explicit UartCliAdapter(UartHandler& uart_handler);

            // CliIoInterface implementation
            void send_raw(const char* str) override;
            void send_line(const char* str) override;
            void send_fmt(const char* fmt, ...) override;
            void send_byte(uint8_t byte) override;
            uint8_t get_byte() override;
        
        private:
            void send_str(const char* s);
            void print_uint(unsigned int val, int width = 0, char pad_char = ' ');
            void print_int(int val, int width = 0, char pad_char = ' ');
            void print_hex(unsigned int val, int width = 0, char pad_char = ' ');


            UartHandler& uart_;
            static constexpr size_t FORMAT_BUFFER_SIZE = 256;
    };
}