# Analog Joystick Reader – Raspberry Pi Pico

## Task Logic Explanation

Este projeto em C utiliza o Raspberry Pi Pico para ler os valores analógicos de um joystick (eixo X e Y) utilizando os canais ADC e exibir os valores em tempo real em um display OLED via I2C.

### System Behavior

- O sistema lê continuamente os sinais dos pinos analógicos **GPIO26 (ADC0 - eixo Y)** e **GPIO27 (ADC1 - eixo X)**.
- A cada ~50ms, os valores brutos dos dois canais ADC (de 0 a 4095) são atualizados e exibidos no display OLED SSD1306.
- O display mostra os rótulos "X:" e "Y", seguidos pelos respectivos valores lidos dos eixos.
- O valor de cada eixo é mostrado em sua linha correspondente no display.

---

## Requirements

- Raspberry Pi Pico  
- Display OLED SSD1306 (I2C)  
- Joystick analógico com saídas nos eixos X e Y (conectado aos GPIOs 26 e 27)  
- Raspberry Pi Pico SDK  
- Biblioteca SSD1306 compatível com o SDK  

---

## Pin Connections

| Componente         | Pino Pico | Função        |
|--------------------|-----------|---------------|
| OLED - SDA         | GPIO14    | I2C SDA       |
| OLED - SCL         | GPIO15    | I2C SCL       |
| Joystick - Eixo Y  | GPIO26    | ADC0          |
| Joystick - Eixo X  | GPIO27    | ADC1          |

---

## Notes

- O display é inicializado usando a interface **I2C1** com frequência de 400kHz.
- Os pinos I2C (GPIO14 e GPIO15) são configurados com resistores de pull-up.
- A função `draw_int_on_oled()` converte os valores inteiros dos ADCs para strings e os escreve no display.

---


