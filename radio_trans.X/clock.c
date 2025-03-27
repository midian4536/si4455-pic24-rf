#include "xc.h"
#include "clock.h"

void clock_init(void) {
    CLKDIVbits.RCDIV = 0;
}

void delay_ms(uint16_t n) {
    __delay_ms(n);
}
