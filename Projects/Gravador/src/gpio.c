#include "hardware/gpio.h"

#define BUTTON_A       5
#define BUTTON_B       6
#define LED_VERMELHO  13
#define LED_VERDE     11

void init_gpio(void) {
    // Inicializa botões como entrada com pull-up
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Inicializa LEDs como saída e os desliga
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_put(LED_VERMELHO, 0);

    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_put(LED_VERDE, 0);
}
