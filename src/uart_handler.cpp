#include "uart_handler.h"
#include "xuartlite.h"
#include <cstdarg>
#include <cstdio>

UartHandler::UartHandler(uint32_t uart_base_addr) {
	XUartLite_Initialize(&uart_, uart_base_addr);
}

void UartHandler::send_raw(const char* str) {
    for (const char* p = str; *p != '\0'; ++p) {
        XUartLite_Send(&uart_, const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(p)), 1);
        while (XUartLite_IsSending(&uart_)) {
            // Busy Wait
        }
    }
}

void UartHandler::send_line(const char* str) {
    send_raw(str);
    send_raw("\r\n");
}

void UartHandler::send_fmt(const char* fmt, ...){
    constexpr size_t BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE];

    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buffer, BUFFER_SIZE, fmt, args);
    va_end(args);

    if (n > 0) {
        // Ensure null-terminated string in case of overflow
        buffer[BUFFER_SIZE - 1] = '\0';
        send_raw(buffer);
    }
}

uint8_t UartHandler::get_byte() {
    uint8_t byte = 0;
    int received = 0;
    // Loop until we receive 1 byte
    while (received != 1) {
        received = XUartLite_Recv(&uart_, &byte, 1);
    }
    return byte;
}

void UartHandler::send_byte(uint8_t byte) {
    XUartLite_Send(&uart_, &byte, 1);
}