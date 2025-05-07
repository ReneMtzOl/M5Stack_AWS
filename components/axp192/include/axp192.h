#pragma once

#include "esp_err.h"
#include "stdbool.h"

/**
 * @brief Initialize the AXP192 power management chip.
 * 
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_init(void);

/**
 * @brief Enable or disable DCDC1 (MCU_VDD).
 * 
 * @param enable True to enable, false to disable.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_set_dcdc1(bool enable);

/**
 * @brief Enable or disable DCDC3 (Backlight).
 * 
 * @param enable True to enable, false to disable.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_set_dcdc3(bool enable);

/**
 * @brief Set the output voltage of DCDC3.
 * 
 * @param voltage Voltage in volts (0.7V to 3.5V).
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_set_dcdc3_voltage(float voltage);

/**
 * @brief Enable or disable LDO2 (LCD power).
 * 
 * @param enable True to enable, false to disable.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_set_ldo2(bool enable);

/**
 * @brief Enable or disable LDO3 (Sensor power).
 * 
 * @param enable True to enable, false to disable.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_set_ldo3(bool enable);

/**
 * @brief Enable or disable LDO1.
 * 
 * @param enable True to enable, false to disable.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_set_ldo1(bool enable);

/**
 * @brief Enable or disable RTC LDO.
 * 
 * @param enable True to enable, false to disable.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_set_rtc_ldo(bool enable);

/**
 * @brief Enable or disable DCDC2.
 * 
 * @param enable True to enable, false to disable.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_set_dcdc2(bool enable);

/**
 * @brief Set the output voltage of DCDC2.
 * 
 * @param voltage Voltage in volts (0.7V to 2.275V).
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_set_dcdc2_voltage(float voltage);

/**
 * @brief Set the output voltage of LDO1.
 * 
 * @param voltage Voltage in volts (1.8V to 3.3V).
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_set_ldo1_voltage(float voltage);

/**
 * @brief Set the output voltage of RTC LDO.
 * 
 * @param voltage Voltage in volts (1.8V to 3.3V).
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_set_rtc_ldo_voltage(float voltage);

/**
 * @brief Check if the battery is charging.
 * 
 * @param charging Pointer to a boolean where the result will be stored.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_is_charging(bool *charging);

/**
 * @brief Get the battery voltage.
 * 
 * @param voltage_mv Pointer to a float where the voltage in millivolts will be stored.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_get_battery_voltage(float *voltage_mv);

/**
 * @brief Get the battery discharge current.
 * 
 * @param current_ma Pointer to a float where the current in milliamps will be stored.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_get_battery_current(float *current_ma);

/**
 * @brief Get the battery charge current.
 * 
 * @param current_ma Pointer to a float where the current in milliamps will be stored.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_get_charge_current(float *current_ma);

/**
 * @brief Get the VBUS voltage.
 * 
 * @param voltage_mv Pointer to a float where the voltage in millivolts will be stored.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_get_vbus_voltage(float *voltage_mv);

/**
 * @brief Get the internal temperature of the AXP192 chip.
 * 
 * @param temp_c Pointer to a float where the temperature in Celsius will be stored.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_get_temperature(float *temp_c);

/**
 * @brief Get the battery level as a percentage.
 * 
 * @param percent Pointer to a uint8_t where the battery level percentage will be stored.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t axp192_get_battery_level(uint8_t *percent);