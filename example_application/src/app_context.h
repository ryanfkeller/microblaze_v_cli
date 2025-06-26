#pragma once

#include "uart_cli_adapter.h"
#include "xgpio.h"

struct AppContext {
    cli_core::UartCliAdapter& uart;
    XGpio& gpio;
};