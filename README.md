# Ardu-meter

An Arduino-compatible power meter using an **INA226** current sensor and a
**128 x 64 SSD1306 I2C OLED**. It shows bus voltage (V), current (mA), and power
(mW) on the display and over serial at 115200 baud, refreshing roughly every
100 ms after the original "Bungle's Ammeter" startup animation.

The current sketch uses INA226. Earlier project notes and photos may show an
INA219; that sensor requires different firmware.

## Repository layout

- [`firmware/ardu_meter/ardu_meter.ino`](firmware/ardu_meter/ardu_meter.ino) — Arduino sketch.
- [`powermeter/3d file.stl`](powermeter/3d%20file.stl) — original 3D model.

## Hardware and wiring

Connect both modules to the board's default I2C bus, as used by `Wire.begin()`.

| Signal | Connection |
| --- | --- |
| SDA | Board SDA to INA226 SDA and OLED SDA |
| SCL | Board SCL to INA226 SCL and OLED SCL |
| GND | Common ground for the board, sensor, display, and measured circuit |
| VCC | Supply each module according to its voltage rating and the board's logic level |

Use your board's pinout to identify SDA and SCL. For a board using custom I2C
pins, configure `Wire` before initializing the devices. Place the INA226 shunt
in series with the load: supply positive to IN+, IN- to load positive, and load
negative to the common ground.

The sketch uses INA226 address **0x40** and OLED address **0x3C**. Adjust
`INA226_ADDRESS` and `OLED_ADDRESS` if your modules differ.

## Libraries and upload

Install these libraries from the Arduino IDE Library Manager:

- [INA226 by Rob Tillaart](https://github.com/RobTillaart/INA226), version **0.6.6**.
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306), including its
  Adafruit GFX and Adafruit BusIO dependencies when prompted.

1. Open `firmware/ardu_meter/ardu_meter.ino`. Keep the sketch in its matching
   `ardu_meter` folder.
2. Set `SHUNT_RESISTANCE_OHMS` to the resistor actually fitted to your INA226
   module, then set `MAX_CURRENT_A` for your measurement range.
3. Select your board and port, then upload.
4. Open Serial Monitor at **115200 baud** to see readings or startup errors.

The supplied **0.1 ohm / 0.8 A settings are an example**, not a confirmed
specification of the pictured hardware. Calibration is required for current
readings. The library requires `MAX_CURRENT_A * SHUNT_RESISTANCE_OHMS <= 0.0819`
and reports an error if the selected range is invalid. The configured range
does not change the module's electrical ratings.

Power is calculated as `voltage_V * current_mA`, which gives milliwatts.
For example, 5 V at 100 mA is 500 mW.

### Command-line build

With Arduino CLI installed, this example compiles for an Arduino Uno:

```sh
arduino-cli core update-index
arduino-cli core install arduino:avr
arduino-cli lib install "INA226@0.6.6" "Adafruit SSD1306"
arduino-cli compile --fqbn arduino:avr:uno firmware/ardu_meter
```

For an ESP32, install its board package and select the FQBN for your specific
board. Compilation checks do not verify physical wiring, calibration, or
measurement accuracy.

## Troubleshooting

- **Failed to find INA226:** check power, SDA/SCL, and the sensor address.
- **Calibration failed:** check the shunt resistance and requested current range.
- **SSD1306 initialization failed:** check the display configuration and available
  memory; also verify power, wiring, and the OLED address if the screen is blank.
- **Incorrect current:** verify the physical shunt value and compare readings
  against a reference meter.

## Project photos

![1740912960768](https://github.com/user-attachments/assets/a5b532ed-4ee5-42ea-b846-1b673e6fc9da)

![1740452512384](https://github.com/user-attachments/assets/ac38430c-6a8c-4615-955a-afeba3d3275d)

[ESP32 project update on LinkedIn](https://www.linkedin.com/posts/sooryajohn_pt2-of-the-power-meter-now-using-an-esp32-activity-7300427251668434944-mYGN)
