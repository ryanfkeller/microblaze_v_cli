#include "test_cmds.h"
#include "cmd_defs.h"
#include "app_context.h"
#include <stdio.h>
#include "xgpio.h"
#include "xparameters.h"

void test_cmds::cmd_io_demo(int argc, char *argv[], AppContext *ctx)
{
	ctx->uart_h.send_line("\r\nCommand IO Demo");
	ctx->uart_h.send_line("  argv:");
	for(int i=0; i<argc ;i++)
	{
		ctx->uart_h.send_fmt("    %s\r\n",argv[i]);
	}
	ctx->uart_h.send_fmt("  argc: %d", argc);
	printf("\r\n");
}

void test_cmds::help(int argc [[maybe_unused]], char **argv [[maybe_unused]], AppContext *ctx)
{

	ctx->uart_h.send_line("\r\nThe available commands are as follows:");
	for (uint8_t i = 0; i< sizeof(cmd_defs::cmd_def_array)/sizeof(cmd_defs::cmd_def_array[0]); i++)
	{
		ctx->uart_h.send_fmt("  %-15s -- %s\r\n", cmd_defs::cmd_def_array[i].name, cmd_defs::cmd_def_array[i].help);
	}
	ctx->uart_h.send_line("");
	return;
}

void test_cmds::toggle_led(int argc [[maybe_unused]], char **argv [[maybe_unused]], AppContext *ctx)
{
	ctx->uart_h.send_line("\r\nToggling LEDs\r\n");
	uint32_t led;

	led = XGpio_DiscreteRead(&ctx->gpio, 1);
	XGpio_DiscreteWrite(&ctx->gpio, 1, ~led);

	return;
}
