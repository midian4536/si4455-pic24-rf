#ifndef RADIO_COMM_H
#define RADIO_COMM_H

#define RADIO_CTS_TIMEOUT 10000

extern unsigned char cts_went_high;

unsigned char radio_comm_get_resp(unsigned char byte_count, unsigned char *pdata);

void radio_comm_send_cmd(unsigned char byte_count, unsigned char *pdata);
void radio_comm_read_data(unsigned char cmd, unsigned char poll_cts, unsigned char byte_count, unsigned char *pdata);
void radio_comm_write_data(unsigned char cmd, unsigned char poll_cts, unsigned char byte_count, unsigned char *pdata);

unsigned char radio_comm_poll_cts(void);

unsigned char radio_comm_send_cmd_get_resp(unsigned char cmd_byte_count, unsigned char *p_cmd_data, unsigned char resp_byte_count, unsigned char *p_resp_data);

void radio_comm_clear_cts(void);

#endif
