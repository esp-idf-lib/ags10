# AGS10 Basic TVOC Reading Example

This example demonstrates how to:

- Initialize the AGS10 sensor
- Read the firmware version
- Continuously read TVOC (Total Volatile Organic Compounds) values

## Wiring

Connect AGS10 sensor to ESP32:

| AGS10 | ESP32 |
|-------|-------|
| VCC   | 3.3V  |
| GND   | GND   |
| SDA   | GPIO21|
| SCL   | GPIO22|

## Expected Output

```
I (xxx) ags10_basic_example: AGS10 Basic Example
I (xxx) ags10_basic_example: AGS10 firmware version: v1
I (xxx) ags10_basic_example: Starting TVOC measurements (readings every 5 seconds)...
I (xxx) ags10_basic_example: TVOC: 45 ppb
I (xxx) ags10_basic_example: TVOC: 47 ppb
I (xxx) ags10_basic_example: TVOC: 44 ppb
...
```

## Notes

- The sensor may take some time to warm up after power-on for accurate readings
- TVOC values are returned in parts per billion (ppb)
- Typical indoor TVOC levels range from 0-2000 ppb
