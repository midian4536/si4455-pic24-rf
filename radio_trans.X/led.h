#ifndef LED_H
#define LED_H

#include <stdint.h>

#define TRIS_LED TRISAbits.TRISA6
#define LED LATAbits.LATA6

void led_init();

void led_shine(uint16_t num, uint16_t time);

#endif