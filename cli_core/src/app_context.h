#pragma once

#include "uart_handler.h"
#include "xgpio.h"

struct AppContext {
    UartHandler& uart_h;
    XGpio& gpio;
};