#include "pca_connect.hpp"

PCA9685::PCA9685() {
    initI2C();
    initPCA9685();
}

void PCA9685::setServoAngle(uint8_t channel, uint16_t pulse) {

    uint8_t on_l = 0;
    uint8_t on_h = 0;
    uint8_t off_l = pulse & 0xFF;
    uint8_t off_h = (pulse >> 8) & 0xFF;

    writeRegister(LED0_ON_L + 4 * channel, on_l);
    writeRegister(LED0_ON_L + 4 * channel + 1, on_h);
    writeRegister(LED0_ON_L + 4 * channel + 2, off_l);
    writeRegister(LED0_ON_L + 4 * channel + 3, off_h);
}

void PCA9685::initI2C() {
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;

    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_PORT, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_PORT, conf.mode, 0, 0, 0));
}

void PCA9685::initPCA9685() {
    ESP_ERROR_CHECK(writeRegister(MODE1_REG, 0x00)); // Активуємо чіп
    vTaskDelay(pdMS_TO_TICKS(10));
    uint8_t prescale = 121; // Частота PWM = 50 Гц
    ESP_ERROR_CHECK(writeRegister(PRESCALE_REG, prescale));
    ESP_ERROR_CHECK(writeRegister(MODE1_REG, 0xA1)); // Режим автоінкременту
}

esp_err_t PCA9685::writeRegister(uint8_t reg, uint8_t value) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (PCA9685_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_write_byte(cmd, value, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_PORT, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}
