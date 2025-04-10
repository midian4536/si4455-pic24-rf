#include "xc.h"
#include "spi.h"
#include "radio_hal.h"

void radio_hal_assert_shut_down(void) {
    SDN = 1;
}

void radio_hal_deassert_shut_down(void) {
    SDN = 0;
}

void radio_hal_clear_nsel(void) {
    nSel = 0;
}

void radio_hal_set_nsel(void) {
    nSel = 1;
}

uint8_t radio_hal_nirq_level(void) {
    return nIRQ;
}

void radio_hal_spi_write_byte(uint8_t byte) {
    spi_transfer(byte);
}

uint8_t radio_hal_spi_read_byte(void) {
    return spi_transfer(0xff);
}

void radio_hal_spi_write_data(uint8_t byte_count, uint8_t *pdata) {
    spi_send_array(pdata, byte_count);
}

void radio_hal_spi_read_data(uint8_t byte_count, uint8_t *pdata) {
    spi_recv_array(pdata, byte_count);
}
