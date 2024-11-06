#include <stdio.h>
#include "xil_printf.h"

#include "cmd_handler.h"
#include "cmd_defs.h"
#include "utilities.h"

void cmd_handler::run_cmd_handler(void) {
	while(1)
	{
		cmd_arg_s args = cmd_handler::get_cmd_args();
		if (args.argc > 0)
		{
			cmd_launcher(args.argc, args.argv);
		}
		else
		{
			xil_printf("Error receiving command!\n\r");
		}
	}
}

cmd_arg_s cmd_handler::get_cmd_args() {
	char cmd_str[256];
	char *cmd_str_p = cmd_str;
	int count = 0;
	char in_char = 0;

	disable_io_buffering();

	cmd_arg_s cmd_args = {0, nullptr};
	xil_printf("cmd> ");

	while(1)
	{
		in_char = inbyte();
		// handle backspace
		if (in_char == 0x08 )
		{
			if (count > 0)
			{
				cmd_str_p--;
				count--;
				xil_printf("\b \b");
			}
			continue;
		}

		// handle CRLF
		if( in_char == '\r' || in_char == '\n')
		{
			xil_printf("\r\n");
			// Only break if we've typed something
			if (count>0)
			{
				break;
			}
			else
			{
				xil_printf("cmd> ");
				continue;
			}
		}

		//handle everything else
		outbyte(in_char);
		*cmd_str_p = in_char;
		cmd_str_p++;
		count++;
		if (count == sizeof(cmd_str)/sizeof(cmd_str[0])-1 )
		{
			enable_io_buffering();
			return cmd_args;
		}

	}
	*cmd_str_p = '\0';

	//process C string into cmd_arg_s
	char *p2 = strtok(cmd_str, " ");
	while (p2 && cmd_args.argc < max_args-1 )
	{
		cmd_args.argv[cmd_args.argc++] = p2;
		p2 = strtok(0, " ");
	}
	cmd_args.argv[cmd_args.argc] = 0;

	enable_io_buffering();

	return cmd_args;
}



void cmd_handler::cmd_launcher(int argc, char **argv) {
	bool cmd_found = false;
	for (uint8_t i = 0; i< sizeof(cmd_defs::cmd_def_array)/sizeof(cmd_defs::cmd_def_array[0]); i++)
	{
		if (strcmp(argv[0], cmd_defs::cmd_def_array[i].name) == 0 )
		{
			cmd_defs::cmd_def_array[i].execute(argc, argv);
			cmd_found = true;
			break;
		}
	}

	if (!cmd_found)
	{
		xil_printf("Command \"%s\" not found\r\n", argv[0]);
		return;
	}
}
