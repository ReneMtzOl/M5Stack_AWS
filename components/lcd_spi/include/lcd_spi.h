#pragma once
#include "esp_err.h"
#include "stdint.h"

#pragma once #include <stdint.h>

#define LCD_WIDTH  320 #define LCD_HEIGHT 240


uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
//Init SPI Periferal and LCD
esp_err_t lcd_spi_init(void);
//Command and data send 
void lcd_send_command(uint8_t cmd);
void lcd_send_data (const uint8_t * data, size_t len);
//Fill screen with solid color
void lcd_fill_color(uint16_t color);
//Set adress 
void lcd_set_address_window(uint16_t x0,uint16_t y0,uint16_t x1, uint16_t y1);
//Set particular pixel primitive
void lcd_draw_pixel( uint16_t x, uint16_t y, uint16_t color);
//Fill rectangle primitive
void lcd_fill_rect (uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
//Draw entire bitmat to memory
void lcd_draw_bitmap (uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data);