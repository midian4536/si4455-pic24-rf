#include "xc.h"
#include "spi.h"
#include "radio_hal.h"
#include "radio_comm.h"
#include "si4455_defs.h"
#include "si4455_api_lib.h"

union si4455_cmd_reply_union si4455_cmd;
unsigned char radio_cmd[16];

void si4455_reset(void) {
    unsigned char loop_count;

    radio_hal_assert_shut_down();

    for (loop_count = 255; loop_count != 0; loop_count--)
        ;
    radio_hal_deassert_shut_down();
    for (loop_count = 255; loop_count != 0; loop_count--)
        ;
    radio_comm_clear_cts();
}

void si4455_power_up(unsigned char boot_options, unsigned char xtal_options, unsigned long xo_freq) {
    radio_cmd[0] = SI4455_CMD_ID_POWER_UP;
    radio_cmd[1] = boot_options;
    radio_cmd[2] = xtal_options;
    radio_cmd[3] = (unsigned char) (xo_freq >> 24);
    radio_cmd[4] = (unsigned char) (xo_freq >> 16);
    radio_cmd[5] = (unsigned char) (xo_freq >> 8);
    radio_cmd[6] = (unsigned char) (xo_freq);

    radio_comm_send_cmd(SI4455_CMD_ARG_COUNT_POWER_UP, radio_cmd);
}

unsigned char si4455_configuration_init(unsigned char *p_set_prop_cmd) {
    unsigned char col, response, num_bytes;

    while (*p_set_prop_cmd != 0x00) {
        num_bytes = *p_set_prop_cmd++;
        if (num_bytes > 16) {
            if (SI4455_CMD_ID_WRITE_TX_FIFO == *p_set_prop_cmd) {
                if (num_bytes > 128) {

                    return SI4455_COMMAND_ERROR;
                }

                p_set_prop_cmd++;
                si4455_write_ezconfig_array(num_bytes - 1, p_set_prop_cmd);

                p_set_prop_cmd += num_bytes - 1;

                continue;
            } else {
                return SI4455_COMMAND_ERROR;
            }
        }

        for (col = 0; col < num_bytes; col++) {
            radio_cmd[col] = *p_set_prop_cmd;
            p_set_prop_cmd++;
        }

        if (radio_comm_send_cmd_get_resp(num_bytes, radio_cmd, 1, &response) != 0xff) {
            return SI4455_CTS_TIMEOUT;
        }

        if (SI4455_CMD_ID_EZCONFIG_CHECK == radio_cmd[0]) {
            if (response) {
                return SI4455_COMMAND_ERROR;
            }
        }

        if (radio_hal_nirq_level() == 0) {
            si4455_get_int_status(0, 0, 0);
            return SI4455_COMMAND_ERROR;
        }
    }
    return SI4455_SUCCESS;
}

void si4455_write_ezconfig_array(unsigned char num_bytes, unsigned char *p_ezconfig_array) {
    radio_comm_write_data(SI4455_CMD_ID_WRITE_TX_FIFO, 1, num_bytes, p_ezconfig_array);
}

void si4455_ezconfig_check(unsigned short check_sum) {
    radio_hal_clear_nsel();

    radio_hal_spi_write_byte(SI4455_CMD_ID_EZCONFIG_CHECK);

    radio_hal_spi_write_byte((unsigned short) check_sum >> 8);
    radio_hal_spi_write_byte((unsigned short) check_sum & 0x00ff);

    radio_hal_set_nsel();

    radio_comm_get_resp(1, radio_cmd);
}

void si4455_start_tx(unsigned char channel, unsigned char condition, unsigned short tx_len) {
    radio_cmd[0] = SI4455_CMD_ID_START_TX;
    radio_cmd[1] = channel;
    radio_cmd[2] = condition;
    radio_cmd[3] = (unsigned char) (tx_len >> 8);
    radio_cmd[4] = (unsigned char) (tx_len);

    radio_comm_send_cmd(SI4455_CMD_ARG_COUNT_START_TX, radio_cmd);
}

void si4455_start_rx(unsigned char channel, unsigned char condition, unsigned short rx_len, unsigned char next_stage1, unsigned char next_stage2, unsigned char next_stage3) {
    radio_cmd[0] = SI4455_CMD_ID_START_RX;
    radio_cmd[1] = channel;
    radio_cmd[2] = condition;
    radio_cmd[3] = (unsigned char) (rx_len >> 8);
    radio_cmd[4] = (unsigned char) (rx_len);
    radio_cmd[5] = next_stage1;
    radio_cmd[6] = next_stage2;
    radio_cmd[7] = next_stage3;

    radio_comm_send_cmd(SI4455_CMD_ARG_COUNT_START_RX, radio_cmd);
}

void si4455_get_int_status(unsigned char ph_clr_pend, unsigned char modem_clr_pend, unsigned char chip_clr_pend) {
    radio_cmd[0] = SI4455_CMD_ID_GET_INT_STATUS;
    radio_cmd[1] = ph_clr_pend;
    radio_cmd[2] = modem_clr_pend;
    radio_cmd[3] = chip_clr_pend;

    radio_comm_send_cmd_get_resp(SI4455_CMD_ARG_COUNT_GET_INT_STATUS, radio_cmd, SI4455_CMD_REPLY_COUNT_GET_INT_STATUS, radio_cmd);

    si4455_cmd.GET_INT_STATUS.INT_PEND = radio_cmd[0];
    si4455_cmd.GET_INT_STATUS.INT_STATUS = radio_cmd[1];
    si4455_cmd.GET_INT_STATUS.PH_PEND = radio_cmd[2];
    si4455_cmd.GET_INT_STATUS.PH_STATUS = radio_cmd[3];
    si4455_cmd.GET_INT_STATUS.MODEM_PEND = radio_cmd[4];
    si4455_cmd.GET_INT_STATUS.MODEM_STATUS = radio_cmd[5];
    si4455_cmd.GET_INT_STATUS.CHIP_PEND = radio_cmd[6];
    si4455_cmd.GET_INT_STATUS.CHIP_STATUS = radio_cmd[7];
}

void si4455_gpio_pin_cfg(unsigned char gpio0, unsigned char gpio1, unsigned char gpio2, unsigned char gpio3, unsigned char nirq, unsigned char sdo, unsigned char gen_config) {
    radio_cmd[0] = SI4455_CMD_ID_GPIO_PIN_CFG;
    radio_cmd[1] = gpio0;
    radio_cmd[2] = gpio1;
    radio_cmd[3] = gpio2;
    radio_cmd[4] = gpio3;
    radio_cmd[5] = nirq;
    radio_cmd[6] = sdo;
    radio_cmd[7] = gen_config;

    radio_comm_send_cmd_get_resp(SI4455_CMD_ARG_COUNT_GPIO_PIN_CFG, radio_cmd, SI4455_CMD_REPLY_COUNT_GPIO_PIN_CFG, radio_cmd);

    si4455_cmd.GPIO_PIN_CFG.GPIO0 = radio_cmd[0];
    si4455_cmd.GPIO_PIN_CFG.GPIO1 = radio_cmd[1];
    si4455_cmd.GPIO_PIN_CFG.GPIO2 = radio_cmd[2];
    si4455_cmd.GPIO_PIN_CFG.GPIO3 = radio_cmd[3];
    si4455_cmd.GPIO_PIN_CFG.NIRQ = radio_cmd[4];
    si4455_cmd.GPIO_PIN_CFG.SPI_SDO = radio_cmd[5];
    si4455_cmd.GPIO_PIN_CFG.GEN_CONFIG = radio_cmd[6];
}

void si4455_change_state(unsigned char next_stage1) {
    radio_cmd[0] = SI4455_CMD_ID_CHANGE_STATE;
    radio_cmd[1] = next_stage1;

    radio_comm_send_cmd(SI4455_CMD_ARG_COUNT_CHANGE_STATE, radio_cmd);
}

void si4455_nop(void) {
    radio_cmd[0] = SI4455_CMD_ID_NOP;
    radio_comm_send_cmd(SI4455_CMD_ARG_COUNT_NOP, radio_cmd);
}

void si4455_fifo_info(unsigned char fifo) {
    radio_cmd[0] = SI4455_CMD_ID_FIFO_INFO;
    radio_cmd[1] = fifo;

    radio_comm_send_cmd_get_resp(SI4455_CMD_ARG_COUNT_FIFO_INFO, radio_cmd, SI4455_CMD_REPLY_COUNT_FIFO_INFO, radio_cmd);

    si4455_cmd.FIFO_INFO.RX_FIFO_COUNT = radio_cmd[0];
    si4455_cmd.FIFO_INFO.TX_FIFO_SPACE = radio_cmd[1];
}

void si4455_part_info(void) {
    radio_cmd[0] = SI4455_CMD_ID_PART_INFO;

    radio_comm_send_cmd_get_resp(SI4455_CMD_ARG_COUNT_PART_INFO, radio_cmd, SI4455_CMD_REPLY_COUNT_PART_INFO, radio_cmd);

    si4455_cmd.PART_INFO.CHIPREV = radio_cmd[0];
    si4455_cmd.PART_INFO.PART = ((unsigned short) radio_cmd[1] << 8) | radio_cmd[2];
    si4455_cmd.PART_INFO.PBUILD = radio_cmd[3];
    si4455_cmd.PART_INFO.ID = ((unsigned short) radio_cmd[4] << 8) | radio_cmd[5];
    si4455_cmd.PART_INFO.CUSTOMER = radio_cmd[6];
    si4455_cmd.PART_INFO.ROMID = radio_cmd[7];
    si4455_cmd.PART_INFO.BOND = radio_cmd[8];
}

void si4455_write_tx_fifo(unsigned char num_bytes, unsigned char *pTxData) {
    radio_comm_write_data(SI4455_CMD_ID_WRITE_TX_FIFO, 0, num_bytes, pTxData);
}

void si4455_read_rx_fifo(unsigned char num_bytes, unsigned char *pRxData) {
    radio_comm_read_data(SI4455_CMD_ID_READ_RX_FIFO, 0, num_bytes, pRxData);
}

void si4455_get_property(unsigned char group, unsigned char num_props, unsigned char start_prop) {
    radio_cmd[0] = SI4455_CMD_ID_GET_PROPERTY;
    radio_cmd[1] = group;
    radio_cmd[2] = num_props;
    radio_cmd[3] = start_prop;

    radio_comm_send_cmd_get_resp(SI4455_CMD_ARG_COUNT_GET_PROPERTY, radio_cmd, radio_cmd[2], radio_cmd);

    si4455_cmd.GET_PROPERTY.DATA0 = radio_cmd[0];
    si4455_cmd.GET_PROPERTY.DATA1 = radio_cmd[1];
    si4455_cmd.GET_PROPERTY.DATA2 = radio_cmd[2];
    si4455_cmd.GET_PROPERTY.DATA3 = radio_cmd[3];
    si4455_cmd.GET_PROPERTY.DATA4 = radio_cmd[4];
    si4455_cmd.GET_PROPERTY.DATA5 = radio_cmd[5];
    si4455_cmd.GET_PROPERTY.DATA6 = radio_cmd[6];
    si4455_cmd.GET_PROPERTY.DATA7 = radio_cmd[7];
    si4455_cmd.GET_PROPERTY.DATA8 = radio_cmd[8];
    si4455_cmd.GET_PROPERTY.DATA9 = radio_cmd[9];
    si4455_cmd.GET_PROPERTY.DATA10 = radio_cmd[10];
    si4455_cmd.GET_PROPERTY.DATA11 = radio_cmd[11];
    si4455_cmd.GET_PROPERTY.DATA12 = radio_cmd[12];
    si4455_cmd.GET_PROPERTY.DATA13 = radio_cmd[13];
    si4455_cmd.GET_PROPERTY.DATA14 = radio_cmd[14];
    si4455_cmd.GET_PROPERTY.DATA15 = radio_cmd[15];
}

void si4455_func_info(void) {
    radio_cmd[0] = SI4455_CMD_ID_FUNC_INFO;

    radio_comm_send_cmd_get_resp(SI4455_CMD_ARG_COUNT_FUNC_INFO, radio_cmd, SI4455_CMD_REPLY_COUNT_FUNC_INFO, radio_cmd);

    si4455_cmd.FUNC_INFO.REVEXT = radio_cmd[0];
    si4455_cmd.FUNC_INFO.REVBRANCH = radio_cmd[1];
    si4455_cmd.FUNC_INFO.REVINT = radio_cmd[2];
    si4455_cmd.FUNC_INFO.PATCH = ((unsigned short) radio_cmd[3] << 8) | radio_cmd[4];
    si4455_cmd.FUNC_INFO.FUNC = radio_cmd[5];
    si4455_cmd.FUNC_INFO.SVNFLAGS = radio_cmd[6];
    si4455_cmd.FUNC_INFO.SVNREV = ((unsigned long) radio_cmd[7] << 24) | ((unsigned long) radio_cmd[8] << 16) | ((unsigned long) radio_cmd[9] << 8) | radio_cmd[10];
}

void si4455_frr_a_read(unsigned char resp_bytes) {
    radio_comm_read_data(SI4455_CMD_ID_FRR_A_READ, 0, resp_bytes, radio_cmd);

    si4455_cmd.FRR_A_READ.FRR_A_VALUE = radio_cmd[0];
    si4455_cmd.FRR_A_READ.FRR_B_VALUE = radio_cmd[1];
    si4455_cmd.FRR_A_READ.FRR_C_VALUE = radio_cmd[2];
    si4455_cmd.FRR_A_READ.FRR_D_VALUE = radio_cmd[3];
}

void si4455_frr_b_read(unsigned char resp_bytes) {
    radio_comm_read_data(SI4455_CMD_ID_FRR_B_READ, 0, resp_bytes, radio_cmd);

    si4455_cmd.FRR_B_READ.FRR_B_VALUE = radio_cmd[0];
    si4455_cmd.FRR_B_READ.FRR_C_VALUE = radio_cmd[1];
    si4455_cmd.FRR_B_READ.FRR_D_VALUE = radio_cmd[2];
    si4455_cmd.FRR_B_READ.FRR_A_VALUE = radio_cmd[3];
}

void si4455_frr_c_read(unsigned char resp_bytes) {
    radio_comm_read_data(SI4455_CMD_ID_FRR_C_READ, 0, resp_bytes, radio_cmd);

    si4455_cmd.FRR_C_READ.FRR_C_VALUE = radio_cmd[0];
    si4455_cmd.FRR_C_READ.FRR_D_VALUE = radio_cmd[1];
    si4455_cmd.FRR_C_READ.FRR_A_VALUE = radio_cmd[2];
    si4455_cmd.FRR_C_READ.FRR_B_VALUE = radio_cmd[3];
}

void si4455_frr_d_read(unsigned char resp_bytes) {
    radio_comm_read_data(SI4455_CMD_ID_FRR_D_READ, 0, resp_bytes, radio_cmd);

    si4455_cmd.FRR_D_READ.FRR_D_VALUE = radio_cmd[0];
    si4455_cmd.FRR_D_READ.FRR_A_VALUE = radio_cmd[1];
    si4455_cmd.FRR_D_READ.FRR_B_VALUE = radio_cmd[2];
    si4455_cmd.FRR_D_READ.FRR_C_VALUE = radio_cmd[3];
}

void si4455_device_state() {
    radio_cmd[0] = SI4455_CMD_ID_REQUEST_DEVICE_STATE;

    radio_comm_send_cmd_get_resp(SI4455_CMD_ARG_COUNT_REQUEST_DEVICE_STATE, radio_cmd, SI4455_CMD_REPLY_COUNT_REQUEST_DEVICE_STATE, radio_cmd);

    si4455_cmd.REQUEST_DEVICE_STATE.CURR_STATE = radio_cmd[0];
    si4455_cmd.REQUEST_DEVICE_STATE.CURRENT_CHANNEL = radio_cmd[1];
}

void si4455_read_cmd_buff(void) {
    radio_cmd[0] = SI4455_CMD_ID_READ_CMD_BUFF;

    radio_comm_send_cmd_get_resp(SI4455_CMD_ARG_COUNT_READ_CMD_BUFF, radio_cmd, SI4455_CMD_REPLY_COUNT_READ_CMD_BUFF, radio_cmd);

    si4455_cmd.READ_CMD_BUFF.CMD_BUFF0 = radio_cmd[0];
    si4455_cmd.READ_CMD_BUFF.CMD_BUFF1 = radio_cmd[1];
    si4455_cmd.READ_CMD_BUFF.CMD_BUFF2 = radio_cmd[2];
    si4455_cmd.READ_CMD_BUFF.CMD_BUFF3 = radio_cmd[3];
    si4455_cmd.READ_CMD_BUFF.CMD_BUFF4 = radio_cmd[4];
    si4455_cmd.READ_CMD_BUFF.CMD_BUFF5 = radio_cmd[5];
    si4455_cmd.READ_CMD_BUFF.CMD_BUFF6 = radio_cmd[6];
    si4455_cmd.READ_CMD_BUFF.CMD_BUFF7 = radio_cmd[7];
    si4455_cmd.READ_CMD_BUFF.CMD_BUFF8 = radio_cmd[8];
    si4455_cmd.READ_CMD_BUFF.CMD_BUFF9 = radio_cmd[9];
    si4455_cmd.READ_CMD_BUFF.CMD_BUFF10 = radio_cmd[10];
    si4455_cmd.READ_CMD_BUFF.CMD_BUFF11 = radio_cmd[11];
    si4455_cmd.READ_CMD_BUFF.CMD_BUFF12 = radio_cmd[12];
    si4455_cmd.READ_CMD_BUFF.CMD_BUFF13 = radio_cmd[13];
    si4455_cmd.READ_CMD_BUFF.CMD_BUFF14 = radio_cmd[14];
    si4455_cmd.READ_CMD_BUFF.CMD_BUFF15 = radio_cmd[15];
}

void si4455_get_adc_reading(unsigned char adc_en, unsigned char adc_cfg) {
    radio_cmd[0] = SI4455_CMD_ID_GET_ADC_READING;
    radio_cmd[1] = adc_en;
    radio_cmd[2] = adc_cfg;

    radio_comm_send_cmd_get_resp(SI4455_CMD_ARG_COUNT_GET_ADC_READING, radio_cmd, SI4455_CMD_REPLY_COUNT_GET_ADC_READING, radio_cmd);

    si4455_cmd.GET_ADC_READING.GPIO_ADC = ((unsigned short) radio_cmd[0] << 8) | radio_cmd[1];
    si4455_cmd.GET_ADC_READING.BATTERY_ADC = ((unsigned short) radio_cmd[2] << 8) | radio_cmd[3];
    si4455_cmd.GET_ADC_READING.TEMP_ADC = ((unsigned short) radio_cmd[4] << 8) | radio_cmd[5];
    si4455_cmd.GET_ADC_READING.TEMP_SLOPE = radio_cmd[6];
    si4455_cmd.GET_ADC_READING.TEMP_INTERCEPT = radio_cmd[7];
}

void si4455_get_ph_status(unsigned char ph_clr_pend) {
    radio_cmd[0] = SI4455_CMD_ID_GET_PH_STATUS;
    radio_cmd[1] = ph_clr_pend;

    radio_comm_send_cmd_get_resp(SI4455_CMD_ARG_COUNT_GET_PH_STATUS, radio_cmd, SI4455_CMD_REPLY_COUNT_GET_PH_STATUS, radio_cmd);

    si4455_cmd.GET_PH_STATUS.PH_PEND = radio_cmd[0];
    si4455_cmd.GET_PH_STATUS.PH_STATUS = radio_cmd[1];
}

void si4455_get_modem_status(unsigned char modem_clr_pend) {
    radio_cmd[0] = SI4455_CMD_ID_GET_MODEM_STATUS;
    radio_cmd[1] = modem_clr_pend;

    radio_comm_send_cmd_get_resp(SI4455_CMD_ARG_COUNT_GET_MODEM_STATUS, radio_cmd, SI4455_CMD_REPLY_COUNT_GET_MODEM_STATUS, radio_cmd);

    si4455_cmd.GET_MODEM_STATUS.MODEM_PEND = radio_cmd[0];
    si4455_cmd.GET_MODEM_STATUS.MODEM_STATUS = radio_cmd[1];
    si4455_cmd.GET_MODEM_STATUS.CURR_RSSI = radio_cmd[2];
    si4455_cmd.GET_MODEM_STATUS.LATCH_RSSI = radio_cmd[3];
    si4455_cmd.GET_MODEM_STATUS.ANT1_RSSI = radio_cmd[4];
    si4455_cmd.GET_MODEM_STATUS.ANT2_RSSI = radio_cmd[5];
    si4455_cmd.GET_MODEM_STATUS.AFC_FREQ_OFFSET = ((unsigned short) radio_cmd[6] << 8) | radio_cmd[7];
}

void si4455_get_chip_status(unsigned char chip_clr_pend) {
    radio_cmd[0] = SI4455_CMD_ID_GET_CHIP_STATUS;
    radio_cmd[1] = chip_clr_pend;

    radio_comm_send_cmd_get_resp(SI4455_CMD_ARG_COUNT_GET_CHIP_STATUS, radio_cmd, SI4455_CMD_REPLY_COUNT_GET_CHIP_STATUS, radio_cmd);

    si4455_cmd.GET_CHIP_STATUS.CHIP_PEND = radio_cmd[0];
    si4455_cmd.GET_CHIP_STATUS.CHIP_STATUS = radio_cmd[1];
    si4455_cmd.GET_CHIP_STATUS.CMD_ERR_STATUS = radio_cmd[2];
}
