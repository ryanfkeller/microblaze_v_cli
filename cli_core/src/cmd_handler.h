#pragma once


#include "utilities.h"
#include "app_context.h"

class CmdHandler {
public:
    explicit CmdHandler(AppContext& ctx);

    void run();
    cmd_arg_s get_cmd_args();
    void cmd_launcher(int argc, char **argv);

private:

    AppContext& ctx_;
};


