#include "driver/uart.h"
#include "button.hpp"
#include "led.hpp"
#include "uart.hpp"
#include "esp_log.h"
#include "config.hpp"

namespace
{
    const char *TAG = "MAIN";

    constexpr int uart_buffer_size = (1024 * 2);
    constexpr uart_port_t uart_num = UART_NUM_2;
    button::ButtonHandler buttonHandler;
    led::LedHandle ledHandle;

    uart::UartHandler uartHandler;

    void onSTMCommand(uart::RxCommand cmd)
    {
        ESP_LOGI(TAG, "onSTMCommand");
        switch (cmd.type)
        {
        case uart::CommandType::OFF:
            ledHandle.turnOff();
            break;
        case uart::CommandType::ON:
            ledHandle.turnOn();
            break;
        case uart::CommandType::R:
            ledHandle.setColor(led::Color::Red, cmd.value);
            break;
        case uart::CommandType::G:
            ledHandle.setColor(led::Color::Green, cmd.value);
            break;
        case uart::CommandType::B:
            ledHandle.setColor(led::Color::Blue, cmd.value);
            break;
        default:
            break;
        }
    }

    void onButtonPress(void *arg, void *data)
    {
        uartHandler.sendCommandToQueue(uart::TxCommand::ButtonPress);
    }

    void OnButtonRelease(void *arg, void *data)
    {
        uartHandler.sendCommandToQueue(uart::TxCommand::ButtonRelease);
    }
}

void setup()
{
    ledHandle.init();
    buttonHandler.init(buttonConfig::pin, buttonConfig::shortPressTimeMs, buttonConfig::activeLevel);
    buttonHandler.setOnPressCallback(&onButtonPress);
    buttonHandler.setOnReleaseCallback(&OnButtonRelease);
    uartHandler.init(uartConfig::rxPin, uartConfig::txPin);
    uartHandler.setCallback(&onSTMCommand);

    if (xTaskCreate(uartHandler.checkMessagesTask, "Check messages", 2048, &uartHandler, 2, NULL) != pdPASS)
    {
        abort();
    }

    if (xTaskCreate(uartHandler.sendMessagesTask, "Send messages", 2048, &uartHandler, 3, NULL) != pdPASS)
    {
        abort();
    }
}

void app()
{
}

extern "C" void app_main()
{
    setup();
}