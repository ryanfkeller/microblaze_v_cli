#pragma once

#include <cstddef>

namespace cli_core {

    // Configuration constants
    constexpr int MAX_ARGS = 10;
    constexpr size_t CMD_BUFFER_SIZE = 256;
    constexpr const char* DEFAULT_PROMPT = "mbv> ";

    // Command argument structure
    struct CommandArgs {
        int argc;
        char* argv[MAX_ARGS];

        CommandArgs() : argc(0) {
            for (int i = 0; i < MAX_ARGS; i++) {
                argv[i] = nullptr;
            }
        }
    };

    // Generic command function signature
    template<typename ContextType>
    using CommandFunction = void(*)(int argc, char* const argv[], ContextType* ctx);

    // Command definition structure
    template<typename ContextType>
    struct CommandDefinition {
        const char* name;
        CommandFunction<ContextType> execute;
        const char* help;
    };

    // Command registration macro
    #define CLI_REGISTER_COMMAND(name, func, help) \
        {#name, func, help}

}