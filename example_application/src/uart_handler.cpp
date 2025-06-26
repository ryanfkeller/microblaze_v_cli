#include "uart_handler.h"
#include "xuartlite.h"
#include <cstdarg>

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