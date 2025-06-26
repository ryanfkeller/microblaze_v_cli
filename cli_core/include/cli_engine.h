#pragma once

#include "cli_types.h"
#include "cli_io_interface.h"
#include <cstring>

namespace cli_core {

    /**
     * Generic CLI engine that handles command parsing, dispatch, and I/O. 
     * Template parameter allows for any application-specific context type.
     */
    template<typename ContextType>
    class CliEngine {
        public:
            /**
             * Constructor
             * @param io Reference to I/O interface implementation
             * @param context Reference to application-specific context
             * @param prompt Custom prompt string (optional)
             */
            CliEngine(CliIoInterface& io, ContextType& context, const char* prompt = DEFAULT_PROMPT);

            /**
             * Register command with the CLI engine
             * @param commands Array of command definitions
             * @param count Number of commands in the array
             */
            void register_commands(const CommandDefinition<ContextType>* commands, size_t count);

            /**
             * Main CLI loop -- runs indefinitely processing commands
             */
            void run();

            /**
             * Process a single command line (useful for testing or non-interactive use)
             * @param command_line String containing the command and arguments
             * @return true if command was foudn and executed, false otherwise
             */
            bool execute_command(const char* command_line);

            /**
             * Print available commands
             */
            void print_help();
        
        private:
            // Parse command line into argc/argv format
            CommandArgs parse_command_line(const char* input);

            // Get command input from user with echo and backspace support
            CommandArgs get_command_input();

            // Find and execute a command
            bool dispatch_command(const CommandArgs& args);

            // Member variables
            CliIoInterface& io_;
            ContextType& context_;
            const char* prompt_;
            const CommandDefinition<ContextType>* commands_;
            size_t command_count_;

            // Input buffer for command parsing
            char input_buffer_[CMD_BUFFER_SIZE];
    };

    // Template imlpementation (must be in header for template instantiation)
    template<typename ContextType>
    CliEngine<ContextType>::CliEngine(CliIoInterface& io, ContextType& context, const char* prompt)
        : io_(io), context_(context), prompt_(prompt), commands_(nullptr), command_count_(0) {}

    template<typename ContextType>
    void CliEngine<ContextType>::register_commands(const CommandDefinition<ContextType>* commands, size_t count) {
        commands_ = commands;
        command_count_ = count;
    }

    template<typename ContextType>
    void CliEngine<ContextType>::run() {
        while (true) {
            CommandArgs args = get_command_input();
            if (args.argc > 0) {
                if (!dispatch_command(args)) {
                    io_.send_raw("Command \"");
                    io_.send_raw(args.argv[0]);
                    io_.send_line("\" not found. Type 'help' for available commands.");
                }
            }
        }
    }

    template<typename ContextType>
    bool CliEngine<ContextType>::execute_command(const char* command_line) {
        CommandArgs args = parse_command_line(command_line);
        return dispatch_command(args);
    }

    template<typename ContextType>
    void CliEngine<ContextType>::print_help() {
        io_.send_line("\r\nAvailable commands:");
    
        // Show built-in commands
        io_.send_fmt("  %-15s -- %s\r\n", "help", "Show available commands");
        
        // Show user commands
        if (commands_ && command_count_ > 0) {
            for (size_t i = 0; i < command_count_; i++) {
                io_.send_fmt("  %-15s -- %s\r\n", commands_[i].name, commands_[i].help);
            }
        } else {
            io_.send_line("  (No additional commands registered)");
        }
        io_.send_line("");
    }

    template<typename ContextType>
    CommandArgs CliEngine<ContextType>::get_command_input() {
        memset(input_buffer_, 0, sizeof(input_buffer_));
        char* buffer_ptr = input_buffer_;
        size_t char_count = 0;
        uint8_t in_char = 0;

        io_.send_prompt(prompt_);

        while (true) {
            in_char = io_.get_byte();

            // Handle backspace
            if (in_char == 8 || in_char == 127) {
                if (char_count > 0) {
                    buffer_ptr--;
                    char_count--;
                    io_.handle_backspace();
                }
                continue;
            }

            // Handle CRLF
            if (in_char == '\r' || in_char == '\n') {
                io_.send_newline();
                // Only break if we've typed something
                if (char_count > 0) {
                    break;
                } else {
                    io_.send_prompt(prompt_);
                    continue;
                }
            }

            // Handle regular characters
            io_.send_byte(in_char);
            *buffer_ptr = in_char;
            buffer_ptr++;
            char_count++;

            // Prevent buffer overflow
            if (char_count >= CMD_BUFFER_SIZE - 1) {
                break;
            }
        }

        *buffer_ptr = '\0';
        return parse_command_line(input_buffer_);
    }

    template<typename ContextType>
    CommandArgs CliEngine<ContextType>::parse_command_line(const char* input) {
        CommandArgs args;

        //Create a mutable copy of the input for tokenization
        if (input != input_buffer_) {
            strncpy(input_buffer_, input, CMD_BUFFER_SIZE - 1);
            input_buffer_[CMD_BUFFER_SIZE - 1] = '\0';
        }

        char* token = input_buffer_;
        while (*token && args.argc < MAX_ARGS - 1) {
            // Skip leading spaces
            while (*token == ' ') token++;
            if (!*token) break;

            // Token start
            args.argv[args.argc++] = token;

            // Find the end of the token
            while (*token && (*token != ' ')) token++;

            // Null terminate the token and advanced
            if (*token) *token++ = '\0';
        }

        args.argv[args.argc] = nullptr;
        return args;
    }

    template<typename ContextType>
    bool CliEngine<ContextType>::dispatch_command(const CommandArgs& args) {
        if (args.argc == 0) {
            return false;
        }

        // Handle built-in commands first
        if (strcmp(args.argv[0], "help") == 0) {
            print_help();
            return true;
        }

        // Handle user-registered commands
        if (!commands_) {
            return false;
        }

        for (size_t i = 0; i < command_count_; i++) {
            if (strcmp(args.argv[0], commands_[i].name) == 0) {
                commands_[i].execute(args.argc, args.argv, &context_);
                return true;
            }
        }

        return false;
    }

}