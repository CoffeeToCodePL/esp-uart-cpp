/*
 * Copyright (c) 2023 Michał Łubiński <mlubinski@coffeetocode.pl>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef ESP_UART_CPP_SERIAL_H
#define ESP_UART_CPP_SERIAL_H

#include <esp_err.h>
#include <driver/uart.h>
#include <driver/gpio.h>
#include <span>

namespace UART {
  namespace Defaults {
    static constexpr uint32_t RXBufferSize = 512;
    static constexpr uint32_t TXBufferSize = 512;
    static constexpr uint32_t QueueSize = 10;
    static constexpr uint32_t Timeout = 1000;
  }

  struct Pins
  {
    gpio_num_t txPin = GPIO_NUM_NC;
    gpio_num_t rxPin = GPIO_NUM_NC;
    gpio_num_t rtsPin = GPIO_NUM_NC;
    gpio_num_t ctsPin = GPIO_NUM_NC;
  };

  class Serial
  {
  public:
    Serial() = default;
    ~Serial() = default;
    Serial(const Serial&) = delete;
    Serial(const Serial&&) = delete;

    [[nodiscard]] esp_err_t initialize(uart_port_t uartPort,
                                       const uart_config_t& uartConfig,
                                       const Pins& uartPins,
                                       uint32_t uartRxBufferSize = Defaults::RXBufferSize,
                                       uint32_t uartTxBufferSize = Defaults::TXBufferSize,
                                       uint32_t uartQueueSize = Defaults::QueueSize);
    void reset();

    [[nodiscard]] esp_err_t read(const std::span<uint8_t>& buffer, uint32_t timeout = Defaults::Timeout);
    [[nodiscard]] esp_err_t write(const std::span<const uint8_t>& buffer);

  private:
    uart_port_t port{ -1 };
    QueueHandle_t queue;
  };
}

#endif // ESP_UART_CPP_SERIAL_H
