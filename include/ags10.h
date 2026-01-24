/*
 * Copyright (c) 2024 xyzroe <i@xyzroe.cc>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file ags10.h
 * @defgroup ags10 ags10
 * @{
 *
 * ESP-IDF driver for AGS10 TVOC sensor
 *
 * Copyright (c) 2024 xyzroe <i@xyzroe.cc>
 *
 * BSD Licensed as described in the file LICENSE
 */
#ifndef __AGS10_H__
#define __AGS10_H__

#include <stdint.h>
#include <stdbool.h>
#include <esp_err.h>
#include <i2cdev.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AGS10_I2CADDR_DEFAULT 0x1A  /*!< AGS10 default I2C address */
#define AGS10_TVOCSTAT_REG    0x00  /*!< Status and TVOC reading */
#define AGS10_VERSION_REG     0x11  /*!< Firmware version */
#define AGS10_GASRES_REG      0x20  /*!< Raw gas resistance */
#define AGS10_SETADDR_REG     0x21  /*!< Change I2C address */
#define AGS10_CRC8_INIT       0xFF  /*!< CRC8 init value */
#define AGS10_CRC8_POLYNOMIAL 0x31  /*!< CRC8 polynomial */
#define I2C_FREQ_HZ           20000 /*!< Fixed I2C frequency for AGS10 */

/**
 * @brief Initialize the AGS10 sensor descriptor
 *
 * This function initializes the I2C device descriptor for the AGS10 sensor.
 * It configures the I2C port, address, and GPIO pins for SDA and SCL.
 *
 * @code{c}
 * #include <ags10.h>
 * #include <i2cdev.h>
 * #include <esp_log.h>
 *
 * #define TAG "ags10_example"
 * #define I2C_PORT I2C_NUM_0
 * #define SDA_GPIO GPIO_NUM_21
 * #define SCL_GPIO GPIO_NUM_22
 *
 * void app_main(void)
 * {
 *     i2c_dev_t dev;
 *     ESP_ERROR_CHECK(i2cdev_init());
 *
 *     esp_err_t err = ags10_init_desc(&dev, I2C_PORT, AGS10_I2CADDR_DEFAULT, SDA_GPIO, SCL_GPIO);
 *     if (err != ESP_OK) {
 *         ESP_LOGE(TAG, "Failed to init AGS10: %s", esp_err_to_name(err));
 *         return;
 *     }
 *
 *     uint32_t tvoc;
 *     err = ags10_read_tvoc(&dev, &tvoc);
 *     if (err == ESP_OK) {
 *         ESP_LOGI(TAG, "TVOC: %lu ppb", tvoc);
 *     }
 *
 *     ags10_free_desc(&dev);
 * }
 * @endcode
 *
 * @param[out] dev Pointer to the I2C device descriptor
 * @param[in] port I2C port number
 * @param[in] addr I2C address of the sensor
 * @param[in] sda_gpio GPIO number for SDA
 * @param[in] scl_gpio GPIO number for SCL
 * @return ESP_OK on success
 */
esp_err_t ags10_init_desc(i2c_dev_t *dev, i2c_port_t port, uint8_t addr, gpio_num_t sda_gpio, gpio_num_t scl_gpio);

/**
 * @brief Free the AGS10 sensor descriptor
 *
 * This function cleans up the I2C device descriptor resources.
 *
 * @param[in] dev Pointer to the I2C device descriptor
 * @return ESP_OK on success
 */
esp_err_t ags10_free_desc(i2c_dev_t *dev);

/**
 * @brief Read TVOC value from the sensor
 *
 * This function reads the Total Volatile Organic Compounds (TVOC) value
 * from the AGS10 sensor. The value is returned in parts per billion (ppb).
 *
 * @note The sensor requires some warm-up time after power-on for accurate readings.
 *       Typical warm-up time is around 10-60 seconds.
 *
 * @param[in] dev Pointer to the I2C device descriptor
 * @param[out] tvoc Pointer to store the TVOC value in ppb
 * @return
 *   - ESP_OK: Success
 *   - ESP_ERR_INVALID_ARG: Invalid argument
 *   - ESP_FAIL: Communication failure
 */
esp_err_t ags10_read_tvoc(i2c_dev_t *dev, uint32_t *tvoc);

/**
 * @brief Read firmware version from the sensor
 *
 * This function reads the firmware version of the AGS10 sensor.
 *
 * @param[in] dev Pointer to the I2C device descriptor
 * @param[out] version Pointer to store the firmware version
 * @return
 *   - ESP_OK: Success
 *   - ESP_ERR_INVALID_ARG: Invalid argument
 *   - ESP_FAIL: Communication failure
 */
esp_err_t ags10_read_version(i2c_dev_t *dev, uint8_t *version);

/**
 * @brief Read resistance value from the sensor
 *
 * This function reads the raw gas resistance value from the AGS10 sensor.
 * This value can be used for advanced calibration or diagnostics.
 *
 * @param[in] dev Pointer to the I2C device descriptor
 * @param[out] resistance Pointer to store the resistance value
 * @return
 *   - ESP_OK: Success
 *   - ESP_ERR_INVALID_ARG: Invalid argument
 *   - ESP_FAIL: Communication failure
 */
esp_err_t ags10_read_resistance(i2c_dev_t *dev, uint32_t *resistance);

/**
 * @brief Set new I2C address for the sensor
 *
 * This function changes the I2C address of the AGS10 sensor.
 * The new address will be persistent after power cycling.
 *
 * @warning Use this function with caution. Make sure no other I2C devices
 *          use the new address to avoid conflicts.
 *
 * @param[in,out] dev Pointer to the I2C device descriptor (address will be updated)
 * @param[in] new_address New I2C address (7-bit, range: 0x08-0x77)
 * @return
 *   - ESP_OK: Success
 *   - ESP_ERR_INVALID_ARG: Invalid argument
 *   - ESP_FAIL: Communication failure
 */
esp_err_t ags10_set_i2c_address(i2c_dev_t *dev, uint8_t new_address);

/**
 * @brief Set zero-point calibration with factory defaults
 *
 * This function resets the zero-point calibration to factory default values.
 * This is useful when the sensor readings drift over time.
 *
 * @param[in] dev Pointer to the I2C device descriptor
 * @return
 *   - ESP_OK: Success
 *   - ESP_ERR_INVALID_ARG: Invalid argument
 *   - ESP_FAIL: Communication failure
 */
esp_err_t ags10_set_zero_point_with_factory_defaults(i2c_dev_t *dev);

/**
 * @brief Set zero-point calibration with current resistance
 *
 * This function sets the current resistance reading as the zero point.
 * Use this function in clean air conditions for calibration.
 *
 * @param[in] dev Pointer to the I2C device descriptor
 * @return
 *   - ESP_OK: Success
 *   - ESP_ERR_INVALID_ARG: Invalid argument
 *   - ESP_FAIL: Communication failure
 */
esp_err_t ags10_set_zero_point_with_current_resistance(i2c_dev_t *dev);

/**
 * @brief Set zero-point calibration with a specific value
 *
 * This function sets the zero-point calibration to a specific value.
 * This is for advanced users who know the exact calibration value.
 *
 * @param[in] dev Pointer to the I2C device descriptor
 * @param[in] value Calibration value (0x0000 = current resistance, 0xFFFF = factory defaults)
 * @return
 *   - ESP_OK: Success
 *   - ESP_ERR_INVALID_ARG: Invalid argument
 *   - ESP_FAIL: Communication failure
 */
esp_err_t ags10_set_zero_point_with(i2c_dev_t *dev, uint16_t value);

#ifdef __cplusplus
}
#endif

/**@}*/

#endif /* __AGS10_H__ */
