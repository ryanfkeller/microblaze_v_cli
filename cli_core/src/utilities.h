#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <array>



constexpr int MAX_ARGS = 10;

struct cmd_arg_s {
	int argc;
	char *argv[MAX_ARGS];
	};

extern void disable_io_buffering(void);

extern void enable_io_buffering(void);

#endif
