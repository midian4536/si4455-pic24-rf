#ifndef RADIO_COMM_H
#define RADIO_COMM_H

#define RADIO_CTS_TIMEOUT 10000

extern uint8_t cts_went_high;

uint8_t radio_comm_get_resp(uint8_t byte_count, uint8_t *pdata);

void radio_comm_send_cmd(uint8_t byte_count, uint8_t *pdata);
void radio_comm_read_data(uint8_t cmd, uint8_t poll_cts, uint8_t byte_count, uint8_t *pdata);
void radio_comm_write_data(uint8_t cmd, uint8_t poll_cts, uint8_t byte_count, uint8_t *pdata);

uint8_t radio_comm_poll_cts(void);

uint8_t radio_comm_send_cmd_get_resp(uint8_t cmd_byte_count, uint8_t *p_cmd_data, uint8_t resp_byte_count, uint8_t *p_resp_data);

void radio_comm_clear_cts(void);

#endif
