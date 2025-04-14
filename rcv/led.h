#ifndef LED_H
#define LED_H

#define TRIS_LED _TRISA6
#define LED _LATA6

void led_init();

void led_shine(uint16_t num, uint16_t time);

#endif