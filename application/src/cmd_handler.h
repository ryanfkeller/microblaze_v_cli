#ifndef CMD_HANDLER_H
#define CMD_HANDLER_H

#include "utilities.h"

namespace cmd_handler {
	void run_cmd_handler(void);
	cmd_arg_s get_cmd_args();
	void* get_cmd_func_ptr(char* cmd_str);

	void cmd_launcher(int argc, char **argv);
}

#endif
