#include <xc.h>
#include <stdbool.h>
#include "spi.h"
#include "radio_driver.h"
#include "si4455_defs.h"

#define RADIO_CTS_TIMEOUT (10000)

bool cts_went_high = 0;

union si4455_cmd_reply_union si4455_cmd;
uint8_t radio_cmd[16];

static inline void radio_hal_assert_shut_down(void)
{
    SDN = 1;
}

static inline void radio_hal_deassert_shut_down(void)
{
    SDN = 0;
}

static inline void radio_hal_clear_nsel(void)
{
    nSel = 0;
}

static inline void radio_hal_set_nsel(void)
{
    nSel = 1;
}

static inline uint8_t radio_hal_nirq_level(void)
{
    return nIRQ;
}

static inline void radio_hal_spi_send_byte(uint8_t byte)
{
    spi_send_byte(byte);
}

static inline uint8_t radio_hal_spi_recv_byte(void)
{
    return spi_recv_byte();
}

static inline void radio_hal_spi_send_bytes(const uint8_t *pdata, uint8_t len)
{
    spi_send_bytes(pdata, len);
}

static inline void radio_hal_spi_recv_bytes(uint8_t *pdata, uint8_t len)
{
    spi_recv_bytes(pdata, len);
}

static uint8_t radio_comm_get_resp(uint8_t *pdata, uint8_t len)
{
    uint8_t cts_val = 0;
    uint16_t err_cnt = RADIO_CTS_TIMEOUT;

    while (err_cnt != 0)
    {
        radio_hal_clear_nsel();
        radio_hal_spi_send_byte(0x44);
        cts_val = radio_hal_spi_recv_byte();

        if (cts_val == 0xff)
        {
            if (len)
            {
                radio_hal_spi_recv_bytes(pdata, len);
            }
            radio_hal_set_nsel();
            break;
        }

        radio_hal_set_nsel();
        err_cnt--;
    }

    if (err_cnt == 0)
    {
        while (1)
        {
        }
    }

    if (cts_val == 0xff)
    {
        cts_went_high = 1;
    }

    return cts_val;
}

static uint8_t radio_comm_poll_cts(void)
{
    return radio_comm_get_resp(0, 0);
}

static void radio_comm_send_cmd(uint8_t *pdata, uint8_t len)
{
    while (!cts_went_high)
    {
        radio_comm_poll_cts();
    }

    radio_hal_clear_nsel();
    radio_hal_spi_send_bytes(pdata, len);
    radio_hal_set_nsel();
    cts_went_high = 0;
}

static void radio_comm_read_data(uint8_t cmd, bool poll_cts, uint8_t *pdata, uint8_t len)
{
    if (poll_cts)
    {
        while (!cts_went_high)
        {
            radio_comm_poll_cts();
        }
    }

    radio_hal_clear_nsel();
    radio_hal_spi_send_byte(cmd);
    radio_hal_spi_recv_bytes(pdata, len);
    radio_hal_set_nsel();
    cts_went_high = 0;
}

static void radio_comm_write_data(uint8_t cmd, bool poll_cts, const uint8_t *pdata, uint8_t len)
{
    if (poll_cts)
    {
        while (!cts_went_high)
        {
            radio_comm_poll_cts();
        }
    }

    radio_hal_clear_nsel();
    radio_hal_spi_send_byte(cmd);
    radio_hal_spi_send_bytes(pdata, len);
    radio_hal_set_nsel();
    cts_went_high = 0;
}

static uint8_t radio_comm_send_cmd_get_resp(uint8_t *pcmd_data, uint8_t cmd_len, uint8_t *presp_data, uint8_t resp_len)
{
    radio_comm_send_cmd(pcmd_data, cmd_len);
    return radio_comm_get_resp(presp_data, resp_len);
}

static void radio_comm_clear_cts()
{
    cts_went_high = 0;
}

void si4455_reset(void)
{
    uint8_t loop_count;

    radio_hal_assert_shut_down();

    for (loop_count = 255; loop_count != 0; loop_count--)
        ;
    radio_hal_deassert_shut_down();
    for (loop_count = 255; loop_count != 0; loop_count--)
        ;
    radio_comm_clear_cts();
}

void si4455_power_up(uint8_t boot_options, uint8_t xtal_options, uint32_t xo_freq)
{
    radio_cmd[0] = SI4455_CMD_ID_POWER_UP;
    radio_cmd[1] = boot_options;
    radio_cmd[2] = xtal_options;
    radio_cmd[3] = (uint8_t)(xo_freq >> 24);
    radio_cmd[4] = (uint8_t)(xo_freq >> 16);
    radio_cmd[5] = (uint8_t)(xo_freq >> 8);
    radio_cmd[6] = (uint8_t)(xo_freq);

    radio_comm_send_cmd(radio_cmd, SI4455_CMD_ARG_COUNT_POWER_UP);
}

uint8_t si4455_configuration_init(const uint8_t *pset_prop_cmd)
{
    uint8_t col, response, len;

    while (*pset_prop_cmd != 0x00)
    {
        len = *pset_prop_cmd++;
        if (len > 16)
        {
            if (SI4455_CMD_ID_WRITE_TX_FIFO == *pset_prop_cmd)
            {
                if (len > 128)
                {

                    return SI4455_COMMAND_ERROR;
                }

                pset_prop_cmd++;
                si4455_write_ezconfig_array(pset_prop_cmd, len - 1);

                pset_prop_cmd += len - 1;

                continue;
            }
            else
            {
                return SI4455_COMMAND_ERROR;
            }
        }

        for (col = 0; col < len; col++)
        {
            radio_cmd[col] = *pset_prop_cmd;
            pset_prop_cmd++;
        }

        if (radio_comm_send_cmd_get_resp(radio_cmd, len, &response, 1) != 0xff)
        {
            return SI4455_CTS_TIMEOUT;
        }

        if (SI4455_CMD_ID_EZCONFIG_CHECK == radio_cmd[0])
        {
            if (response)
            {
                return SI4455_COMMAND_ERROR;
            }
        }

        if (radio_hal_nirq_level() == 0)
        {
            si4455_get_int_status(0, 0, 0);
            return SI4455_COMMAND_ERROR;
        }
    }
    return SI4455_SUCCESS;
}

void si4455_write_ezconfig_array(const uint8_t *pezconfig_array, uint8_t len)
{
    radio_comm_write_data(SI4455_CMD_ID_WRITE_TX_FIFO, 1, pezconfig_array, len);
}

void si4455_ezconfig_check(uint16_t check_sum)
{
    radio_hal_clear_nsel();

    radio_hal_spi_send_byte(SI4455_CMD_ID_EZCONFIG_CHECK);

    radio_hal_spi_send_byte((uint16_t)check_sum >> 8);
    radio_hal_spi_send_byte((uint16_t)check_sum & 0x00ff);

    radio_hal_set_nsel();

    radio_comm_get_resp(radio_cmd, 1);
}

void si4455_start_tx(uint8_t channel, uint8_t condition, uint16_t tx_len)
{
    radio_cmd[0] = SI4455_CMD_ID_START_TX;
    radio_cmd[1] = channel;
    radio_cmd[2] = condition;
    radio_cmd[3] = (uint8_t)(tx_len >> 8);
    radio_cmd[4] = (uint8_t)(tx_len);

    radio_comm_send_cmd(radio_cmd, SI4455_CMD_ARG_COUNT_START_TX);
}

void si4455_start_rx(uint8_t channel, uint8_t condition, uint16_t rx_len, uint8_t next_stage1, uint8_t next_stage2, uint8_t next_stage3)
{
    radio_cmd[0] = SI4455_CMD_ID_START_RX;
    radio_cmd[1] = channel;
    radio_cmd[2] = condition;
    radio_cmd[3] = (uint8_t)(rx_len >> 8);
    radio_cmd[4] = (uint8_t)(rx_len);
    radio_cmd[5] = next_stage1;
    radio_cmd[6] = next_stage2;
    radio_cmd[7] = next_stage3;

    radio_comm_send_cmd(radio_cmd, SI4455_CMD_ARG_COUNT_START_RX);
}

void si4455_get_int_status(uint8_t ph_clr_pend, uint8_t modem_clr_pend, uint8_t chip_clr_pend)
{
    radio_cmd[0] = SI4455_CMD_ID_GET_INT_STATUS;
    radio_cmd[1] = ph_clr_pend;
    radio_cmd[2] = modem_clr_pend;
    radio_cmd[3] = chip_clr_pend;

    radio_comm_send_cmd_get_resp(radio_cmd, SI4455_CMD_ARG_COUNT_GET_INT_STATUS, radio_cmd, SI4455_CMD_REPLY_COUNT_GET_INT_STATUS);

    si4455_cmd.GET_INT_STATUS.INT_PEND = radio_cmd[0];
    si4455_cmd.GET_INT_STATUS.INT_STATUS = radio_cmd[1];
    si4455_cmd.GET_INT_STATUS.PH_PEND = radio_cmd[2];
    si4455_cmd.GET_INT_STATUS.PH_STATUS = radio_cmd[3];
    si4455_cmd.GET_INT_STATUS.MODEM_PEND = radio_cmd[4];
    si4455_cmd.GET_INT_STATUS.MODEM_STATUS = radio_cmd[5];
    si4455_cmd.GET_INT_STATUS.CHIP_PEND = radio_cmd[6];
    si4455_cmd.GET_INT_STATUS.CHIP_STATUS = radio_cmd[7];
}

void si4455_gpio_pin_cfg(uint8_t gpio0, uint8_t gpio1, uint8_t gpio2, uint8_t gpio3, uint8_t nirq, uint8_t sdo, uint8_t gen_config)
{
    radio_cmd[0] = SI4455_CMD_ID_GPIO_PIN_CFG;
    radio_cmd[1] = gpio0;
    radio_cmd[2] = gpio1;
    radio_cmd[3] = gpio2;
    radio_cmd[4] = gpio3;
    radio_cmd[5] = nirq;
    radio_cmd[6] = sdo;
    radio_cmd[7] = gen_config;

    radio_comm_send_cmd_get_resp(radio_cmd, SI4455_CMD_ARG_COUNT_GPIO_PIN_CFG, radio_cmd, SI4455_CMD_REPLY_COUNT_GPIO_PIN_CFG);

    si4455_cmd.GPIO_PIN_CFG.GPIO0 = radio_cmd[0];
    si4455_cmd.GPIO_PIN_CFG.GPIO1 = radio_cmd[1];
    si4455_cmd.GPIO_PIN_CFG.GPIO2 = radio_cmd[2];
    si4455_cmd.GPIO_PIN_CFG.GPIO3 = radio_cmd[3];
    si4455_cmd.GPIO_PIN_CFG.NIRQ = radio_cmd[4];
    si4455_cmd.GPIO_PIN_CFG.SPI_SDO = radio_cmd[5];
    si4455_cmd.GPIO_PIN_CFG.GEN_CONFIG = radio_cmd[6];
}

void si4455_change_state(uint8_t next_stage1)
{
    radio_cmd[0] = SI4455_CMD_ID_CHANGE_STATE;
    radio_cmd[1] = next_stage1;

    radio_comm_send_cmd(radio_cmd, SI4455_CMD_ARG_COUNT_CHANGE_STATE);
}

void si4455_nop(void)
{
    radio_cmd[0] = SI4455_CMD_ID_NOP;
    radio_comm_send_cmd(radio_cmd, SI4455_CMD_ARG_COUNT_NOP);
}

void si4455_fifo_info(uint8_t fifo)
{
    radio_cmd[0] = SI4455_CMD_ID_FIFO_INFO;
    radio_cmd[1] = fifo;

    radio_comm_send_cmd_get_resp(radio_cmd, SI4455_CMD_ARG_COUNT_FIFO_INFO, radio_cmd, SI4455_CMD_REPLY_COUNT_FIFO_INFO);

    si4455_cmd.FIFO_INFO.RX_FIFO_COUNT = radio_cmd[0];
    si4455_cmd.FIFO_INFO.TX_FIFO_SPACE = radio_cmd[1];
}

void si4455_part_info(void)
{
    radio_cmd[0] = SI4455_CMD_ID_PART_INFO;

    radio_comm_send_cmd_get_resp(radio_cmd, SI4455_CMD_ARG_COUNT_PART_INFO, radio_cmd, SI4455_CMD_REPLY_COUNT_PART_INFO);

    si4455_cmd.PART_INFO.CHIPREV = radio_cmd[0];
    si4455_cmd.PART_INFO.PART = ((uint16_t)radio_cmd[1] << 8) | radio_cmd[2];
    si4455_cmd.PART_INFO.PBUILD = radio_cmd[3];
    si4455_cmd.PART_INFO.ID = ((uint16_t)radio_cmd[4] << 8) | radio_cmd[5];
    si4455_cmd.PART_INFO.CUSTOMER = radio_cmd[6];
    si4455_cmd.PART_INFO.ROMID = radio_cmd[7];
    si4455_cmd.PART_INFO.BOND = radio_cmd[8];
}

void si4455_write_tx_fifo(uint8_t *ptx_data, uint8_t len)
{
    radio_comm_write_data(SI4455_CMD_ID_WRITE_TX_FIFO, 0, ptx_data, len);
}

void si4455_read_rx_fifo(uint8_t *prx_data, uint8_t len)
{
    radio_comm_read_data(SI4455_CMD_ID_READ_RX_FIFO, 0, prx_data, len);
}

void si4455_get_property(uint8_t group, uint8_t num_props, uint8_t start_prop)
{
    radio_cmd[0] = SI4455_CMD_ID_GET_PROPERTY;
    radio_cmd[1] = group;
    radio_cmd[2] = num_props;
    radio_cmd[3] = start_prop;

    radio_comm_send_cmd_get_resp(radio_cmd, SI4455_CMD_ARG_COUNT_GET_PROPERTY, radio_cmd, radio_cmd[2]);

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

void si4455_func_info(void)
{
    radio_cmd[0] = SI4455_CMD_ID_FUNC_INFO;

    radio_comm_send_cmd_get_resp(radio_cmd, SI4455_CMD_ARG_COUNT_FUNC_INFO, radio_cmd, SI4455_CMD_REPLY_COUNT_FUNC_INFO);

    si4455_cmd.FUNC_INFO.REVEXT = radio_cmd[0];
    si4455_cmd.FUNC_INFO.REVBRANCH = radio_cmd[1];
    si4455_cmd.FUNC_INFO.REVINT = radio_cmd[2];
    si4455_cmd.FUNC_INFO.PATCH = ((uint16_t)radio_cmd[3] << 8) | radio_cmd[4];
    si4455_cmd.FUNC_INFO.FUNC = radio_cmd[5];
    si4455_cmd.FUNC_INFO.SVNFLAGS = radio_cmd[6];
    si4455_cmd.FUNC_INFO.SVNREV = ((uint32_t)radio_cmd[7] << 24) | ((uint32_t)radio_cmd[8] << 16) | ((uint32_t)radio_cmd[9] << 8) | radio_cmd[10];
}

void si4455_frr_a_read(uint8_t resp_bytes)
{
    radio_comm_read_data(SI4455_CMD_ID_FRR_A_READ, 0, radio_cmd, resp_bytes);

    si4455_cmd.FRR_A_READ.FRR_A_VALUE = radio_cmd[0];
    si4455_cmd.FRR_A_READ.FRR_B_VALUE = radio_cmd[1];
    si4455_cmd.FRR_A_READ.FRR_C_VALUE = radio_cmd[2];
    si4455_cmd.FRR_A_READ.FRR_D_VALUE = radio_cmd[3];
}

void si4455_frr_b_read(uint8_t resp_bytes)
{
    radio_comm_read_data(SI4455_CMD_ID_FRR_B_READ, 0, radio_cmd, resp_bytes);

    si4455_cmd.FRR_B_READ.FRR_B_VALUE = radio_cmd[0];
    si4455_cmd.FRR_B_READ.FRR_C_VALUE = radio_cmd[1];
    si4455_cmd.FRR_B_READ.FRR_D_VALUE = radio_cmd[2];
    si4455_cmd.FRR_B_READ.FRR_A_VALUE = radio_cmd[3];
}

void si4455_frr_c_read(uint8_t resp_bytes)
{
    radio_comm_read_data(SI4455_CMD_ID_FRR_C_READ, 0, radio_cmd, resp_bytes);

    si4455_cmd.FRR_C_READ.FRR_C_VALUE = radio_cmd[0];
    si4455_cmd.FRR_C_READ.FRR_D_VALUE = radio_cmd[1];
    si4455_cmd.FRR_C_READ.FRR_A_VALUE = radio_cmd[2];
    si4455_cmd.FRR_C_READ.FRR_B_VALUE = radio_cmd[3];
}

void si4455_frr_d_read(uint8_t resp_bytes)
{
    radio_comm_read_data(SI4455_CMD_ID_FRR_D_READ, 0, radio_cmd, resp_bytes);

    si4455_cmd.FRR_D_READ.FRR_D_VALUE = radio_cmd[0];
    si4455_cmd.FRR_D_READ.FRR_A_VALUE = radio_cmd[1];
    si4455_cmd.FRR_D_READ.FRR_B_VALUE = radio_cmd[2];
    si4455_cmd.FRR_D_READ.FRR_C_VALUE = radio_cmd[3];
}

void si4455_device_state()
{
    radio_cmd[0] = SI4455_CMD_ID_REQUEST_DEVICE_STATE;

    radio_comm_send_cmd_get_resp(radio_cmd, SI4455_CMD_ARG_COUNT_REQUEST_DEVICE_STATE, radio_cmd, SI4455_CMD_REPLY_COUNT_REQUEST_DEVICE_STATE);

    si4455_cmd.REQUEST_DEVICE_STATE.CURR_STATE = radio_cmd[0];
    si4455_cmd.REQUEST_DEVICE_STATE.CURRENT_CHANNEL = radio_cmd[1];
}

void si4455_read_cmd_buff(void)
{
    radio_cmd[0] = SI4455_CMD_ID_READ_CMD_BUFF;

    radio_comm_send_cmd_get_resp(radio_cmd, SI4455_CMD_ARG_COUNT_READ_CMD_BUFF, radio_cmd, SI4455_CMD_REPLY_COUNT_READ_CMD_BUFF);

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

void si4455_get_adc_reading(uint8_t adc_en, uint8_t adc_cfg)
{
    radio_cmd[0] = SI4455_CMD_ID_GET_ADC_READING;
    radio_cmd[1] = adc_en;
    radio_cmd[2] = adc_cfg;

    radio_comm_send_cmd_get_resp(radio_cmd, SI4455_CMD_ARG_COUNT_GET_ADC_READING, radio_cmd, SI4455_CMD_REPLY_COUNT_GET_ADC_READING);

    si4455_cmd.GET_ADC_READING.GPIO_ADC = ((uint16_t)radio_cmd[0] << 8) | radio_cmd[1];
    si4455_cmd.GET_ADC_READING.BATTERY_ADC = ((uint16_t)radio_cmd[2] << 8) | radio_cmd[3];
    si4455_cmd.GET_ADC_READING.TEMP_ADC = ((uint16_t)radio_cmd[4] << 8) | radio_cmd[5];
    si4455_cmd.GET_ADC_READING.TEMP_SLOPE = radio_cmd[6];
    si4455_cmd.GET_ADC_READING.TEMP_INTERCEPT = radio_cmd[7];
}

void si4455_get_ph_status(uint8_t ph_clr_pend)
{
    radio_cmd[0] = SI4455_CMD_ID_GET_PH_STATUS;
    radio_cmd[1] = ph_clr_pend;

    radio_comm_send_cmd_get_resp(radio_cmd, SI4455_CMD_ARG_COUNT_GET_PH_STATUS, radio_cmd, SI4455_CMD_REPLY_COUNT_GET_PH_STATUS);

    si4455_cmd.GET_PH_STATUS.PH_PEND = radio_cmd[0];
    si4455_cmd.GET_PH_STATUS.PH_STATUS = radio_cmd[1];
}

void si4455_get_modem_status(uint8_t modem_clr_pend)
{
    radio_cmd[0] = SI4455_CMD_ID_GET_MODEM_STATUS;
    radio_cmd[1] = modem_clr_pend;

    radio_comm_send_cmd_get_resp(radio_cmd, SI4455_CMD_ARG_COUNT_GET_MODEM_STATUS, radio_cmd, SI4455_CMD_REPLY_COUNT_GET_MODEM_STATUS);

    si4455_cmd.GET_MODEM_STATUS.MODEM_PEND = radio_cmd[0];
    si4455_cmd.GET_MODEM_STATUS.MODEM_STATUS = radio_cmd[1];
    si4455_cmd.GET_MODEM_STATUS.CURR_RSSI = radio_cmd[2];
    si4455_cmd.GET_MODEM_STATUS.LATCH_RSSI = radio_cmd[3];
    si4455_cmd.GET_MODEM_STATUS.ANT1_RSSI = radio_cmd[4];
    si4455_cmd.GET_MODEM_STATUS.ANT2_RSSI = radio_cmd[5];
    si4455_cmd.GET_MODEM_STATUS.AFC_FREQ_OFFSET = ((uint16_t)radio_cmd[6] << 8) | radio_cmd[7];
}

void si4455_get_chip_status(uint8_t chip_clr_pend)
{
    radio_cmd[0] = SI4455_CMD_ID_GET_CHIP_STATUS;
    radio_cmd[1] = chip_clr_pend;

    radio_comm_send_cmd_get_resp(radio_cmd, SI4455_CMD_ARG_COUNT_GET_CHIP_STATUS, radio_cmd, SI4455_CMD_REPLY_COUNT_GET_CHIP_STATUS);

    si4455_cmd.GET_CHIP_STATUS.CHIP_PEND = radio_cmd[0];
    si4455_cmd.GET_CHIP_STATUS.CHIP_STATUS = radio_cmd[1];
    si4455_cmd.GET_CHIP_STATUS.CMD_ERR_STATUS = radio_cmd[2];
}
