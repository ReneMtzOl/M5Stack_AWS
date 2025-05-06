#pragma once

#include "esp_err.h"
#include "stdbool.h"

esp_err_t axp192_init(void);
esp_err_t axp192_set_dcdc1(bool enable);  // LCD backlight
esp_err_t axp192_set_ldo2(bool enable);   // LCD power
esp_err_t axp192_set_ldo3(bool enable);   // Sensor power
esp_err_t axp192_is_charging(bool *charging);
esp_err_t axp192_get_battery_voltage(float *voltage_mv);
esp_err_t axp192_get_battery_current(float *current_ma);
esp_err_t axp192_get_charge_current(float *current_ma);
esp_err_t axp192_get_vbus_voltage(float *voltage_mv);
esp_err_t axp192_get_temperature(float *temp_c);
esp_err_t axp192_get_battery_level(uint8_t *percent);