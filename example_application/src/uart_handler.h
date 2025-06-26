#pragma once

#include "xuartlite.h"

class UartHandler {
    public:
    
        UartHandler(uint32_t uart_base_addr);

        void send_raw(const char* str);      // Send a raw string without a newline
        void send_line(const char* str);     // Send a string followed by CRLF
        void send_byte(uint8_t byte);        // Send a single byte

        uint8_t get_byte();
        

    private: 
        XUartLite uart_;
};