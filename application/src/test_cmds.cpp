#include "test_cmds.h"
#include "xil_printf.h"
#include "cmd_defs.h"
#include <stdio.h>
#include "xgpio.h"

void test_cmds::cmd_io_demo(int argc, char *argv[])
{
	xil_printf("\r\nCommand IO Demo\r\n");
	xil_printf("  argv: \r\n");
	for(int i=0; i<argc ;i++)
	{
		xil_printf("    %s\r\n",argv[i]);
	}
	xil_printf("  argc: %d\r\n", argc);
	xil_printf("\r\n");
}

void test_cmds::help(int argc, char **argv)
{
	xil_printf("\r\nThe available commands are as follows:\r\n");
	for (uint8_t i = 0; i< sizeof(cmd_defs::cmd_def_array)/sizeof(cmd_defs::cmd_def_array[0]); i++)
	{
		xil_printf("  %-15s -- %s\r\n", cmd_defs::cmd_def_array[i].name, cmd_defs::cmd_def_array[i].help);
	}
	xil_printf("\r\n");
	return;
}

void test_cmds::toggle_led(int argc, char **argv)
{
	xil_printf("\r\nToggling LEDs\r\n\r\n");
	XGpio gpio;
	uint32_t led;

	XGpio_Initialize(&gpio, XPAR_AXI_GPIO_LEDS_DEVICE_ID);
	XGpio_SetDataDirection(&gpio, XPAR_AXI_GPIO_LEDS_DEVICE_ID, 0x00000000);

	led = XGpio_DiscreteRead(&gpio, XPAR_AXI_GPIO_LEDS_DEVICE_ID);
	XGpio_DiscreteWrite(&gpio, XPAR_AXI_GPIO_LEDS_DEVICE_ID, ~led);

	return;
}
