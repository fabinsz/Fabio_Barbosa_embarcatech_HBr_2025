#include "hal_led.h"
#include "led_embutido.h"
#include "pico/stdlib.h"

static int estado_led = 0;

void hal_led_init(void) {
    led_embutido_init();
}

void hal_led_toggle(void) {
    estado_led = !estado_led;
    led_embutido_set(estado_led);
}
