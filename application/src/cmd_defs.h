#ifndef CMD_DEFS_H
#define CMD_DEFS_H

#include "test_cmds.h"
#include "utilities.h"

namespace cmd_defs{

	struct cmd_def {
		char const *name;
		function_pointer_t execute;
		char const *help;
	};

	constexpr cmd_def cmd_def_array[] = {
			{"cmd_io_demo",
				&test_cmds::cmd_io_demo,
				"Test command to demonstrate functionality"
			},
			{"toggle_led",
				&test_cmds::toggle_led,
				"Toggles on-board LEDs",
			},
			{"help",
				&test_cmds::help,
				"Prints available commands",
			}
	};

}

#endif
