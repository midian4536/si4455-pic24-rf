#ifndef CLOCK_H
#define CLOCK_H

#define SYS_FREQ        8000000UL
#define FCY             (SYS_FREQ/2)
#include <libpic30.h> 

void clock_init(void);
void delay_ms(uint16_t n);

#endif
