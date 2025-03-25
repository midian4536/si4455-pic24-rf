#ifndef RADIO_HAL_H
#define RADIO_HAL_H

void radio_hal_assert_shut_down(void);
void radio_hal_deassert_shut_down(void);

void radio_hal_clear_nsel(void);
void radio_hal_set_nsel(void);

uint8_t radio_hal_nirq_level(void);

void radio_hal_spi_write_byte(uint8_t byte);
uint8_t radio_hal_spi_read_byte(void);

void radio_hal_spi_write_data(uint8_t byte_count, uint8_t *pdata);
void radio_hal_spi_read_data(uint8_t byte_count, uint8_t *pdata);

#endif
