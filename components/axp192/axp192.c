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
