#include "app_commands.h"
#include "app_context.h"
#include <stdio.h>
#include "xgpio.h"
#include "xparameters.h"

namespace app_commands {

	void cmd_test_demo(int argc, char* const argv[], AppContext *ctx)
	{
		ctx->uart_h.send_line("\r\nCommand Test Demo");
		ctx->uart_h.send_line("  argv:");
		for(int i=0; i<argc ;i++)
		{
			ctx->uart_h.send_fmt("    %s\r\n",argv[i]);
		}
		ctx->uart_h.send_fmt("  argc: %d", argc);
		printf("\r\n");
	}

	// void help([[maybe_unused]] int argc, [[maybe_unused]] char* const argv[], AppContext *ctx)
	// {

	// 	ctx->uart_h.send_line("\r\nThe available commands are as follows:");
	// 	for (size_t i = 0; i < command_count; i++)
	// 	{
	// 		ctx->uart_h.send_fmt("  %-15s -- %s\r\n",
	// 			command_list[i].name,
	// 			command_list[i].help);
	// 	}
	// 	ctx->uart_h.send_line("");
	// 	return;
	// }

	void toggle_led([[maybe_unused]] int argc, [[maybe_unused]] char* const argv[], AppContext *ctx)
	{
		ctx->uart_h.send_line("\r\nToggling LEDs\r\n");
		uint32_t led = XGpio_DiscreteRead(&ctx->gpio, 1);
		XGpio_DiscreteWrite(&ctx->gpio, 1, ~led);

		return;
	}

	const cli_core::CommandDefinition<AppContext> command_list[] = {
		CLI_REGISTER_COMMAND(
			cmd_test_demo,
			cmd_test_demo,
			"Test command to demonstrate functionality"
		),
		CLI_REGISTER_COMMAND(
			toggle_led,
			toggle_led,
			"Toggles on-board LEDs"
		),
		// CLI_REGISTER_COMMAND(
		// 	help,
		// 	help,
		// 	"Prints available commands"
		// )
	};

	const size_t command_count = sizeof(command_list) / sizeof(command_list[0]);
}
