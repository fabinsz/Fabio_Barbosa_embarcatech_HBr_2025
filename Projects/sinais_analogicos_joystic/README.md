# Analog Joystick Reader – Raspberry Pi Pico

## Task Logic Explanation

This C project uses the Raspberry Pi Pico to read analog values from a joystick (X and Y axes) using the ADC channels and display the values in real time on an OLED display via I2C.

### System Behavior

- The system continuously reads signals from the analog pins **GPIO26 (ADC0 - Y axis)** and **GPIO27 (ADC1 - X axis)**.
- Every ~50ms, the raw values from both ADC channels (0 to 4095) are updated and displayed on the SSD1306 OLED display.
- The display shows the labels "X:" and "Y", followed by the respective values read from the axes.
- The value of each axis is displayed in its corresponding line on the screen.

---

## Requirements

- Raspberry Pi Pico  
- SSD1306 OLED Display (I2C)  
- Analog joystick with X and Y axis outputs (connected to GPIOs 26 and 27)  
- Raspberry Pi Pico SDK  
- SSD1306-compatible library for the SDK  

---

## Pin Connections

| Component         | Pico Pin | Function       |
|-------------------|----------|----------------|
| OLED - SDA        | GPIO14   | I2C SDA        |
| OLED - SCL        | GPIO15   | I2C SCL        |
| Joystick - Y axis | GPIO26   | ADC0           |
| Joystick - X axis | GPIO27   | ADC1           |

---

## Notes

- The display is initialized using the **I2C1** interface with a frequency of 400kHz.
- The I2C pins (GPIO14 and GPIO15) are configured with pull-up resistors.
- The function `draw_int_on_oled()` converts integer ADC values to strings and writes them to the display.
