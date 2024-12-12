#ifndef UART_HANDLER_HPP
#define UART_HANDLER_HPP

#include <string>
#include <cstring>
#include <iostream>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"

#include "parser.hpp"

#define UART_PORT_NUM UART_NUM_1
#define UART_BUF_SIZE 2048
#define TAG "UART_HANDLER"
#define NUM_SERVOS 17

void uart_event_task(void *pvParameters);

#endif
