#include "xc.h"
#include "clock.h"

void clock_init(void) {
    CLKDIVbits.RCDIV = 0;
}

void delay_ms(unsigned int n) {
    for (unsigned int i = 0; i < 1000; i++)
        for (unsigned int j = 0; j < n; j++)
            ;
}
