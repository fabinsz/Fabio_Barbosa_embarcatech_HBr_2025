# Countdown with Interrupts – Raspberry Pi Pico

## Task Logic Explanation

This C project implements a countdown timer controlled by buttons using the Raspberry Pi Pico and an OLED display via I2C.

### System Behavior

- **Button A (GPIO5)**: Resets the counter to 9 and starts the countdown.

- **During the countdown (from 9 to 0)**:
  - The counter decreases by 1 every second.
  - Each press of **Button B (GPIO6)** is counted and shown on the display.

- **When the counter reaches 0**:
  - The countdown stops.
  - The number of Button B presses is preserved.
  - Further Button B presses are ignored until the counter is restarted with Button A.

- **Pressing Button A again**:
  - Restarts the process: the counter goes back to 9 and the Button B press count is reset.

---

## Requirements

- Raspberry Pi Pico
- SSD1306 OLED Display (I2C)
- 2 push buttons (for GPIO5 and GPIO6)
- Raspberry Pi Pico SDK
- SSD1306 display driver library

---

