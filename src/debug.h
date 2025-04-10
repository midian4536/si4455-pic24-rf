#ifndef DEBUG_H
#define	DEBUG_H

#include <stdint.h>

void char_to_hex(uint8_t value, uint8_t *hex1, uint8_t *hex2);

void show_si4455_properties();
void show_si4455_fifo();
void show_si4455_state();

#endif