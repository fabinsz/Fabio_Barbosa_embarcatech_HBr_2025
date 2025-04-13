# Internal Temperature Reader – Raspberry Pi Pico

## Task Logic Explanation

Este projeto em C utiliza o Raspberry Pi Pico para ler a **temperatura interna do chip** através do sensor embutido conectado ao canal ADC4. Os dados são exibidos em tempo real em um display OLED SSD1306 via comunicação I2C.

### System Behavior

- O sistema lê a temperatura interna do RP2040 utilizando o sensor conectado ao **canal ADC4**.
- Para aumentar a precisão da leitura, são coletadas **100 amostras** e calculada a média.
- A cada segundo, o valor em graus Celsius (ºC) é exibido no display OLED.
- A temperatura também é impressa no terminal serial através da porta USB.

---

## Requirements

- Raspberry Pi Pico  
- Display OLED SSD1306 (I2C)  
- Raspberry Pi Pico SDK  
- Biblioteca SSD1306 compatível com o SDK (com suporte a `ssd1306_draw_string()` e `render_on_display()`)  

---

## Pin Connections

| Componente       | Pino Pico | Função        |
|------------------|-----------|---------------|
| OLED - SDA       | GPIO14    | I2C SDA       |
| OLED - SCL       | GPIO15    | I2C SCL       |

---

## Notes

- O sensor de temperatura do RP2040 é lido através do canal **ADC4**, conforme especificado no datasheet.
- A conversão da leitura bruta do ADC para temperatura em ºC utiliza a fórmula oficial:  
  \[
  T = 27 - \frac{(V_{\text{sensor}} - 0.706)}{0.001721}
  \]
- A leitura do ADC é feita com resolução de 12 bits (0 a 4095) e referência de tensão de 3.3V.
- A função `draw_float_on_oled()` converte valores `float` em strings com precisão ajustável para exibição no display.
- O display é inicializado na interface **I2C1**, com resistores de pull-up ativados nos pinos GPIO14 e GPIO15.

