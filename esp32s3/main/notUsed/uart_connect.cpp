#include "uart_connect.hpp"

void uart_event_task(void *pvParameters) {
    uint8_t *data = new uint8_t[UART_BUF_SIZE];
    if (data == nullptr) {
        ESP_LOGE(TAG, "Failed to allocate memory for UART buffer");
        vTaskDelete(nullptr);
    }

    std::string line_buffer;

    while (true) {
        int length = uart_read_bytes(UART_PORT_NUM, data, UART_BUF_SIZE - 1, pdMS_TO_TICKS(100));
        if (length > 0) {

            data[length] = '\0'; // Завершуємо рядок

            for (int i = 0; i < length; ++i) {
                if (data[i] == '\n') {
                    process_line(line_buffer); // Обробляємо рядок
                    line_buffer.clear(); // Очищуємо буфер
                } else {
                    line_buffer += static_cast<char>(data[i]);
                }
            }
        }
    }

    delete[] data;
    vTaskDelete(nullptr);
}
