#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <i2cdev.h>
#include <ags10.h>

#define TAG "ags10_basic_example"

#define I2C_PORT I2C_NUM_0

void app_main(void)
{
    ESP_LOGI(TAG, "AGS10 Basic Example");

    // Initialize I2C library
    ESP_ERROR_CHECK(i2cdev_init());

    i2c_dev_t dev = { 0 };

    // Еще одна задержка
    vTaskDelay(pdMS_TO_TICKS(500));

    // Initialize AGS10 device descriptor
    esp_err_t err = ags10_init_desc(&dev, I2C_PORT, AGS10_I2CADDR_DEFAULT, CONFIG_EXAMPLE_I2C_MASTER_SDA, CONFIG_EXAMPLE_I2C_MASTER_SCL);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize AGS10: %s", esp_err_to_name(err));
        return;
    }

    // Небольшая задержка перед инициализацией
    vTaskDelay(pdMS_TO_TICKS(1000));

    // Read firmware version
    uint8_t version;
    err = ags10_read_version(&dev, &version);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "AGS10 firmware version: v%d", version);
    }
    else
    {
        ESP_LOGE(TAG, "Failed to read AGS10 version: %s", esp_err_to_name(err));
        ags10_free_desc(&dev);
        return;
    }

    ESP_LOGI(TAG, "Starting TVOC measurements (readings every 5 seconds)...");

    while (1)
    {
        uint32_t tvoc;

        // Read TVOC value
        err = ags10_read_tvoc(&dev, &tvoc);
        if (err == ESP_OK)
        {
            ESP_LOGI(TAG, "TVOC: %lu ppb", tvoc);
        }
        else
        {
            ESP_LOGE(TAG, "Failed to read TVOC: %s", esp_err_to_name(err));
        }

        // Wait 5 seconds before next reading
        vTaskDelay(pdMS_TO_TICKS(5000));
    }

    // Clean up (this code will never be reached in this example)
    ags10_free_desc(&dev);
}
