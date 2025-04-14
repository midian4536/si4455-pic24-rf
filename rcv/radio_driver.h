#ifndef RADIO_DRIVER_H
#define RADIO_DRIVER_H

#define TRIS_SDN _TRISA1
#define SDN _LATA1

#define TRIS_NIRQ _TRISB14
#define nIRQ _RB14

#define TRIS_nSel _TRISB15
#define nSel _LATB15

#define SI4455_USER_CONFIG_XTAL_FREQ (30000000L)

extern union si4455_cmd_reply_union si4455_cmd;
extern uint8_t radio_cmd[16];

enum
{
    SI4455_SUCCESS,
    SI4455_NO_PATCH,
    SI4455_CTS_TIMEOUT,
    SI4455_PATCH_FAIL,
    SI4455_COMMAND_ERROR
};

void si4455_reset(void);
void si4455_power_up(uint8_t boot_options, uint8_t xtal_options, uint32_t xo_freq);

uint8_t si4455_configuration_init(const uint8_t *pset_prop_cmd);
void si4455_write_ezconfig_array(const uint8_t *pezconfig_array, uint8_t len);
void si4455_ezconfig_check(uint16_t check_sum);

void si4455_start_tx(uint8_t channel, uint8_t condition, uint16_t tx_len);
void si4455_start_rx(uint8_t channel, uint8_t condition, uint16_t rx_len, uint8_t next_stage1, uint8_t next_stage2, uint8_t next_stage3);

void si4455_get_int_status(uint8_t ph_clr_pend, uint8_t modem_clr_pend, uint8_t chip_clr_pend);

void si4455_gpio_pin_cfg(uint8_t gpio0, uint8_t gpio1, uint8_t gpio2, uint8_t gpio3, uint8_t nirq, uint8_t sdo, uint8_t gen_config);

void si4455_change_state(uint8_t next_stage1);

void si4455_nop(void);

void si4455_fifo_info(uint8_t fifo);
void si4455_part_info(void);

void si4455_write_tx_fifo(uint8_t *ptx_data, uint8_t len);
void si4455_read_rx_fifo(uint8_t *prx_data, uint8_t len);

void si4455_get_property(uint8_t group, uint8_t num_props, uint8_t start_prop);

void si4455_func_info(void);

void si4455_frr_a_read(uint8_t resp_bytes);
void si4455_frr_b_read(uint8_t resp_bytes);
void si4455_frr_c_read(uint8_t resp_bytes);
void si4455_frr_d_read(uint8_t resp_bytes);

void si4455_device_state(void);

void si4455_read_cmd_buff(void);
void si4455_get_adc_reading(uint8_t adc_en, uint8_t adc_cfg);

void si4455_get_ph_status(uint8_t ph_clr_pend);
void si4455_get_modem_status(uint8_t modem_clr_pend);
void si4455_get_chip_status(uint8_t chip_clr_pend);

#endif