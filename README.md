<div align="center">

# Arduino Uno Digital Tachometer

**Real-time rotational speed measurement using Arduino Uno, an IR sensor, and a 16×2 I2C LCD**

</div>

---

## Overview

This project measures the rotational speed of a rotating object in **revolutions per minute (RPM)**. An IR obstacle-avoidance / line-tracking sensor module detects changes as the target rotates. An Arduino Uno measures the time between detected pulses, calculates the RPM, and displays the result on a 16×2 LCD with an I2C interface.

The project has been implemented and tested on **physical hardware** and also demonstrated in **simulation**.

## Project Demo

### Physical implementation

[**Watch the physical project demo**](Tachometer.mp4)

### Simulation

[**Watch the simulation demo**](Tachometer%20Simulation%20Vid.mp4)

## Hardware

| Component | Description |
|---|---|
| Microcontroller | Arduino Uno |
| Sensor | MH-Sensor-Series (Flying-Fish) MH-B IR Obstacle Avoidance / Line Tracking Sensor Module |
| Display | 16×2 LCD with I2C interface |
| LCD I2C address | `0x27` (as configured in the sketch) |
| Pulse input | Arduino digital pin `D2` (external interrupt) |

## Circuit

### Circuit diagram

![Tachometer circuit diagram](Tachometer%20Circuit.png)

### Simulation — idle / OFF

![Tachometer simulation with measurement OFF](Tachometer%20Simulation%20OFF.png)

### Simulation — active / ON

![Tachometer simulation with measurement ON](Tachometer%20Simulation%20ON.png)

## How It Works

1. The IR sensor detects the rotating target and produces output pulses.
2. The Arduino Uno detects rising edges on digital pin `D2` using an external interrupt.
3. The sketch measures the time between pulses with `micros()`.
4. The pulse period is used to calculate rotational speed.
5. The RPM value is shown on the LCD and measurement details are printed to the Serial Monitor.

The current sketch assumes **2 pulses per revolution**, configured by `PulsesPerRevolution`.

## Hardware Connections

| Module signal | Arduino Uno connection |
|---|---|
| IR sensor output | `D2` |
| LCD SDA | `A4` |
| LCD SCL | `A5` |
| LCD VCC | `5V` (verify the display module's requirements) |
| LCD GND | `GND` |
| IR sensor VCC / GND | Connect to the supply and ground specified for the sensor module |

Check the pin labels on your specific modules before wiring. The LCD address in the sketch is `0x27`; some I2C backpacks use a different address.

## Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software)
- `LiquidCrystal_I2C` library compatible with your installed Arduino environment

## Getting Started

1. Clone or download this repository.
2. Open the Arduino sketch (`.ino`) in the Arduino IDE.
3. Install a compatible `LiquidCrystal_I2C` library.
4. Connect the sensor output to `D2` and wire the LCD over I2C as described above.
5. Select **Arduino Uno** and the correct port in the Arduino IDE.
6. Upload the sketch.
7. Open the Serial Monitor at **9600 baud**.
8. Rotate the target and observe the RPM reading on the LCD and Serial Monitor.

## Serial Monitor Output

The sketch prints the following fields:

- `Period`: measured interval between pulses.
- `Readings`: pulse averaging parameter used by the sketch.
- `Frequency`: calculated frequency value.
- `RPM`: calculated rotational speed.
- `Tachometer`: average of the stored RPM readings.

## Notes

- The configured value `PulsesPerRevolution = 2` must match the number of detected pulses per full revolution for the RPM calculation to be correct.
- The displayed LCD value is the current `RPM` variable; the `Tachometer` value printed to Serial is the average of the stored readings.
- Sensor alignment, target surface, ambient light, and pulse detection can affect readings. Validate the sensor position for your setup.
- This README documents the current project behavior; it does not claim a specific measurement accuracy or maximum RPM.

## Repository Contents

- `Tachometer Circuit.png` — circuit image
- `Tachometer Simulation OFF.png` — simulation image before active measurement
- `Tachometer Simulation ON.png` — simulation image during active measurement
- `Tachometer Simulation Vid.mp4` — simulation video
- `Tachometer.mp4` — physical project video

## License

No license has been specified in this repository yet. Unless a license is added, reuse and redistribution are subject to the default copyright rules.
