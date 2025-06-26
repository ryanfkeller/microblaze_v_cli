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


    void UartCliAdapter::send_str(const char* s) {
        while (*s) {
            uart_.send_byte(*s++);
        }
    }

    // Helper: print decimal unsigned integer with optional zero-pad and width
    void UartCliAdapter::print_uint(unsigned int val, int width, char pad_char) {
        char buf[16]; // enough for 32-bit integer
        int pos = 0;

        // Convert number to string in reverse
        do {
            buf[pos++] = '0' + (val % 10);
            val /= 10;
        } while (val > 0);

        // Pad if needed
        while (pos < width) {
            buf[pos++] = pad_char;
        }

        // Output in correct order
        for (int i = pos - 1; i >= 0; i--) {
            uart_.send_byte(buf[i]);
        }
    }

    // Helper: print signed decimal integer with width/pad
    void UartCliAdapter::print_int(int val, int width, char pad_char) {
        if (val < 0) {
            uart_.send_byte('-');
            val = -val;
            if (width > 0) width--; // Adjust width for '-'
        }
        print_uint((unsigned int)val, width, pad_char);
    }

    // Helper: print unsigned int in hex (lowercase) with width/pad
    void UartCliAdapter::print_hex(unsigned int val, int width, char pad_char) {
        const char* hex_chars = "0123456789abcdef";
        char buf[16];
        int pos = 0;

        do {
            buf[pos++] = hex_chars[val & 0xF];
            val >>= 4;
        } while (val > 0);

        while (pos < width) {
            buf[pos++] = pad_char;
        }

        for (int i = pos - 1; i >= 0; i--) {
            uart_.send_byte(buf[i]);
        }
    }

    void UartCliAdapter::send_fmt(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);

        while (*fmt) {
            if (*fmt == '%') {
                fmt++;

                // Parse flags
                char pad_char = ' ';
                if (*fmt == '0') {
                    pad_char = '0';
                    fmt++;
                }

                // Parse width
                int width = 0;
                while (*fmt >= '0' && *fmt <= '9') {
                    width = width * 10 + (*fmt - '0');
                    fmt++;
                }

                // Parse specifier
                switch (*fmt) {
                    case 'd': {
                        int val = va_arg(args, int);
                        print_int(val, width, pad_char);
                        break;
                    }
                    case 'u': {
                        unsigned int val = va_arg(args, unsigned int);
                        print_uint(val, width, pad_char);
                        break;
                    }
                    case 'x': {
                        unsigned int val = va_arg(args, unsigned int);
                        print_hex(val, width, pad_char);
                        break;
                    }
                    case 'c': {
                        char c = (char)va_arg(args, int); // char promoted to int
                        uart_.send_byte(c);
                        break;
                    }
                    case 's': {
                        const char* s = va_arg(args, const char*);
                        // For simplicity, ignore width for strings for now
                        send_str(s);
                        break;
                    }
                    case '%': {
                        uart_.send_byte('%');
                        break;
                    }
                    default: {
                        // Unknown specifier, print literally
                        uart_.send_byte('%');
                        uart_.send_byte(*fmt);
                        break;
                    }
                }
                fmt++;
            } else {
                uart_.send_byte(*fmt++);
            }
        }

        va_end(args);
    }

    
     void UartCliAdapter::send_byte(uint8_t byte) {
         uart_.send_byte(byte);
     }

     uint8_t UartCliAdapter::get_byte() {
         return uart_.get_byte();
     }
}