#ifndef SI4455_API_LIB_H
#define SI4455_API_LIB_H

#define SI4455_USER_CONFIG_XTAL_FREQ 30000000L

extern union si4455_cmd_reply_union si4455_cmd;
extern unsigned char radio_cmd[16];

enum
{
    SI4455_SUCCESS,
    SI4455_NO_PATCH,
    SI4455_CTS_TIMEOUT,
    SI4455_PATCH_FAIL,
    SI4455_COMMAND_ERROR
};

void si4455_reset(void);
void si4455_power_up(unsigned char boot_options, unsigned char xtal_options, unsigned long xo_freq);

unsigned char si4455_configuration_init(unsigned char *p_set_prop_cmd);
void si4455_write_ezconfig_array(unsigned char num_bytes, unsigned char *p_ezconfig_array);
void si4455_ezconfig_check(unsigned short check_sum);

void si4455_start_tx(unsigned char channel, unsigned char condition, unsigned short tx_len);
void si4455_start_rx(unsigned char channel, unsigned char condition, unsigned short rx_len, unsigned char next_stage1, unsigned char next_stage2, unsigned char next_stage3);

void si4455_get_int_status(unsigned char ph_clr_pend, unsigned char modem_clr_pend, unsigned char chip_clr_pend);

void si4455_gpio_pin_cfg(unsigned char gpio0, unsigned char gpio1, unsigned char gpio2, unsigned char gpio3, unsigned char nirq, unsigned char sdo, unsigned char gen_config);

void si4455_change_state(unsigned char next_stage1);

void si4455_nop(void);

void si4455_fifo_info(unsigned char fifo);
void si4455_part_info(void);

void si4455_write_tx_fifo(unsigned char num_bytes, unsigned char *p_tx_data);
void si4455_read_rx_fifo(unsigned char num_bytes, unsigned char *p_rx_data);

void si4455_get_property(unsigned char group, unsigned char num_props, unsigned char start_prop);

void si4455_func_info(void);

void si4455_frr_a_read(unsigned char resp_bytes);
void si4455_frr_b_read(unsigned char resp_bytes);
void si4455_frr_c_read(unsigned char resp_bytes);
void si4455_frr_d_read(unsigned char resp_bytes);

void si4455_device_state(void);

void si4455_read_cmd_buff(void);
void si4455_get_adc_reading(unsigned char adc_en, unsigned char adc_cfg);

void si4455_get_ph_status(unsigned char ph_clr_pend);
void si4455_get_modem_status(unsigned char modem_clr_pend);
void si4455_get_chip_status(unsigned char chip_clr_pend);

#endif
