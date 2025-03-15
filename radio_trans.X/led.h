#ifndef LED_H
#define LED_H

#define TRIS_LED TRISAbits.TRISA6

#define LED LATAbits.LATA6

void led_init();

void led_shine(unsigned int num, unsigned int time);

#endif