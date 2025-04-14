#ifndef RADIO_H
#define RADIO_H

#include <stdbool.h>

#define RADIO_MAX_PACKET_LENGTH (64)

extern uint8_t custom_radio_packet[RADIO_MAX_PACKET_LENGTH];

void radio_power_up(void);
void radio_init(void);
void radio_start_rx(uint8_t channel);
void radio_start_tx(uint8_t channel, uint8_t *pdata);
void radio_start_tx_variable_packet(uint8_t channel, uint8_t *pdata, uint8_t len);

bool radio_check_received(void);
bool radio_check_transmitted(void);
bool radio_check_received_variable_packet(void);

#endif
