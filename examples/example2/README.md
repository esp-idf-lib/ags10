# AGS10 Advanced Example with Calibration

This example demonstrates advanced features of the AGS10 sensor:

- Reading both TVOC and raw resistance values
- Zero-point calibration with current resistance
- Factory reset calibration
- Continuous monitoring with detailed logging

## Wiring

Connect AGS10 sensor to ESP32:

| AGS10 | ESP32 |
|-------|-------|
| VCC   | 3.3V  |
| GND   | GND   |
| SDA   | GPIO21|
| SCL   | GPIO22|

## Features Demonstrated

1. **Initial readings** - Shows TVOC and raw resistance values
2. **Zero-point calibration** - Sets current environment as baseline (requires clean air)
3. **Continuous monitoring** - Takes readings every 10 seconds
4. **Factory reset** - Resets calibration to factory defaults after 10 readings

## Expected Output

```
I (xxx) ags10_advanced_example: AGS10 Advanced Example
I (xxx) ags10_advanced_example: AGS10 firmware version: v1
I (xxx) ags10_advanced_example: Initial readings:
I (xxx) ags10_advanced_example: TVOC: 45 ppb
I (xxx) ags10_advanced_example: Raw resistance: 123456
I (xxx) ags10_advanced_example: Performing zero-point calibration with current resistance...
I (xxx) ags10_advanced_example: Make sure the sensor is in clean air environment!
I (xxx) ags10_advanced_example: Zero-point calibration successful
I (xxx) ags10_advanced_example: Starting continuous measurements (every 10 seconds)...
I (xxx) ags10_advanced_example: === Reading #1 ===
I (xxx) ags10_advanced_example: TVOC: 12 ppb
I (xxx) ags10_advanced_example: Raw resistance: 123456
...
```

## Calibration Notes

- **Zero-point calibration**: Use in clean air environment to set baseline
- **Factory reset**: Restores original factory calibration
- **Raw resistance**: Shows the actual sensor resistance value for diagnostics

## Important

- Ensure clean air environment during zero-point calibration
- Allow sensor warm-up time for accurate readings
- Raw resistance values can help diagnose sensor condition
