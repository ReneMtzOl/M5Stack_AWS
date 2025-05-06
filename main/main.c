#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "i2c_bus.h"
#include "lcd_spi.h"
#include "axp192.h"

float vbat, vbus, temp, charge_current, discharge_current;
uint8_t level;
bool charging;

void app_main(void)
{

    i2c_bus_init(I2C_NUM_0, GPIO_NUM_21, GPIO_NUM_22, 400000);

    lcd_spi_init();
    lcd_fill_color(color565(0, 0, 0)); // Fill screen with black color
    lcd_fill_rect(60,40,200,160,color565(255, 0, 0)); // Fill rectangle with red color

    /*
        uint8_t id = 0;
        if (i2c_bus_read(0x68, 0x75, &id, 1) == ESP_OK) {
            printf("Device ID: 0x%02X\n", id);
        } else {
            printf("I2C Read failed\n");
        }
    */
    if (axp192_init() == ESP_OK)
    {
        printf("AXP192 initialized.\n");
    }
    else
    {
        printf("Failed to initialize AXP192.\n");
    }

    axp192_set_ldo2(true); // LCD
    axp192_set_ldo3(false); // Vibrator

    axp192_get_battery_voltage(&vbat);
    axp192_get_vbus_voltage(&vbus);
    axp192_get_temperature(&temp);
    axp192_get_charge_current(&charge_current);
    axp192_get_battery_current(&discharge_current);
    axp192_get_battery_level(&level);
    axp192_is_charging(&charging);

    // axp192_set_dcdc1(true); //MCU_VDD. NUNCA APAGAR

    printf("VBAT: %.2f mV\n", vbat);
    printf("VBUS: %.2f mV\n", vbus);
    printf("Battery current: -%.2f mA\n", discharge_current);
    printf("Charge current: +%.2f mA\n", charge_current);
    printf("Temp: %.2f °C\n", temp);
    printf("Battery level: %d%%\n", level);
    printf("Charging: %s\n", charging ? "Yes" : "No");
    
    while (1)
    {
        
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        axp192_get_battery_voltage(&vbat);
        axp192_get_vbus_voltage(&vbus);
        axp192_get_temperature(&temp);
        axp192_get_charge_current(&charge_current);
        axp192_get_battery_current(&discharge_current);
        axp192_get_battery_level(&level);
        axp192_is_charging(&charging);
    
        printf("VBAT: %.2f mV\n", vbat);
        printf("VBUS: %.2f mV\n", vbus);
        printf("Battery current: -%.2f mA\n", discharge_current);
        printf("Charge current: +%.2f mA\n", charge_current);
        printf("Temp: %.2f °C\n", temp);
        printf("Battery level: %d%%\n", level);
        printf("Charging: %s\n", charging ? "Yes" : "No");        
        axp192_set_ldo3(true); //Vibrator
        vTaskDelay(1000 / portTICK_PERIOD_MS);        
        axp192_set_ldo3(false); //Vibrator

        break;
    }
}