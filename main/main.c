#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "i2c_bus.h"
#include "axp192.h"

void app_main(void)
{
    i2c_bus_init(I2C_NUM_0, GPIO_NUM_21, GPIO_NUM_22, 400000);
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

    axp192_set_ldo2(true);
    axp192_set_dcdc1(true); //MCU_VDD. NUNCA APAGAR
    axp192_set_ldo3(false);
}