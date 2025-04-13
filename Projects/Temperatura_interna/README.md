# Internal Temperature Reader – Raspberry Pi Pico

## Task Logic Explanation

This C project uses the Raspberry Pi Pico to read the **internal temperature** from the onboard sensor connected to ADC channel 4. The data is displayed in real time on an SSD1306 OLED display via I2C communication.

### System Behavior

- The system reads the RP2040's internal temperature using the sensor connected to **ADC channel 4**.
- To improve precision, **100 samples** are collected and averaged.
- Every second, the temperature value in degrees Celsius (°C) is displayed on the OLED screen.
- The temperature is also printed to the serial terminal via the USB port.

---

## Requirements

- Raspberry Pi Pico  
- SSD1306 OLED Display (I2C)  
- Raspberry Pi Pico SDK  
- SSD1306-compatible library for the SDK (with support for `ssd1306_draw_string()` and `render_on_display()`)  

---

## Pin Connections

| Component         | Pico Pin | Function       |
|------------------|----------|----------------|
| OLED - SDA       | GPIO14   | I2C SDA        |
| OLED - SCL       | GPIO15   | I2C SCL        |

---

## Notes

- The RP2040 temperature sensor is read via **ADC channel 4**, as specified in the datasheet.
- The raw ADC reading is converted to degrees Celsius using the official formula:  
  \[
  T = 27 - \frac{(V_{\text{sensor}} - 0.706)}{0.001721}
  \]
- The ADC operates at 12-bit resolution (0 to 4095) with a reference voltage of 3.3V.
- The function `draw_float_on_oled()` converts float values to strings with configurable precision for display.
- The display is initialized on **I2C1**, with pull-up resistors enabled on GPIO14 and GPIO15.

