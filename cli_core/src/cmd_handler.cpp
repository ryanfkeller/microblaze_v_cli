#include "cmd_handler.h"
#include "app_context.h"
#include "uart_handler.h"
#include "cmd_defs.h"
#include "utilities.h"

CmdHandler::CmdHandler(AppContext& ctx)
		: ctx_(ctx) {
	ctx_.uart_h.send_raw("\x1b[2J\r\n");
	ctx_.uart_h.send_line("▄▖    ▌  ▖▖     ▌▜     ");
	ctx_.uart_h.send_line("▌ ▛▛▌▛▌  ▙▌▀▌▛▌▛▌▐ █▌▛▘");
	ctx_.uart_h.send_line("▙▖▌▌▌▙▌  ▌▌█▌▌▌▙▌▐▖▙▖▌ ");
                       
	ctx_.uart_h.send_raw(" Version 0.1\n\n\r");

	}


void CmdHandler::run() {
	while(1)
	{
		cmd_arg_s args = get_cmd_args();
		if (args.argc > 0)
		{
			cmd_launcher(args.argc, args.argv);
		}
		else
		{
			ctx_.uart_h.send_line("Error receiving command!");
		}
	}
}

cmd_arg_s CmdHandler::get_cmd_args() {
	char cmd_str[256];
	memset(cmd_str, 0, sizeof(cmd_str));
	char *cmd_str_p = cmd_str;
	int count = 0;
	uint8_t in_char = 0;

	disable_io_buffering();

	cmd_arg_s cmd_args = {0, {nullptr}};

	ctx_.uart_h.send_raw("cmd> ");

	while(1)
	{
		in_char = ctx_.uart_h.get_byte();

		// handle backspace
		if (in_char == 8 || in_char == 127 ) {
			if (count > 0) {
				cmd_str_p--;
				count--;
				ctx_.uart_h.send_raw("\b \b");
			}
			continue;
		}

		// handle CRLF
		if( in_char == '\r' || in_char == '\n') {
			ctx_.uart_h.send_raw("\r\n");
			// Only break if we've typed something
			if (count>0) {
				break;
			}
			else {
				ctx_.uart_h.send_raw("cmd> ");
				continue;
			}
		}

		//handle everything else
		ctx_.uart_h.send_byte(in_char);
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
	char* token = cmd_str;
	while (*token && cmd_args.argc < MAX_ARGS - 1) {
		// Skip leading spaces
		while (*token == ' ') ++token;
		if (!*token) break;

		// Token start
		cmd_args.argv[cmd_args.argc++] = token;

		// Find the end of the token
		while (*token && (*token != ' ')) ++token;

		// Null terminate the token and advance
		if (*token) *token++ = '\0'; //Null terminate and move to the next token
	}

	cmd_args.argv[cmd_args.argc] = nullptr;

	enable_io_buffering();

	return cmd_args;
}



void CmdHandler::cmd_launcher(int argc, char *argv[MAX_ARGS]) {
	bool cmd_found = false;
	for (uint8_t i = 0; i< sizeof(cmd_defs::cmd_def_array)/sizeof(cmd_defs::cmd_def_array[0]); i++)
	{
		if (strcmp(argv[0], cmd_defs::cmd_def_array[i].name) == 0 )
		{
			cmd_defs::cmd_def_array[i].execute(argc, argv, &ctx_);
			cmd_found = true;
			break;
		}
	}

	if (!cmd_found)
	{
		ctx_.uart_h.send_raw("Command \"");
        ctx_.uart_h.send_raw(argv[0]);
        ctx_.uart_h.send_line("\" not found");
	}
}