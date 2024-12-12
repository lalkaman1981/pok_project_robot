#ifndef PCA_CONNECT_HPP
#define PCA_CONNECT_HPP

#include <iostream>
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define I2C_MASTER_SCL_IO           9    // GPIO для SCL
#define I2C_MASTER_SDA_IO           8    // GPIO для SDA
#define I2C_MASTER_FREQ_HZ          100000
#define I2C_MASTER_PORT             I2C_NUM_0

#define PCA9685_ADDR                0x40  // Адреса PCA9685 за замовчуванням
#define MODE1_REG                   0x00
#define PRESCALE_REG                0xFE
#define LED0_ON_L                   0x06

static const char* TAG = "PCA9685_Servo";

class PCA9685 {
public:
    PCA9685();
    void setServoAngle(uint8_t channel, uint16_t pulse);

private:
    void initI2C();
    void initPCA9685();
    esp_err_t writeRegister(uint8_t reg, uint8_t value);
};

#endif
