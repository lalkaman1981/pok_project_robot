#include <stdio.h>
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// PCA9685 Default I2C Address
#define PCA9685_ADDR 0x40
#define I2C_MASTER_SCL_IO 9    // GPIO number for I2C SCL
#define I2C_MASTER_SDA_IO 8    // GPIO number for I2C SDA
#define I2C_MASTER_NUM I2C_NUM_0 // I2C port number
#define I2C_MASTER_FREQ_HZ 400000 // I2C frequency

// PCA9685 Registers
#define PCA9685_MODE1 0x00
#define PCA9685_MODE2 0x01
#define PCA9685_PRESCALE 0xFE
#define LED0_ON_L 0x06

// Function prototypes
esp_err_t pca9685_write(uint8_t reg, uint8_t value);
esp_err_t pca9685_init(void);
void set_servo_angle(uint8_t channel, float angle);
void setUpPCA(void);

void setUpPCA(void) {
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, 0, 0, 0));
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));

    ESP_ERROR_CHECK(pca9685_init());
}

// Write to a PCA9685 register
esp_err_t pca9685_write(uint8_t reg, uint8_t value) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (PCA9685_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_write_byte(cmd, value, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

// Initialize the PCA9685
esp_err_t pca9685_init(void) {
    // Reset the PCA9685
    ESP_ERROR_CHECK(pca9685_write(PCA9685_MODE1, 0x00));
    // Set prescale for a 50Hz PWM frequency (servo standard)
    uint8_t prescale = 121; // Approx. value for 50Hz PWM
    ESP_ERROR_CHECK(pca9685_write(PCA9685_PRESCALE, prescale));
    vTaskDelay(pdMS_TO_TICKS(5));
    ESP_ERROR_CHECK(pca9685_write(PCA9685_MODE1, 0xA1)); // Auto-increment enabled
    return ESP_OK;
}

// Set the servo angle for a specific channel
void set_servo_angle(uint8_t channel, float angle) {
    // Map angle to pulse width (1ms to 2ms for 0° to 180°)
    uint16_t pulse_min = 150;  // 1ms pulse
    uint16_t pulse_max = 500;  // 2ms pulse
    uint16_t pulse = pulse_min + (angle / 180.0) * (pulse_max - pulse_min);


    uint8_t reg = LED0_ON_L + 4 * channel;
    ESP_ERROR_CHECK(pca9685_write(reg, 0));                // ON_L
    ESP_ERROR_CHECK(pca9685_write(reg + 1, 0));            // ON_H
    ESP_ERROR_CHECK(pca9685_write(reg + 2, pulse & 0xFF)); // OFF_L
    ESP_ERROR_CHECK(pca9685_write(reg + 3, pulse >> 8));   // OFF_H
}
