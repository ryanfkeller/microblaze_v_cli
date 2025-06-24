#pragma once

#include "app_context.h"
namespace test_cmds {
	void help(int argc, char **argv, AppContext *ctx);
	void cmd_io_demo(int argc, char **argv, AppContext *ctx);
	void toggle_led(int argc, char **argv, AppContext *ctx);
}
