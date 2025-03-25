#ifndef RADIO_H
#define RADIO_H

#define RADIO_MAX_PACKET_LENGTH 64

typedef struct {
    uint8_t *radio_configuration_array;
    uint8_t radio_channel_number;
    uint8_t radio_packet_length;
    uint8_t radio_state_after_power_up;
    uint16_t radio_delay_cnt_after_reset;

    uint8_t radio_variable_packet_state;
    uint8_t radio_custom_payload[RADIO_MAX_PACKET_LENGTH];
} radio_configuration_t;

extern const uint8_t radio_configuration_data_array[];
extern const radio_configuration_t *p_radio_configuration;
extern uint8_t custom_radio_packet[RADIO_MAX_PACKET_LENGTH];

void radio_power_up(void);
void radio_init(void);
void radio_start_rx(uint8_t channel);
void radio_start_tx(uint8_t channel, uint8_t *pdata);
void radio_start_tx_variable_packet(uint8_t channel, uint8_t *pdata, uint8_t length);

uint8_t radio_check_received(void);
uint8_t radio_check_transmitted(void);
uint8_t radio_check_received_variable_packet(void);

#endif
