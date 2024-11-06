/*
 * main.c: simple test application
 */


#include "xil_printf.h"
#include "cmd_handler.h"


int main()
{
	//Clear screen and print welcome message
	xil_printf("\x1b[2J\r\n");
	xil_printf("  ___   _  __     ___          _ \r\n");
	xil_printf(" | _ \\ | |/ /    / __|_ __  __| |\r\n");
	xil_printf(" |   /_| ' < _  | (__| '  \\/ _` |\r\n");
	xil_printf(" |_|_(_)_|\\_(_)  \\___|_|_|_\\__,_|\r\n");
                                 

	xil_printf(" Version 0.1\n\n\r");

	//Launch command handler
	cmd_handler::run_cmd_handler();
    return 0;
}
