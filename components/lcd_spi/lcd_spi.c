#include "lcd_spi.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <string.h>

#define TAG "LCD_SPI"
#define LCD_HOST SPI2_HOST
// Se usa SPI por hardware

#define LCD_WIDTH  320
#define LCD_HEIGHT 240

#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK 18
#define PIN_NUM_CS 5
#define PIN_NUM_DC 15
#define PIN_NUM_RST 4

static spi_device_handle_t lcd_handle;

esp_err_t lcd_spi_init(void)
{
    esp_err_t ret = ESP_FAIL;
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PIN_NUM_DC) | (1ULL << PIN_NUM_RST),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = false,
        .pull_down_en = false,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    // Screen reset
    gpio_set_level(PIN_NUM_RST, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(PIN_NUM_RST, 1);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    // Configure SPI

    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4096,
    };

    ret = spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK)
    {
        return ret;
    }

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 40 * 1000 * 1000,
        .mode = 0,
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 7,
    };

    ret = spi_bus_add_device(LCD_HOST, &devcfg, &lcd_handle);
    if (ret != ESP_OK)
    {
        return ret;
    }

    /*
    // Minimum init sequence for ILI9342
    lcd_send_command(0xEF); // Test command
    const uint8_t data[] = {0x03, 0x80, 0x02};
    lcd_send_data(data, sizeof(data));
    */
    lcd_send_command(0x01); // Software reset
    vTaskDelay(100 / portTICK_PERIOD_MS);

    lcd_send_command(0x28); // Display OFF
    // Init sequence
    lcd_send_command(0xCF);
    uint8_t cf[] = {0x00, 0xC1, 0x30};
    lcd_send_data(cf, 3);

    lcd_send_command(0xE8);
    uint8_t e8[] = {0x85, 0x00, 0x78};
    lcd_send_data(e8, 3);

    lcd_send_command(0xCB);
    uint8_t cb[] = {0x39, 0x2C, 0x00, 0x34, 0x02};
    lcd_send_data(cb, 5);

    lcd_send_command(0xF7);
    uint8_t f7[] = {0x20};
    lcd_send_data(f7, 1);

    lcd_send_command(0xEA);
    uint8_t ea[] = {0x00, 0x00};
    lcd_send_data(ea, 2);

    lcd_send_command(0xC0); // Power Control
    uint8_t c0[] = {0x23};
    lcd_send_data(c0, 1);

    lcd_send_command(0xC1); // Power Control
    uint8_t c1[] = {0x10};
    lcd_send_data(c1, 1);

    lcd_send_command(0xC5); // VCOM Control
    uint8_t c5[] = {0x3E, 0x28};
    lcd_send_data(c5, 2);

    lcd_send_command(0xC7);
    uint8_t c7[] = {0x86};
    lcd_send_data(c7, 1);

    lcd_send_command(0x36);    // Memory Access Control
    uint8_t madctl[] = {0x48}; // MX, BGR
    lcd_send_data(madctl, 1);

    lcd_send_command(0x3A);       // Pixel Format
    uint8_t pixel_fmt[] = {0x55}; // 16 Bit
    lcd_send_data(pixel_fmt, 1);

    lcd_send_command(0xB1);
    uint8_t b1[] = {0x00, 0x18};
    lcd_send_data(b1, 2);

    lcd_send_command(0xB6); // Display Function Control
    uint8_t b6[] = {0x08, 0x82, 0x27};
    lcd_send_data(b6, 1);

    lcd_send_command(0xF2); // Enable 3G
    uint8_t f2[] = {0x99};
    lcd_send_data(f2, 1);

    lcd_send_command(0x26); // Gamma Curve
    uint8_t gamma_curve[] = {0x01};
    lcd_send_data(gamma_curve, 1);

    lcd_send_command(0x11); // Sleep Out
    vTaskDelay(120/portTICK_PERIOD_MS);
    
    lcd_send_command(0x29); //Display ON

    return ESP_OK;
}

// Send SPI Command
void lcd_send_command(uint8_t cmd)
{
    gpio_set_level(PIN_NUM_DC, 0); // DC = 0 -> Command
    spi_transaction_t t = {
        .length = 0,
        .tx_buffer = &cmd,
    };
    spi_device_transmit(lcd_handle, &t);
}
// Send SPI Data
void lcd_send_data(const uint8_t *data, size_t len)
{
    gpio_set_level(PIN_NUM_DC, 1); // DC = 1 -> Data

    spi_transaction_t t = {
        .length = len * 8,
        .tx_buffer = data,
    };
    spi_device_transmit(lcd_handle, &t);
}

// Fill screen with solid color
void lcd_fill_color(uint16_t color){
    lcd_set_address_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    uint8_t data[2] = {color >> 8, color & 0xFF};
    for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++)
    {
        lcd_send_data(data, sizeof(data));
    }
}
// Set adress
void lcd_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
    lcd_send_command(0x2A); // Column Address Set
    uint8_t data[4] = {x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF};
    lcd_send_data(data, sizeof(data));

    lcd_send_command(0x2B); // Row Address Set
    data[0] = y0 >> 8;
    data[1] = y0 & 0xFF;
    data[2] = y1 >> 8;
    data[3] = y1 & 0xFF;
    lcd_send_data(data, sizeof(data));

    lcd_send_command(0x2C); // Memory Write
}
// Set particular pixel primitive
void lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t color){
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT)
        return;

    lcd_set_address_window(x, y, x, y);
    uint8_t data[2] = {color >> 8, color & 0xFF};
    lcd_send_data(data, sizeof(data));
}
// Fill rectangle primitive
void lcd_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT)
        return;

    if (x + w > LCD_WIDTH)
        w = LCD_WIDTH - x;
    if (y + h > LCD_HEIGHT)
        h = LCD_HEIGHT - y;

    lcd_set_address_window(x, y, x + w - 1, y + h - 1);
    uint8_t data[2] = {color >> 8, color & 0xFF};
    for (int i = 0; i < w * h; i++)
    {
        lcd_send_data(data, sizeof(data));
    }
}
// Draw entire bitmat to memory
void lcd_draw_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data){
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT)
        return;

    if (x + w > LCD_WIDTH)
        w = LCD_WIDTH - x;
    if (y + h > LCD_HEIGHT)
        h = LCD_HEIGHT - y;

    lcd_set_address_window(x, y, x + w - 1, y + h - 1);
    lcd_send_data((const uint8_t *)data, w * h * sizeof(uint16_t));
}