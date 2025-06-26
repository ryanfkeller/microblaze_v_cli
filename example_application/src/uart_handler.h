#pragma once

#include "xuartlite.h"

class UartHandler {
    public:
    
        UartHandler(uint32_t uart_base_addr);

        void send_raw(const char* str);      // Send a raw string without a newline
        void send_line(const char* str);     // Send a string followed by CRLF
        void send_fmt(const char* fmt, ...); // Send a formatted string (i.e., printf)

        uint8_t get_byte();
        void send_byte(uint8_t byte);

    private: 
        XUartLite uart_;
};