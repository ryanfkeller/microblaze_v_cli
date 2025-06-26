#include "uart_cli_adapter.h"

// Note: Users will need to include thier own UartHandler header
#include "uart_handler.h"

namespace cli_core {
    UartCliAdapter::UartCliAdapter(UartHandler& uart_handler) : uart_(uart_handler) {}
    
    void UartCliAdapter::send_raw(const char* str){
        uart_.send_raw(str);
    }

    void UartCliAdapter::send_line(const char* str) {
        uart_.send_line(str);
    }

    void UartCliAdapter::send_fmt(const char* fmt, ...){
        char buffer[FORMAT_BUFFER_SIZE];

        va_list args;
        va_start(args, fmt);
        int n = vsnprintf(buffer, FORMAT_BUFFER_SIZE, fmt, args);
        va_end(args);

        if (n > 0) {
            // Ensure null-terminated string in case of overflow
            buffer[FORMAT_BUFFER_SIZE - 1] = '\0';
            send_raw(buffer);
        }
    }

    void UartCliAdapter::send_byte(uint8_t byte) {
        uart_.send_byte(byte);
    }

    uint8_t UartCliAdapter::get_byte() {
        return uart_.get_byte();
    }
}