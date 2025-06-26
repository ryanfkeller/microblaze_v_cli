#pragma once

#include "cli_types.h"
#include "app_context.h"

namespace app_commands {

    // Command function declarations
    void cmd_test_demo(int argc, char* const argv[], AppContext* ctx);
    void toggle_led(int argc, char* const argv[], AppContext* ctx);
    // void help(int argc, char* const argv[], AppContext* ctx);

    // Command registration array
    extern const cli_core::CommandDefinition<AppContext> command_list[];
    extern const size_t command_count;

}