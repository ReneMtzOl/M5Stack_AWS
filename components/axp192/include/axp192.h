#pragma once

#include "esp_err.h"
#include "stdbool.h"

esp_err_t axp192_init(void);
esp_err_t axp192_set_dcdc1(bool enable);  // LCD backlight
esp_err_t axp192_set_ldo2(bool enable);   // LCD power
esp_err_t axp192_set_ldo3(bool enable);   // Sensor power