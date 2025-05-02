#include "i2c_bus.h"
#include "freertos/semphr.h"

static SemaphoreHandle_t i2c_mutex = NULL;
static i2c_port_t i2c_port_num;

esp_err_t i2c_bus_init(i2c_port_t port, gpio_num_t sda, gpio_num_t scl, uint32_t freq_hz) {
    i2c_port_num = port;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda,
        .scl_io_num = scl,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = freq_hz,
    };
    esp_err_t ret = i2c_param_config(i2c_port_num, &conf);
    if (ret != ESP_OK) return ret;

    ret = i2c_driver_install(i2c_port_num, conf.mode, 0, 0, 0);
    if (ret != ESP_OK) return ret;

    i2c_mutex = xSemaphoreCreateMutex();
    return (i2c_mutex != NULL) ? ESP_OK : ESP_FAIL;
}

esp_err_t i2c_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    if (xSemaphoreTake(i2c_mutex, portMAX_DELAY) == pdTRUE) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, reg_addr, true);
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_READ, true);
        i2c_master_read(cmd, data, len, I2C_MASTER_LAST_NACK);
        i2c_master_stop(cmd);
        esp_err_t ret = i2c_master_cmd_begin(i2c_port_num, cmd, pdMS_TO_TICKS(100));
        i2c_cmd_link_delete(cmd);
        xSemaphoreGive(i2c_mutex);
        return ret;
    }
    return ESP_ERR_TIMEOUT;
}

esp_err_t i2c_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    if (xSemaphoreTake(i2c_mutex, portMAX_DELAY) == pdTRUE) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, reg_addr, true);
        i2c_master_write(cmd, data, len, true);
        i2c_master_stop(cmd);
        esp_err_t ret = i2c_master_cmd_begin(i2c_port_num, cmd, pdMS_TO_TICKS(100));
        i2c_cmd_link_delete(cmd);
        xSemaphoreGive(i2c_mutex);
        return ret;
    }
    return ESP_ERR_TIMEOUT;
}
