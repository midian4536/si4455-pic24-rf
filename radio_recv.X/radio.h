#ifndef RADIO_H
#define RADIO_H

#define RADIO_MAX_PACKET_LENGTH 64

typedef struct {
    unsigned char *radio_configuration_array;
    unsigned char radio_channel_number;
    unsigned char radio_packet_length;
    unsigned char radio_state_after_power_up;
    unsigned short radio_delay_cnt_after_reset;

    unsigned char radio_variable_packet_state;
    unsigned char radio_custom_payload[RADIO_MAX_PACKET_LENGTH];
} radio_configuration_t;

extern const unsigned char radio_configuration_data_array[];
extern const radio_configuration_t *p_radio_configuration;
extern unsigned char custom_radio_packet[RADIO_MAX_PACKET_LENGTH];
extern unsigned char radio_packet_length;

void radio_power_up(void);
void radio_init(void);
void radio_start_rx(unsigned char channel);
void radio_start_tx(unsigned char channel, unsigned char *pdata);
void radio_start_tx_variable_packet(unsigned char channel, unsigned char *pdata, unsigned char length);

unsigned char radio_check_received(void);
unsigned char radio_check_transmitted(void);
unsigned char radio_check_received_variable_packet(void);

#endif
