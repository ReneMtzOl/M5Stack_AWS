#include "axp192.h"
#include "i2c_bus.h"
#include "esp_log.h"
#include "stdbool.h"

#define AXP192_ADDR  0x34
#define AXP_PWR_CTRL 0x12
#define TAG "AXP192"

esp_err_t axp192_init(void) {
    uint8_t val;
    esp_err_t err = i2c_bus_read(AXP192_ADDR, AXP_PWR_CTRL, &val, 1);
    if (err != ESP_OK) return err;

    // Encender DCDC1 (MCU), LDO2 (LCD), y LDO3 (VIBRATOR)
    val |= (1 << 0); // DCDC1
    val |= (1 << 2); // LDO2
    val |= (1 << 3); // LDO3

    return i2c_bus_write(AXP192_ADDR, AXP_PWR_CTRL, &val, 1);
}

esp_err_t axp192_set_dcdc1(bool enable) {
    uint8_t val;
    i2c_bus_read(AXP192_ADDR, AXP_PWR_CTRL, &val, 1);
    if (enable) val |= (1 << 0);
    else        val &= ~(1 << 0);
    return i2c_bus_write(AXP192_ADDR, AXP_PWR_CTRL, &val, 1);
}

esp_err_t axp192_set_dcdc3(bool enable){
    uint8_t val;
    i2c_bus_read(AXP192_ADDR, AXP_PWR_CTRL, &val, 1);
    if (enable) val |= (1 << 1);
    else        val &= ~(1 << 1);
    return i2c_bus_write(AXP192_ADDR, AXP_PWR_CTRL, &val, 1);
}

esp_err_t axp192_set_dcdc3_voltage(float voltage) {
    if (voltage < 0.7 || voltage > 3.5) {
        return ESP_ERR_INVALID_ARG; // Rango válido: 0.7V a 3.5V
    }

    uint8_t val = (uint8_t)((voltage - 0.7) / 0.025); // Pasos de 25mV
    return i2c_bus_write(AXP192_ADDR, 0x27, &val, 1); // Registro 0x27 para DCDC3
}

esp_err_t axp192_set_ldo2(bool enable) {
    uint8_t val;
    i2c_bus_read(AXP192_ADDR, AXP_PWR_CTRL, &val, 1);
    if (enable) val |= (1 << 2);
    else        val &= ~(1 << 2);
    return i2c_bus_write(AXP192_ADDR, AXP_PWR_CTRL, &val, 1);
}

esp_err_t axp192_set_ldo3(bool enable) {
    uint8_t val;
    i2c_bus_read(AXP192_ADDR, AXP_PWR_CTRL, &val, 1);
    if (enable) val |= (1 << 3);
    else        val &= ~(1 << 3);
    return i2c_bus_write(AXP192_ADDR, AXP_PWR_CTRL, &val, 1);
}

esp_err_t axp192_get_battery_voltage(float *voltage_mv) {
    uint8_t buf[2];
    esp_err_t err = i2c_bus_read(AXP192_ADDR, 0x78, buf, 2);
    if (err != ESP_OK) return err;

    uint16_t raw = (buf[0] << 4) | (buf[1] & 0x0F);
    *voltage_mv = raw * 1.1f;  // 1.1 mV por bit
    return ESP_OK;
}

esp_err_t axp192_is_charging(bool *charging) {
    uint8_t val;
    esp_err_t err = i2c_bus_read(AXP192_ADDR, 0x01, &val, 1);
    if (err != ESP_OK) return err;

    *charging = val & (1 << 6);  // Bit 6 indica carga
    return ESP_OK;
}

esp_err_t axp192_get_battery_current(float *current_ma) {
    uint8_t buf[2];
    esp_err_t err = i2c_bus_read(AXP192_ADDR, 0x7B, buf, 2); // battery current
    if (err != ESP_OK) return err;

    uint16_t raw = (buf[0] << 4) | (buf[1] & 0x0F);
    *current_ma = raw * 0.5f; // 0.5 mA por bit
    return ESP_OK; 
}

esp_err_t axp192_get_charge_current(float *current_ma) {
    uint8_t buf[2];
    esp_err_t err = i2c_bus_read(AXP192_ADDR, 0x7A, buf, 2); // charge current
    if (err != ESP_OK) return err;

    uint16_t raw = (buf[0] << 5) | (buf[1] & 0x1F);
    *current_ma = raw * 0.5f;
    return ESP_OK;
}

esp_err_t axp192_get_vbus_voltage(float *voltage_mv) {
    uint8_t buf[2];
    esp_err_t err = i2c_bus_read(AXP192_ADDR, 0x5A, buf, 2);
    if (err != ESP_OK) return err;

    uint16_t raw = (buf[0] << 4) | (buf[1] & 0x0F);
    *voltage_mv = raw * 1.7f;  // 1.7 mV por bit
    return ESP_OK;
}

esp_err_t axp192_get_temperature(float *temp_c) {
    uint8_t buf[2];
    esp_err_t err = i2c_bus_read(AXP192_ADDR, 0x5E, buf, 2);
    if (err != ESP_OK) return err;

    uint16_t raw = (buf[0] << 4) | (buf[1] & 0x0F);
    *temp_c = (raw * 0.1f) - 144.7f;
    return ESP_OK;
}
esp_err_t axp192_get_battery_level(uint8_t *percent) {
    float voltage = 0;
    if (axp192_get_battery_voltage(&voltage) != ESP_OK) return ESP_FAIL;

    // Clamping entre 3.0V y 4.2V
    if (voltage < 3000) voltage = 3000;
    if (voltage > 4200) voltage = 4200;

    *percent = (uint8_t)(((voltage - 3000.0f) / 1200.0f) * 100.0f);
    return ESP_OK;
}
