#include <xc.h>
#include "radio.h"
#include "radio_driver.h"
#include "radio_config.h"
#include "si4455_defs.h"

typedef struct
{
    const uint8_t *radio_configuration_array;
    const uint8_t radio_channel_number;
    const uint8_t radio_packet_length;
    const uint8_t radio_state_after_power_up;
    const uint16_t radio_delay_cnt_after_reset;
} radio_configuration_t;

const uint8_t radio_configuration_data_array[] = RADIO_CONFIGURATION_DATA_ARRAY;
const radio_configuration_t radio_configuration = RADIO_CONFIGURATION_DATA;
const radio_configuration_t *p_radio_configuration = &radio_configuration;
uint8_t custom_radio_packet[RADIO_MAX_PACKET_LENGTH];

void radio_power_up(void)
{
    si4455_reset();
    for (uint16_t delay = 0; delay < p_radio_configuration->radio_delay_cnt_after_reset; delay++)
        ;
}

void radio_init(void)
{
    TRIS_nSel = 0;
    nSel = 1;
    TRIS_SDN = 0;
    SDN = 1;
    TRIS_NIRQ = 1;

    radio_power_up();
    while (SI4455_SUCCESS != si4455_configuration_init(p_radio_configuration->radio_configuration_array))
    {
        for (uint16_t delay = 0x7fff; delay--;)
            ;
        radio_power_up();
    }
    si4455_get_int_status(0, 0, 0);
}

void radio_start_rx(uint8_t channel)
{
    si4455_get_int_status(0, 0, 0);
    si4455_fifo_info(SI4455_CMD_FIFO_INFO_ARG_RX_BIT);
    si4455_start_rx(channel, 0, 0, SI4455_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE, SI4455_CMD_START_RX_ARG_RXVALID_STATE_ENUM_RX, SI4455_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX);
}

void radio_start_tx(uint8_t channel, uint8_t *pdata)
{
    si4455_get_int_status(0, 0, 0);
    si4455_fifo_info(SI4455_CMD_FIFO_INFO_ARG_TX_BIT);
    si4455_write_tx_fifo(pdata, radio_configuration.radio_packet_length);
    si4455_start_tx(channel, 0x30, radio_configuration.radio_packet_length);
}

void radio_start_tx_variable_packet(uint8_t channel, uint8_t *pdata, uint8_t len)
{
    si4455_get_int_status(0, 0, 0);
    si4455_fifo_info(SI4455_CMD_FIFO_INFO_ARG_TX_BIT);
    si4455_write_tx_fifo(pdata, len);
    si4455_start_tx(channel, 0x30, len);
}

bool radio_check_received(void)
{
    if (nIRQ == 0)
    {
        si4455_get_int_status(0, 0, 0);
        if (si4455_cmd.GET_INT_STATUS.PH_PEND & SI4455_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
        {
            si4455_read_rx_fifo(custom_radio_packet, radio_configuration.radio_packet_length);
            return 1;
        }
        if (si4455_cmd.GET_INT_STATUS.PH_PEND & SI4455_CMD_GET_INT_STATUS_REP_CRC_ERROR_BIT)
        {
            si4455_fifo_info(SI4455_CMD_FIFO_INFO_ARG_RX_BIT);
        }
    }
    return 0;
}

bool radio_check_transmitted(void)
{
    if (nIRQ == 0)
    {
        si4455_get_int_status(0, 0, 0);
        if (si4455_cmd.GET_INT_STATUS.PH_PEND & SI4455_CMD_GET_INT_STATUS_REP_PACKET_SENT_PEND_BIT)
        {
            return 1;
        }
    }
    return 0;
}

bool radio_check_received_variable_packet(void)
{
    if (nIRQ == 0)
    {
        si4455_get_int_status(0, 0, 0);

        if (si4455_cmd.GET_INT_STATUS.CHIP_PEND & SI4455_CMD_GET_CHIP_STATUS_REP_CMD_ERROR_PEND_BIT)
        {
            si4455_change_state(SI4455_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_SLEEP);
            si4455_fifo_info(SI4455_CMD_FIFO_INFO_ARG_RX_BIT);
            si4455_change_state(SI4455_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
        }

        if (si4455_cmd.GET_INT_STATUS.MODEM_PEND & SI4455_CMD_GET_INT_STATUS_REP_SYNC_DETECT_BIT)
        {
            ;
        }

        if (si4455_cmd.GET_INT_STATUS.PH_PEND & SI4455_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
        {
            si4455_fifo_info(0);
            si4455_read_rx_fifo(custom_radio_packet, si4455_cmd.FIFO_INFO.RX_FIFO_COUNT);
            return 1;
        }

        if (si4455_cmd.GET_INT_STATUS.PH_PEND & SI4455_CMD_GET_INT_STATUS_REP_CRC_ERROR_BIT)
        {
            si4455_fifo_info(SI4455_CMD_FIFO_INFO_ARG_RX_BIT);
        }
    }
    return 0;
}