/*
 * main.c: simple test application
 */


#include <stdio.h>
#include <xgpio.h>
#include "app_context.h"
#include "xparameters.h"
#include "uart_handler.h"
#include "cmd_handler.h"


int main()
{
	// Initialize UART handler
	static UartHandler uart_h(XPAR_AXI_UARTLITE_0_BASEADDR);
	static XGpio gpio;
	XGpio_Initialize(&gpio, XPAR_AXI_GPIO_0_BASEADDR);

	// Add the UART handler to the app context
	AppContext ctx{uart_h, gpio};

	//Clear screen and print welcome message
	uart_h.send_raw("\x1b[2J\r\n");
	uart_h.send_line("▄▖    ▌  ▖▖     ▌▜     ");
	uart_h.send_line("▌ ▛▛▌▛▌  ▙▌▀▌▛▌▛▌▐ █▌▛▘");
	uart_h.send_line("▙▖▌▌▌▙▌  ▌▌█▌▌▌▙▌▐▖▙▖▌ ");
                       
	uart_h.send_raw(" Version 0.1\n\n\r");
	// Initialize devices
	

	// //Launch command handler
	CmdHandler cmd_handler(ctx);
	cmd_handler.run();
    return 0;
}
