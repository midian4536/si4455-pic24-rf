#include "xc.h"
#include "spi.h"
#include "radio_hal.h"
#include "radio_comm.h"

uint8_t cts_went_high = 0;

uint8_t radio_comm_get_resp(uint8_t byte_count, uint8_t *pdata) {
    uint8_t cts_val = 0;
    uint16_t err_cnt = RADIO_CTS_TIMEOUT;

    while (err_cnt != 0) {
        radio_hal_clear_nsel();
        radio_hal_spi_write_byte(0x44);
        cts_val = radio_hal_spi_read_byte();

        if (cts_val == 0xff) {
            if (byte_count) {
                radio_hal_spi_read_data(byte_count, pdata);
            }
            radio_hal_set_nsel();
            break;
        }

        radio_hal_set_nsel();
        err_cnt--;
    }

    if (err_cnt == 0) {
        while (1) {
        }
    }

    if (cts_val == 0xff) {
        cts_went_high = 1;
    }

    return cts_val;
}

void radio_comm_send_cmd(uint8_t byte_count, uint8_t *pdata) {
    while (!cts_went_high) {
        radio_comm_poll_cts();
    }

    radio_hal_clear_nsel();
    radio_hal_spi_write_data(byte_count, pdata);
    radio_hal_set_nsel();
    cts_went_high = 0;
}

void radio_comm_read_data(uint8_t cmd, uint8_t poll_cts, uint8_t byte_count, uint8_t *pdata) {
    if (poll_cts) {
        while (!cts_went_high) {
            radio_comm_poll_cts();
        }
    }

    radio_hal_clear_nsel();
    radio_hal_spi_write_byte(cmd);
    radio_hal_spi_read_data(byte_count, pdata);
    radio_hal_set_nsel();
    cts_went_high = 0;
}

void radio_comm_write_data(uint8_t cmd, uint8_t poll_cts, uint8_t byte_count, uint8_t *pdata) {
    if (poll_cts) {
        while (!cts_went_high) {
            radio_comm_poll_cts();
        }
    }

    radio_hal_clear_nsel();
    radio_hal_spi_write_byte(cmd);
    radio_hal_spi_write_data(byte_count, pdata);
    radio_hal_set_nsel();
    cts_went_high = 0;
}

uint8_t radio_comm_poll_cts(void) {
    return radio_comm_get_resp(0, 0);
}

uint8_t radio_comm_send_cmd_get_resp(uint8_t cmd_byte_count, uint8_t *p_cmd_data, uint8_t resp_byte_count, uint8_t *p_resp_data) {
    radio_comm_send_cmd(cmd_byte_count, p_cmd_data);
    return radio_comm_get_resp(resp_byte_count, p_resp_data);
}

void radio_comm_clear_cts() {
    cts_went_high = 0;
}
