#pragma once
#include "esp_err.h"
#include "driver/i2c.h"

esp_err_t i2c_bus_init(i2c_port_t port, gpio_num_t sda, gpio_num_t scl, uint32_t freq_hz);
esp_err_t i2c_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len);
esp_err_t i2c_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len);