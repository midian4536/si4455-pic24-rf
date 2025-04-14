#include <xc.h>
#include "led.h"
#include "delay.h"

void led_init()
{
    TRIS_LED = 0;
    LED = 0;
}

void led_shine(uint16_t num, uint16_t time)
{
    for (uint16_t i = 0; i < num; i++)
    {
        LED = 1;
        __delay_ms(time);
        LED = 0;
        __delay_ms(time);
    }
}
