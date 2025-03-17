#include "xc.h"
#include "adc.h"

void adc_init(void) {
    AD1PCFG = 0xffff;
}
