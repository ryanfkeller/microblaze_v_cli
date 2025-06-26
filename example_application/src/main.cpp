/**
 * main.cpp: CLI example application for Microblaze V
 */

#include <stdio.h>
#include <xgpio.h>
#include "app_context.h"
#include "app_commands.h"
#include "xparameters.h"
#include "uart_handler.h"
#include "cli_engine.h"
#include "uart_cli_adapter.h"

#ifndef VERSION_STRING
#define VERSION_STRING "dev"
#endif
#ifndef TIMESTAMP_STRING
#define TIMESTAMP_STRING "%Y-%m-%d %H:%M:%S UTC"
#endif

// Application banner
void show_banner(cli_core::CliIoInterface& io) {
    io.clear_screen();
    io.send_line("███╗   ███╗██████╗ ██╗   ██╗");
    io.send_line("████╗ ████║██╔══██╗██║   ██║");
    io.send_line("██╔████╔██║██████╔╝██║   ██║");
    io.send_line("██║╚██╔╝██║██╔══██╗╚██╗ ██╔╝");
    io.send_line("██║ ╚═╝ ██║██████╔╝ ╚████╔╝ ");
    io.send_line("╚═╝     ╚═╝╚═════╝   ╚═══╝  ");
    io.send_line("    MicroBlaze V CLI");
    io.send_line("");
    io.send_fmt("Version:     %s\n\r", VERSION_STRING);
    io.send_fmt("Build Time:  %s\n\n\r", TIMESTAMP_STRING);
}

int main() {

    // Initialize hardware
    static UartHandler uart_h(XPAR_AXI_UARTLITE_0_BASEADDR);
    static XGpio gpio;
    XGpio_Initialize(&gpio, XPAR_AXI_GPIO_0_BASEADDR);

    // Create CLI I/O adapter
    cli_core::UartCliAdapter uart(uart_h);
    
    // Create application context
    AppContext app_context{uart, gpio};
    
    // Show application banner
    show_banner(uart);
    
    // Create and configure CLI engine
    cli_core::CliEngine<AppContext> cli_engine(uart, app_context);
    cli_engine.register_commands(app_commands::command_list, app_commands::command_count);
    
    // Start CLI main loop
    cli_engine.run();
    
    return 0;
}