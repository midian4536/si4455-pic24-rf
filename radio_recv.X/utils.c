#include "utils.h"

void delay_ms(unsigned int n) {
    for (unsigned int i = 0; i < 1000; i++)
        for (unsigned int j = 0; j < n; j++)
            ;
}
