#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>


typedef void(*function_pointer_t)(int, char*[]);

struct cmd_arg_s {
	int argc;
	char *argv[];
	};

constexpr int max_args = 10;

extern void disable_io_buffering(void);

extern void enable_io_buffering(void);

#endif
