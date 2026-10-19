# Wiring and Setup

## Hardware

- Arduino Uno
- MH-Sensor-Series (Flying-Fish) MH-B IR obstacle-avoidance / line-tracking module
- 16×2 LCD with I2C backpack
- Rotating target that creates two detectable pulses per revolution

## Connections

| Signal | Arduino Uno |
|---|---|
| Sensor digital output | D2 |
| LCD SDA | A4 |
| LCD SCL | A5 |
| LCD power and ground | Follow the specific module labels and voltage requirements |
| Sensor power and ground | Follow the sensor module documentation |

The sketch expects the LCD I2C address to be `0x27`. Some LCD backpacks use another address.

## Upload

1. Open `src/ArduinoUnoTachometer/ArduinoUnoTachometer.ino` in Arduino IDE.
2. Install a compatible `LiquidCrystal_I2C` library.
3. Select Arduino Uno and the correct port.
4. Upload the sketch and open Serial Monitor at 9600 baud.

## Calibration

Set `PULSES_PER_REVOLUTION` to the number of rising edges the sensor detects during one complete revolution. The current setting is 2. Confirm this by rotating the target slowly and counting the detected pulses.

The sketch reports 0 RPM if no pulse arrives for 100,000 microseconds. This is a deliberate stopped/timeout threshold, not a calibrated accuracy specification.
