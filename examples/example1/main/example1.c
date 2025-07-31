#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <i2cdev.h>
#include <ags10.h>

#define TAG "ags10_basic_example"

#define I2C_PORT I2C_NUM_0
#define SDA_GPIO 6
#define SCL_GPIO 7

void scan_i2c_bus(gpio_num_t sda_pin, gpio_num_t scl_pin)
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = sda_pin;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = scl_pin;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;
    conf.clk_flags = 0;

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0));

    ESP_LOGI(__func__, "Scanning I2C bus...");

    for (uint8_t addr = 1; addr < 127; addr++)
    {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
        i2c_master_stop(cmd);

        esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(1000));
        i2c_cmd_link_delete(cmd);

        if (ret == ESP_OK)
        {
            ESP_LOGI(__func__, "Found device at address 0x%02x", addr);
        }
    }

    ESP_ERROR_CHECK(i2c_driver_delete(I2C_NUM_0));
}

void app_main(void)
{
    ESP_LOGI(TAG, "AGS10 Basic Example");

    // Initialize I2C library
    ESP_ERROR_CHECK(i2cdev_init());

    i2c_dev_t dev;
    
    scan_i2c_bus(SDA_GPIO, SCL_GPIO);
    
    // Еще одна задержка
    vTaskDelay(pdMS_TO_TICKS(500));

    // Initialize AGS10 device descriptor
    esp_err_t err = ags10_init_desc(&dev, I2C_PORT, AGS10_I2CADDR_DEFAULT, SDA_GPIO, SCL_GPIO);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize AGS10: %s", esp_err_to_name(err));
        return;
    }

        // Небольшая задержка перед инициализацией
    vTaskDelay(pdMS_TO_TICKS(1000));

    // Read firmware version
    uint8_t version;
    err = ags10_read_version(&dev, &version);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "AGS10 firmware version: v%d", version);
    } else {
        ESP_LOGE(TAG, "Failed to read AGS10 version: %s", esp_err_to_name(err));
        ags10_free_desc(&dev);
        return;
    }

    ESP_LOGI(TAG, "Starting TVOC measurements (readings every 5 seconds)...");
    
    while (1) {
        uint32_t tvoc;
        
        // Read TVOC value
        err = ags10_read_tvoc(&dev, &tvoc);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "TVOC: %lu ppb", tvoc);
        } else {
            ESP_LOGE(TAG, "Failed to read TVOC: %s", esp_err_to_name(err));
        }

        // Wait 5 seconds before next reading
        vTaskDelay(pdMS_TO_TICKS(5000));
    }

    // Clean up (this code will never be reached in this example)
    ags10_free_desc(&dev);
}
