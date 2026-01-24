#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <i2cdev.h>
#include <ags10.h>

#define TAG "ags10_advanced_example"

#define I2C_PORT I2C_NUM_0

static void read_all_values(i2c_dev_t *dev)
{
    uint32_t tvoc, resistance;
    esp_err_t err;

    // Read TVOC
    err = ags10_read_tvoc(dev, &tvoc);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "TVOC: %lu ppb", tvoc);
    }
    else
    {
        ESP_LOGE(TAG, "Failed to read TVOC: %s", esp_err_to_name(err));
    }

    // Read resistance (raw sensor value)
    err = ags10_read_resistance(dev, &resistance);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "Raw resistance: %lu", resistance);
    }
    else
    {
        ESP_LOGE(TAG, "Failed to read resistance: %s", esp_err_to_name(err));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "AGS10 Advanced Example");

    // Initialize I2C library
    ESP_ERROR_CHECK(i2cdev_init());

    i2c_dev_t dev;

    // Initialize AGS10 device descriptor
    esp_err_t err = ags10_init_desc(&dev, I2C_PORT, AGS10_I2CADDR_DEFAULT, CONFIG_EXAMPLE_I2C_MASTER_SDA, CONFIG_EXAMPLE_I2C_MASTER_SCL);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize AGS10: %s", esp_err_to_name(err));
        return;
    }

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

    ESP_LOGI(TAG, "Initial readings:");
    read_all_values(&dev);

    // Wait a bit for sensor to stabilize
    vTaskDelay(pdMS_TO_TICKS(2000));

    // Example of zero-point calibration
    ESP_LOGI(TAG, "Performing zero-point calibration with current resistance...");
    ESP_LOGW(TAG, "Make sure the sensor is in clean air environment!");

    vTaskDelay(pdMS_TO_TICKS(3000)); // Give user time to ensure clean air

    err = ags10_set_zero_point_with_current_resistance(&dev);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "Zero-point calibration successful");
    }
    else
    {
        ESP_LOGE(TAG, "Zero-point calibration failed: %s", esp_err_to_name(err));
    }

    ESP_LOGI(TAG, "Starting continuous measurements (every 10 seconds)...");
    ESP_LOGI(TAG, "Readings after calibration:");

    int reading_count = 0;
    while (1)
    {
        reading_count++;
        ESP_LOGI(TAG, "=== Reading #%d ===", reading_count);

        read_all_values(&dev);

        // Every 10th reading (100 seconds), demonstrate factory reset
        if (reading_count == 10)
        {
            ESP_LOGI(TAG, "Resetting to factory defaults...");
            err = ags10_set_zero_point_with_factory_defaults(&dev);
            if (err == ESP_OK)
            {
                ESP_LOGI(TAG, "Factory reset successful");
            }
            else
            {
                ESP_LOGE(TAG, "Factory reset failed: %s", esp_err_to_name(err));
            }
        }

        ESP_LOGI(TAG, "Waiting 10 seconds...");
        vTaskDelay(pdMS_TO_TICKS(10000));
    }

    // Clean up (this code will never be reached in this example)
    ags10_free_desc(&dev);
}
