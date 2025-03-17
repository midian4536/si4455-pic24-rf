#include "led.h"
#include "xc.h"
#include "clock.h"

void led_init() {
    TRIS_LED = 0;
    LED = 0;
}

void led_shine(unsigned int num, unsigned int time) {
    for (unsigned int i = 0; i < num; i++) {
        LED = 1;
        delay_ms(time);
        LED = 0;
        delay_ms(time);
    }
}
