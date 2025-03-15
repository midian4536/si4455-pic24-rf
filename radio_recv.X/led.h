#ifndef LED_H
#define LED_H

#define TRIS_LED TRISBbits.TRISB12

#define LED LATBbits.LATB12

void led_init();

void led_shine(unsigned int num, unsigned int time);

#endif