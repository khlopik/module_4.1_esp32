#pragma once

#include "driver/gpio.h"

namespace buttonConfig
{
    constexpr gpio_num_t pin = GPIO_NUM_7;
    constexpr uint32_t shortPressTimeMs = 50;
    constexpr int activeLevel = 0;
}

namespace uartConfig
{
    constexpr gpio_num_t rxPin = GPIO_NUM_16;
    constexpr gpio_num_t txPin = GPIO_NUM_17;
}