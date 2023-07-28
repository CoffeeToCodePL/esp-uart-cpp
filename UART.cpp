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

#include "UART.h"

esp_err_t UART::Serial::initialize(const uart_port_t uartPort,
                                   const uart_config_t& uartConfig,
                                   const Pins& uartPins,
                                   const uint32_t uartRxBufferSize,
                                   const uint32_t uartTxBufferSize,
                                   const uint32_t uartQueueSize)
{
  port = uartPort;

  if (uart_param_config(uartPort, &uartConfig) != ESP_OK) {
    return ESP_FAIL;
  }

  if (uart_set_pin(uartPort, uartPins.txPin, uartPins.rxPin, uartPins.rtsPin, uartPins.ctsPin) != ESP_OK) {
    return ESP_FAIL;
  }

  if (uart_driver_install(uartPort, uartRxBufferSize, uartTxBufferSize, uartQueueSize, &queue, 0) != ESP_OK) {
    return ESP_FAIL;
  }

  return ESP_OK;
}

void UART::Serial::reset()
{
  uart_driver_delete(port);
}

esp_err_t UART::Serial::read(const std::span<uint8_t>& buffer, uint32_t timeout)
{
  size_t pendingBytesCount{ 0 };

  if (uart_get_buffered_data_len(port, &pendingBytesCount) != ESP_OK) {
    return ESP_FAIL;
  }

  return uart_read_bytes(port, buffer.data(), std::min(pendingBytesCount, buffer.size()), pdMS_TO_TICKS(timeout));
}

esp_err_t UART::Serial::write(const std::span<const uint8_t>& buffer)
{
  return uart_write_bytes(port, buffer.data(), buffer.size());
}