# esp-idf-lib/ags10

[![Build examples](https://github.com/esp-idf-lib/ags10/actions/workflows//build.yml/badge.svg)](https://github.com/esp-idf-lib/ags10/actions/workflows//build.yml)
[![Build docs](https://github.com/esp-idf-lib/ags10/actions/workflows//build-docs.yml/badge.svg)](https://github.com/esp-idf-lib/ags10/actions/workflows//build-docs.yml)
[![Validation](https://github.com/esp-idf-lib/ags10/actions/workflows//validate-component.yml/badge.svg)](https://github.com/esp-idf-lib/ags10/actions/workflows//validate-component.yml)

ESP-IDF driver for AGS10 Total Volatile Organic Compounds (TVOC) sensor.

* [Documentation](https://esp-idf-lib.github.io/ags10/)
* [Repository](https://github.com/esp-idf-lib/ags10)

## Features

- Read TVOC values in parts per billion (ppb)
- Read raw gas resistance values
- Configurable zero-point calibration
- Factory reset calibration
- I2C address configuration
- Thread-safe operations
- Support for multiple ESP32 targets

## Supported Targets

- ESP32
- ESP32-C2
- ESP32-C3  
- ESP32-C5
- ESP32-C6
- ESP32-H2
- ESP32-P4
- ESP32-S2
- ESP32-S3
- ESP8266

## Installation

Add this component to your project using the ESP-IDF Component Manager:

```bash
idf.py add-dependency "esp-idf-lib/ags10"
```

## Usage

### Basic Usage

```c
#include <i2cdev.h>
#include <ags10.h>

// Initialize I2C
ESP_ERROR_CHECK(i2cdev_init());

// Initialize device descriptor
i2c_dev_t dev;
ESP_ERROR_CHECK(ags10_init_desc(&dev, I2C_NUM_0, AGS10_I2CADDR_DEFAULT, 
                                GPIO_NUM_21, GPIO_NUM_22));

// Read TVOC value
uint32_t tvoc;
esp_err_t err = ags10_read_tvoc(&dev, &tvoc);
if (err == ESP_OK) {
    printf("TVOC: %lu ppb\n", tvoc);
}

// Clean up
ags10_free_desc(&dev);
```

### Advanced Usage with Calibration

```c
// Read firmware version
uint8_t version;
ags10_read_version(&dev, &version);

// Read raw resistance
uint32_t resistance;
ags10_read_resistance(&dev, &resistance);

// Calibrate with current resistance (in clean air)
ags10_set_zero_point_with_current_resistance(&dev);

// Reset to factory calibration
ags10_set_zero_point_with_factory_defaults(&dev);

// Change I2C address (persistent)
ags10_set_i2c_address(&dev, 0x1B);
```

## Hardware Connection

| AGS10 Pin | ESP32 Pin | Description |
|-----------|-----------|-------------|
| VCC       | 3.3V      | Power supply |
| GND       | GND       | Ground |
| SDA       | GPIO21    | I2C Data (configurable) |
| SCL       | GPIO22    | I2C Clock (configurable) |

## Configuration

The driver uses these default settings:
- I2C Address: `0x1A` (configurable via `ags10_set_i2c_address()`)
- I2C Frequency: 20 kHz (fixed for AGS10 compatibility)

## API Reference

### Initialization Functions

- `ags10_init_desc()` - Initialize device descriptor
- `ags10_free_desc()` - Free device descriptor

### Reading Functions

- `ags10_read_tvoc()` - Read TVOC value in ppb
- `ags10_read_version()` - Read firmware version
- `ags10_read_resistance()` - Read raw gas resistance

### Calibration Functions

- `ags10_set_zero_point_with_factory_defaults()` - Reset to factory calibration
- `ags10_set_zero_point_with_current_resistance()` - Calibrate with current environment
- `ags10_set_zero_point_with()` - Set specific calibration value

### Configuration Functions

- `ags10_set_i2c_address()` - Change I2C address (persistent)

## Examples

- **example1**: Basic TVOC reading
- **example2**: Advanced usage with calibration features

## TVOC Interpretation

| TVOC Level (ppb) | Air Quality | Description |
|------------------|-------------|-------------|
| 0-65             | Excellent   | No irritation or discomfort |
| 65-220           | Good        | No irritation or discomfort |
| 220-660          | Moderate    | Irritation and discomfort possible |
| 660-2200         | Poor        | Irritation and discomfort expected |
| 2200-5500        | Very Poor   | Irritation and discomfort, more serious health issues possible |
| >5500            | Extremely Poor | More serious health issues possible |

## Important Notes

- Allow 10-60 seconds warm-up time after power-on for accurate readings
- Use zero-point calibration in clean air environment for best accuracy
- Raw resistance values can help diagnose sensor condition
- I2C address changes are persistent across power cycles
- The sensor operates at a fixed 20 kHz I2C frequency

## Thread Safety

This driver is thread-safe when `CONFIG_ESP_IDF_LIB_ENABLE_LOCKING` is enabled.

## License

Licensed under the BSD 3-Clause License. See [LICENSE](LICENSE) for details.

## Author

Copyright (c) 2024 xyzroe <i@xyzroe.cc>
* [Issues](https://github.com/esp-idf-lib/ags10/issues)
* [Discussions and questions](https://github.com/esp-idf-lib/core/discussions)
* [Component page at the ESP Component Registry](https://components.espressif.com/components/esp-idf-lib/ags10)

## Installation

```sh
idf.py add-dependency esp-idf-lib/ags10
```

## Support

For questions and discussions about the component, please use
[Discussions](https://github.com/esp-idf-lib/core/discussions)
at [esp-idf-lib/core](https://github.com/esp-idf-lib/core).

## Contributing

Please read [CONTRIBUTING.md](https://github.com/esp-idf-lib/core/blob/main/CONTRIBUTING.md)
at [esp-idf-lib/core](https://github.com/esp-idf-lib/core).
